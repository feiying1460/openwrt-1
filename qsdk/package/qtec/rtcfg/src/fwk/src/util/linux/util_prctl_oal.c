#include "fwk.h"
#include "util_prctl_oal.h"
#include <sys/wait.h>  /* for waitpid */
#include <fcntl.h>     /* for open */
#include <errno.h>
#include <dirent.h>


/* amount of time to sleep between collect process attempt if timeout was specified. */
#define COLLECT_WAIT_INTERVAL_MS 40


static void freeArgs(char **argv);
VOS_RET_E parseArgs(const char *cmd, const char *args, char ***argv);


VOS_RET_E oal_spawnProcess(const SpawnProcessInfo *spawnInfo, SpawnedProcessInfo *procInfo)
{
   char **argv=NULL;
   SINT32 pid, i;
   VOS_RET_E ret=VOS_RET_SUCCESS;

   if ((ret = parseArgs(spawnInfo->exe, spawnInfo->args, &argv)) != VOS_RET_SUCCESS)
   {
      return ret;
   }

   pid = fork();
   if (pid == 0)
   {
      SINT32 devNullFd=-1, fd;
      char appName[BUFLEN_512] = {0};                //add buffer length for support sim add by lqy 2013.7.2 
      char *strName = NULL;

      /*
       * This is the child.
       */

      /*
       * If user gave us specific instructions on fd re-routing,
       * do it before execv.
       */
      if ((spawnInfo->stdinFd == -1) ||
          (spawnInfo->stdoutFd == -1) ||
          (spawnInfo->stderrFd == -1)) {
         devNullFd = open("/dev/null", O_RDWR);
         if (devNullFd == -1)
         {
            fprintf(stderr, "%s:%d:open /dev/null failed\n",
                __FUNCTION__, __LINE__);
            exit(-1);
         }
      }

      if (spawnInfo->stdinFd != 0)
      {
         close(0);
         fd = (spawnInfo->stdinFd == -1) ? devNullFd : spawnInfo->stdinFd;
         dup2(fd, 0);
      }

      if (spawnInfo->stdoutFd != 1)
      {
         close(1);
         fd = (spawnInfo->stdoutFd == -1) ? devNullFd : spawnInfo->stdoutFd;
         dup2(fd, 1);
      }

      if (spawnInfo->stderrFd != 2)
      {
         close(2);
         fd = (spawnInfo->stderrFd == -1) ? devNullFd : spawnInfo->stderrFd;
         dup2(fd, 2);
      }

      if (devNullFd != -1)
      {
         close(devNullFd);
      }

      /* if child has a serverFd, dup it to the fixed number */
      if (spawnInfo->serverFd != -1)
      {
         close(VOS_DYNAMIC_LAUNCH_SERVER_FD);         
         dup2(spawnInfo->serverFd, VOS_DYNAMIC_LAUNCH_SERVER_FD);
      }

      /* close all of the child's other fd's */
      for (i=3; i <= spawnInfo->maxFd; i++)
      {
         if ((spawnInfo->serverFd != -1)  &&
             (i == VOS_DYNAMIC_LAUNCH_SERVER_FD))
         {
            continue;
         }
         close(i);
      }

      /*
       * Set all signal handlers back to default action,
       * in case the parent had set them to SIG_IGN or something.
       * See kernel/linux/include/asm-mips/signal.h
       */
      signal(SIGHUP, SIG_DFL);
      if (spawnInfo->inheritSigint == FALSE)
      {
         /*
          * Note: there is a bug in pthread library in the 4.4.2 toolchain
          * which breaks SIG_IGN inheritance.  Apps are advised to set
          * SIGINT handler explicitly in the app itself.
          */
         signal(SIGINT, SIG_DFL);
      }
      signal(SIGQUIT, SIG_DFL);
      signal(SIGILL, SIG_DFL);
      signal(SIGTRAP, SIG_DFL);
      signal(SIGABRT, SIG_DFL);  /* same as SIGIOT */
#ifndef DESKTOP_LINUX
      //signal(SIGEMT, SIG_DFL);
#endif
      signal(SIGFPE, SIG_DFL);
      signal(SIGBUS, SIG_DFL);
      signal(SIGSEGV, SIG_DFL);
      signal(SIGSYS, SIG_DFL);
      signal(SIGPIPE, SIG_DFL);
      signal(SIGALRM, SIG_DFL);
      signal(SIGTERM, SIG_DFL);
      signal(SIGUSR1, SIG_DFL);
      signal(SIGUSR2, SIG_DFL);
      signal(SIGCHLD, SIG_DFL);  /* same as SIGCLD */
      signal(SIGPWR, SIG_DFL);
      signal(SIGWINCH, SIG_DFL);
      signal(SIGURG, SIG_DFL);
      signal(SIGIO, SIG_DFL);    /* same as SIGPOLL */
      signal(SIGSTOP, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
      signal(SIGCONT, SIG_DFL);
      signal(SIGTTIN, SIG_DFL);
      signal(SIGTTOU, SIG_DFL);
      signal(SIGVTALRM, SIG_DFL);
      signal(SIGPROF, SIG_DFL);
      signal(SIGXCPU, SIG_DFL);
      signal(SIGXFSZ, SIG_DFL);

      UTIL_STRNCPY(appName, spawnInfo->exe, sizeof(appName));
      strName = strrchr(appName, '/');
      if (strName)
      {
          strName++;
      }

      if (!strName || *strName == '\0')
      {
          fprintf(stderr, "%s:%d:Invalid process name %s\n",
              __FUNCTION__, __LINE__, spawnInfo->exe);

          return VOS_RET_INTERNAL_ERROR;
      }

      /* overlay child executable over myself */
      execv(spawnInfo->exe, argv);

      /* We should not reach this line.  If we do, exec has failed. */
      exit(-1);
   }

   /* this is the parent */

   freeArgs(argv); /* don't need these anymore */

   memset(procInfo, 0, sizeof(SpawnedProcessInfo));
   procInfo->pid = pid;
   procInfo->status = PSTAT_RUNNING;

   if (spawnInfo->spawnMode == SPAWN_AND_WAIT)
   {
      CollectProcessInfo collectInfo;

      collectInfo.collectMode = COLLECT_PID_TIMEOUT;
      collectInfo.pid = pid;
      collectInfo.timeout = spawnInfo->timeout;

      ret = oal_collectProcess(&collectInfo, procInfo);
      if (ret == VOS_RET_TIMED_OUT)
      {
         VOS_RET_E r2;

         vosLog_debug("pid %d has not exited in %d ms, SIGKILL", pid, spawnInfo->timeout);
         r2 = oal_signalProcess(pid, SIGKILL);
         if (r2 != VOS_RET_SUCCESS)
         {
            vosLog_error("SIGKILL to pid %d failed with ret=%d", pid, r2);
         }

         /* try one more time to collect it */
         collectInfo.collectMode = COLLECT_PID_TIMEOUT;
         collectInfo.pid = pid;
         collectInfo.timeout = COLLECT_WAIT_INTERVAL_MS;
         r2 = oal_collectProcess(&collectInfo, procInfo);
         if (r2 == VOS_RET_SUCCESS)
         {
            /* we finally go it, otherwise, leave ret at VOS_RET_TIMED_OUT */
            ret = VOS_RET_SUCCESS;
         }
      }
      else if (ret != VOS_RET_SUCCESS)
      {
         /* some other error with collect */
         vosLog_error("Could not collect pid %d", pid);
      }
   }

   return ret;
}


VOS_RET_E oal_collectProcess(const CollectProcessInfo *collectInfo, SpawnedProcessInfo *procInfo)
{
   SINT32 rc, status, waitOption=0;
   SINT32 requestedPid=-1;
   UINT32 timeoutRemaining=0;
   UINT32 sleepTime;
   VOS_RET_E ret=VOS_RET_SUCCESS;

   memset(procInfo, 0, sizeof(SpawnedProcessInfo));


   switch(collectInfo->collectMode)
   {
   case COLLECT_PID:
      requestedPid = collectInfo->pid;
      break;

   case COLLECT_PID_TIMEOUT:
      requestedPid = collectInfo->pid;
      timeoutRemaining = collectInfo->timeout;
      waitOption = WNOHANG;
      break;

   case COLLECT_ANY:
      requestedPid = -1;
      break;

   case COLLECT_ANY_TIMEOUT:
      requestedPid = -1;
      timeoutRemaining = collectInfo->timeout;
      waitOption = WNOHANG;
      break;
   }


   /*   
    * Linux does not offer a "wait for pid to exit for this amount of time"
    * system call, so I simulate that feature by doing a waitpid with 
    * WNOHANG and then sleeping for a short amount of time (50ms) between
    * checks.  The loop is a bit tricky because it has to handle callers
    * who don't want to wait at all and callers who do want to wait for
    * a specified time interval.  For callers who don't want to wait
    * (collectInfo->timeout == 0), timeoutRemaining will get set to 1,
    * which means they go through the loop exactly once.
    * For callers who want to wait a specified number of milliseconds,
    * we must go through the loop at least twice, once for the initial waitpid,
    * then a sleep, then one more waitpids before timing out (hence the weird
    * +/- 1 adjustments in the timeoutRemaining and sleepTime calculations).
    */
   timeoutRemaining = (timeoutRemaining <= 1) ?
                      (timeoutRemaining + 1) : timeoutRemaining;
   while (timeoutRemaining > 0)
   {
      rc = waitpid(requestedPid, &status, waitOption);
      if (rc == 0)
      {
         /*
          * requested process or any process has not exited.
          * Possibly sleep so we can check again, and calculate time remaining.
          */
         if (timeoutRemaining > 1)
         {
            sleepTime = (timeoutRemaining > COLLECT_WAIT_INTERVAL_MS) ?
                         COLLECT_WAIT_INTERVAL_MS : timeoutRemaining - 1;
            usleep(sleepTime * USECS_IN_MSEC);
            timeoutRemaining -= sleepTime;
         }
         else
         {
            timeoutRemaining = 0;
         }
      }
      else if (rc > 0)
      {
         /* OK, we got a process.  Fill out info. */
         procInfo->pid = rc;
         procInfo->status = PSTAT_EXITED;
         procInfo->signalNumber = WIFSIGNALED(status) ? WTERMSIG(status) : 0;
         procInfo->exitCode = WEXITSTATUS(status);

         timeoutRemaining = 0;
      }
      else
      {
         /* maybe a bad pid was specified or there are no children to wait for */
         vosLog_error("possible bad pid %d, errno=%d", requestedPid, errno);
         ret = VOS_RET_INVALID_ARGUMENTS;
         timeoutRemaining = 0;
      }
   }


   if (procInfo->status != PSTAT_EXITED)
   {
      /* the requested process or any process has not exited. */
      ret = VOS_RET_TIMED_OUT;
   }

   return ret;
}

VOS_RET_E oal_signalProcess(SINT32 pid, SINT32 sig)
{
   SINT32 rc;

   if (pid <= 0)
   {
      vosLog_error("bad pid %d", pid);
      return VOS_RET_INVALID_ARGUMENTS;
   }

   vosLog_debug("pid = %d, sig = %d", pid, sig);
   
   if ((rc = kill(pid, sig)) < 0)
   {
      vosLog_error("invalid signal(%d) or pid(%d)", sig, pid);
      return VOS_RET_INVALID_ARGUMENTS;
   }

   vosLog_debug("send signal succ");

   return VOS_RET_SUCCESS;
}



/** Give a single string, allocate and fill in an array of char *'s
 * each pointing to an individually malloc'd buffer containing a single
 * argument in the string; the array will end with a char * slot containing NULL.
 *
 * This array can be passed to execv.
 * This array must be freed by calling freeArgs.
 */
VOS_RET_E parseArgs(const char *cmd, const char *args, char ***argv)
{
    UINT32 numArgs = 0;
    UINT32 argIndex = 0;
    char **array = NULL;
    char *dupArgs = NULL;
    const char *arg = NULL;
    char *saveptr = NULL;
    const char *delim = " \t";

    vosLog_debug("cmd = %s, args = %s, argv = %p",
        cmd ? cmd : "", args ? args : "", argv);

    if (NULL == argv)
    {
        vosLog_error("NULL == argv");
        return VOS_RET_INVALID_ARGUMENTS;
    }

    numArgs = 2;

    if (args && (util_strlen(args) > 0))
    {
        dupArgs = VOS_STRDUP(args);
        if (NULL == dupArgs)
        {
            vosLog_error("dup args failed");
            return VOS_RET_RESOURCE_EXCEEDED;
        }

        arg = strtok_r(dupArgs, delim, &saveptr);
        if (arg)
        {
            numArgs++;

            while ((arg = strtok_r(NULL, delim, &saveptr)))
            {
                numArgs++;
            }
        }
    }

    array = (char **) VOS_MALLOC_FLAGS(numArgs * sizeof(char *), ALLOC_ZEROIZE);
    if (NULL == array)
    {
        vosLog_error("malloc failed");
        VOS_FREE(dupArgs);
        return VOS_RET_RESOURCE_EXCEEDED;
    }

    if (cmd && (util_strlen(cmd) > 0))
    {
        arg = strrchr(cmd, '/');
        if (arg)
        {
            arg++;
        }
        else
        {
            arg = cmd;
        }

        array[argIndex++] = VOS_STRDUP(arg);
        if (NULL == array[argIndex - 1])
        {
            vosLog_error("dup arg failed");
            VOS_FREE(dupArgs);
            freeArgs(array);
            return VOS_RET_RESOURCE_EXCEEDED;
        }
    }
    
    if (dupArgs)
    {
        UTIL_STRNCPY(dupArgs, args, util_strlen(args) + 1);
        
        arg = strtok_r(dupArgs, delim, &saveptr);
        if (arg)
        {
            array[argIndex++] = VOS_STRDUP(arg);
            if (NULL == array[argIndex - 1])
            {
                vosLog_error("dup arg failed");
                VOS_FREE(dupArgs);
                freeArgs(array);
                return VOS_RET_RESOURCE_EXCEEDED;
            }

            while ((arg = strtok_r(NULL, delim, &saveptr)))
            {
                array[argIndex++] = VOS_STRDUP(arg);
                if (NULL == array[argIndex - 1])
                {
                    vosLog_error("dup arg failed");
                    VOS_FREE(dupArgs);
                    freeArgs(array);
                    return VOS_RET_RESOURCE_EXCEEDED;
                }
            }
        }

        VOS_FREE(dupArgs);
    }

    argIndex = 0;
    while (array[argIndex])
    {
        vosLog_debug("argv[%u] = %s", argIndex, array[argIndex]);
        argIndex++;
    }

    (*argv) = array;

    return VOS_RET_SUCCESS;
}


/** Free all the arg buffers in the argv, and the argv array itself.
 *
 */
void freeArgs(char **argv)
{
   UINT32 i=0;

   while (argv[i] != NULL)
   {
      VOS_MEM_FREE_BUF_AND_NULL_PTR(argv[i]);
      i++;
   }

   VOS_MEM_FREE_BUF_AND_NULL_PTR(argv);
}


int oal_getPidByName(const char *name)
{
   DIR *dir;
   FILE *fp;
   struct dirent *dent;
   UBOOL8 found=FALSE;
   int pid, rc, p, i;
   int rval = UTIL_INVALID_PID;
   char filename[BUFLEN_256];
   char processName[BUFLEN_256];

   if (NULL == (dir = opendir("/proc")))
   {
      vosLog_error("could not open /proc");
      return rval;
   }

   while (!found && (dent = readdir(dir)) != NULL)
   {
      /*
       * Each process has its own directory under /proc, the name of the
       * directory is the pid number.
       */
      if ((dent->d_type == DT_DIR) &&
          (VOS_RET_SUCCESS == util_strtol(dent->d_name, NULL, 10, &pid)))
      {
         snprintf(filename, sizeof(filename), "/proc/%d/stat", pid);
         if ((fp = fopen(filename, "r")) == NULL)
         {
            vosLog_error("could not open %s", filename);
         }
         else
         {
            /* Get the process name, format: 913 (consoled) */
            memset(processName, 0, sizeof(processName));
            rc = fscanf(fp, "%d (%s", &p, processName);
            fclose(fp);

            if (rc >= 2)
            {
               i = strlen(processName);
               if (i > 0)
               {
                  /* strip out the trailing ) character */
                  if (processName[i-1] == ')')
                     processName[i-1] = 0;
               }
            }

            if (!util_strcmp(processName, name))
            {
               rval = pid;
               found = TRUE;
            }
         }
      }
   }

   closedir(dir);

   return rval;
}

