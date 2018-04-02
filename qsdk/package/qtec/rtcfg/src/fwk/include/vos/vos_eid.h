#ifndef __VOS_EID_H__
#define __VOS_EID_H__


/*!\file vos_eid.h
 * \brief Header file containing entity id assignments in
 * the VOS.
 */


/*!\enum VosEntityId
 * \brief Entity Id's (eid) are like process id's (pid), except they identify an
 * application in general, not the specific instance of that application.
 *
 * For example, the ppp application has an entity id of 22.  That entity id
 * refers to the ppp application in general, not a specific instance of ppp,
 * which is uniquely identified by its process id (pid).
 * 
 * Entity Id's are used by the various VOS API's to identify its caller.
 * They are also used by the messaging sub-system to identify the source
 * and destination of messages.  In most cases, there is only one instance
 * of a particular application, so specifying the VosEntityId is enough to
 * identify the application.  However, for applications that may have multiple
 * instances, additional steps may be required.  (For details on how
 * to identify an application for the purpose of sending or receiving messages,
 * see vos_msg.h).
 *
 * Entity Id's and TR69 access bits are bound together by the entityInfoArray
 * in eid.c.  Entities that have TR69 access bits must be listed first.
 *
 * Any changes in this array must also be reflected in entityInfoArray in eid.c
 */
typedef enum {
   EID_INVALID=0,
   EID_TR69C=1,
   EID_TR64C=2,
   EID_HTTPD=3,
   EID_SNMPD=4,
   EID_CONSOLED=5,
   EID_TELNETD=6,
   EID_SSHD=7,
   EID_UPNP=8,
   EID_AVAILABLE1=9,
   EID_VECTORINGD=10,
   EID_VODSL=11,
   EID_CVTPSI=13,
   EID_DECTDBGD=14,
   EID_EPON_APP=15,
   EID_WLNVRAM=17,
   EID_WLWAPID=18,
   EID_WLEVENT=19,
   EID_SMD=20,
   EID_SSK=21,
   EID_PPP=22,
   EID_DHCPC=23,
   EID_DHCPD=24,
   EID_FTPD=25,
   EID_TFTPD=26,
   EID_TFTP=27,
   EID_DNSPROBE=28,
   EID_SYSLOGD=30,
   EID_KLOGD=31,
   EID_DDNSD=33,
   EID_ZEBRA=34,
   EID_RIPD=35,
   EID_SNTP=36,
   EID_URLFILTERD=37,
   EID_TRACERT=39,
   EID_PING=40,
   EID_DHCP6C=41,
   EID_DHCP6S=42,
   EID_RADVD=43,
   EID_DNSPROXY=44,
   EID_IPPD=45,
   EID_FTP=46,
   EID_DSLDIAGD=48,
   EID_SOAPSERVER=49,
   EID_PWRCTL=50,
   EID_HOTPLUG=51,
   EID_L2TPD=52,
   EID_SAMBA=53,
   EID_PPTPD=54,
   EID_DECT=56,
   EID_OMCID=60,
   EID_OMCIPMD=61,   
   EID_RASTATUS6=62,
   EID_UNITTEST=90,
   EID_MISC=91,
   EID_WLWPS=93,
   EID_CMFD=94,
   EID_MCPD=95,
   EID_MOCAD=96,
   EID_RNGD=97,
   EID_DMSD=98,
   EID_WGET=99,
   EID_SWMDK=100,
   EID_OLT_TEST=101,
   EID_OSGID=102,
   EID_BMUD=103,
   EID_BMUCTL=104,
   EID_MCPCTL=107,
   EID_PPPOE_SERVER = 108,
   EID_IGMP = 109,
   EID_IPV6MON=110,
   EID_SIPALG=111,
   EID_MONITORCOLLECTOR=112,
   EID_QTECDEVICEMANAGER,
   EID_MYWEBSOCKET,
   EID_CGIREQ,
   EID_CGIMSGPROC,
   EID_LANHOST,
   EID_SCRCTRL,
   EID_TCP_PROXY,
   EID_WDS_CTRL,
   EID_IMG_UPGRADE,
   EID_QTEC_GETKEY,
   EID_SMB_INIT,
   EID_MYCLIENT_FOR_LOCK,
   EID_TIMER_TASK,
   EID_NET_SNIFFER = 222,
   EID_MODUPDTD=500,
   EID_MODUPDTV=501,
   EID_CMC=502,
   EID_HAL=503,
   EID_GPONHAL=504,
   EID_PCSCD=505,
   EID_VOIPAGENT=506,
   EID_CTCSTP=507,
   EID_MMED = 600,
   EID_DYNAMIC_BASE = 700,   
EID_LAST=65535
} VosEntityId;


/** Param accessible by TR69C;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_TR69C              0x0001

/** Param accessible by TR64C;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_TR64C              0x0002

/** Param accessible by httpd;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_HTTPD              0x0004

/** Param accessible by SNMP;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_SNMPD              0x0008

/** Param accessible by consoled/cli;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_CONSOLED           0x0010

/** Param accessible by telnetd/cli;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_TELNETD            0x0020

/** Param accessible by sshd/cli;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_SSHD               0x0040

/** Param accessible by UPnp;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_UPNP               0x0080

/** Param accessible by ssk;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_SSK                0x0100

/** Param accessible by monitorcollector;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_MONITORCOLLECTOR         0x0200

/** Param accessible by VODSL;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_VODSL              0x0400

/** Param accessible by eoc MME;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_MME               0x0800

/** used for voipagent;  Used in MdmNodeAttributes.accessBitMask.
 */
#define NDA_ACCESS_VOIPAGENT          0x1000

/** Param accessible by DECTDBGD;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_DECTDBGD           0x2000

/** Param accessible by SWMDK;  Used in MdmNodeAttributes.accessBitMask. */
#define NDA_ACCESS_EPONAPP            0x4000


/** List of all management entities other than tr69c.
 *
 * tr69c considers these management entities "subscriber" management entities.
 * This is used by TR69C to grant access to all other management entities.
 * This bit mask is used in MdmNodeAttributes.accessBitMask.
 * See also DEFAULT_ACCESS_BIT_MASK in mdm_defs.h
 *
 */
#define NDA_ACCESS_SUBSCRIBER  (NDA_ACCESS_TR64C        | NDA_ACCESS_SSK        | \
                                NDA_ACCESS_HTTPD        | NDA_ACCESS_SNMPD      | \
                                NDA_ACCESS_CONSOLED     | NDA_ACCESS_TELNETD    | \
                                NDA_ACCESS_SSHD         | NDA_ACCESS_UPNP       | \
                                NDA_ACCESS_MONITORCOLLECTOR   | NDA_ACCESS_VOIPAGENT  | \
                                NDA_ACCESS_VODSL        | NDA_ACCESS_MME        | \
                                NDA_ACCESS_DECTDBGD     | NDA_ACCESS_EPONAPP)


/** Entity is a server, which opens a socket and listens for requests.
 *
 * This flag is used in VosEntityInfo.flags below.
 */
#define EIF_SERVER                    0x01


/** If entity is a server, server socket uses TCP (otherwise, uses UDP)
 *
 * This flag is used in VosEntityInfo.flags below.
 */
#define EIF_SERVER_TCP                0x02


/** Entity should be launched during stage 1 of VOS startup..
 *
 * This flag is used in VosEntityInfo.flags below.
 * So far, the only app we launch during stage 1 is ssk.
 */
#define EIF_LAUNCH_IN_STAGE_1         0x04


/** Entity should be launched on system boot (during stage 2).
 *
 * This flag is used in VosEntityInfo.flags below.
 */
#define EIF_LAUNCH_ON_BOOT            0x08


/** Entity will need access to the MDM.
 *
 * This flag is used in VosEntityInfo.flags below.
 * This flag will cause smd to pass the MDM shared memory id to the
 * app in the -m option.  
 * If the entity is going to access the MDM, it must have a NDA_ACCESS_xxx
 * bit defined for it.
 */
#define EIF_MDM                       0x10


/** Entity can run on the Desktop Linux.
 *
 * Some entities, such as httpd, can run on the Desktop Linux as
 * well as modems.  Others, such as dhcpd, can only run on the modem.
 */
#define EIF_DESKTOP_LINUX_CAPABLE     0x20


/** Entity will create a vos_msg communications link to smd. */
#define EIF_MESSAGING_CAPABLE         0x40


/** Entity can have multiple instances, currently only ppp and dhcpc */
#define EIF_MULTIPLE_INSTANCES        0x80


/** Entity supports IPv6. */
#define EIF_IPV6                      0x100

/** Tell smd to relaunch this App if it dies abnormally (sigsegv, etc) */
#define EIF_AUTO_RELAUNCH             0x200

/* tell the progress been monitoring */
#define EIF_MONITOR                   0x400

#define EIF_NONE             0x000


/** A structure that contains info about an VosEntityId.
 *
 * For each new VosEntityId defined, add a corresponding entry to the
 * EntityInfoArray in vos_util/eid.c
 *
 */
typedef struct
{
   VosEntityId eid;    /**< The entity id for this entry. */
   UINT16 accessBit;   /**< This entity's bit position in MdmNodeAttributes.accessBitMask (only 15 bits available) */
   char *name;         /**< Short name for this entity. */
   char *path;         /**< Full path to the executable. */
   char *runArgs;      /**< Args to pass to this executable on startup */
   UINT16 flags;       /**< Additional info about this entity, see EIF_xxx */
   UINT8  backLog;     /**< Backlog arg for the server socket */
   UINT16 port;        /**< If server, port number for the server socket. */
   UBOOL8 isFeatureCompiledIn; /**< Is this feature selected via make menuconfig */
   UINT32 maxMemUsage;    /**< Maximum memory usage, in KB, 0 if unknown. */
   UINT32 normalMemUsage; /**< Typical memory usage, in KB, 0 if unknown. */
   UINT32 minMemUsage;    /**< Minimum memory usage, in KB, 0 if unknown. */
   SINT32 lifeNum;
} VosEntityInfo;


typedef struct
{
    VosEntityId eid;
    UINT16 accessBit;
    char name[32];
    char path[128];
    char runArgs[256];
    UINT16 flags;
    UINT8 backLog;
    UINT16 port;
    UBOOL8 isFeartureComiledIn;
    UINT32 maxMemUsage;
    UINT32 normalMemUsage;
    UINT32 minMemUsage;
}VosEntityInfoMsg;


/** Get the VosEntityInfo for the specified VosEntityId.
 *
 * @param eid (IN) VosEntityId to get info on.
 * @return pointer to EntityInfo structure corresponding to the specified eid.
 *         Caller must not modify the returned VosEntityInfo structure.
 *         Caller must not free the returned VosEntityInfo structure.
 *         Returns NULL if VosEntityId is invalid.
 */
const VosEntityInfo *vosEid_getEntityInfo(VosEntityId eid);


/** Get the VosEntityInfo for the specified bit in the bitmask.
 *
 * @param bit (IN) A bit in the MdmNodeAttributes.accessBitMask.  Only a 
 *                 single bit is allowed.
 * @return pointer to EntityInfo structure corresponding to the specified bit.
 *         Caller must not modify the returned VosEntityInfo structure.
 *         Returns NULL if bit is not recognized.
 */
const VosEntityInfo *vosEid_getEntityInfoByAccessBit(UINT16 bit);


/** Get the VosEntityInfo for the specified entity's short string name.
 *
 * @param stringName (IN) The short string name of the entity.
 * @return pointer to EntityInfo structure corresponding to the specified name.
 *         Caller must not modify the returned VosEntityInfo structure.
 *         Caller must not free the returned VosEntityInfo structure.
 *         Returns NULL if name is not recognized.
 */
const VosEntityInfo *vosEid_getEntityInfoByStringName(const char *stringName);


/** Convert the given bitmask into a buffer of comma separated string names.
 *
 * If the bitmask contains bits which are not recognized, this function
 * will ignore them and fill the buffer with the string names for bits that it
 * does recognize and return VOS_RET_SUCCESS_UNRECOGNIZED_DATA_IGNORED.
 * If the bitmask is empty or does not contain any bits which this function
 * recognized, a buffer containing empty string will be returned.
 *
 * @param bitMask  (IN) bitmask.
 * @param buf     (OUT) A buffer containing the comma separated string names of the
 *                      entities in the bit mask.  The caller is responsible for
 *                      freeing the string buffer.
 * @return VOS_RET_E enum.
 */
VOS_RET_E vosEid_getStringNamesFromBitMask(UINT16 bitMask, char **buf);


/** Convert the given comma separated strings names into a bit mask.
 *
 * If the buffer contains string names which are not recognized, this function
 * will ignore them and fill the bitMask with bits for the string names that it
 * does recognize and return VOS_RET_SUCCESS_UNRECOGNIZED_DATA_IGNORED.
 *
 * @param buf      (IN) A buffer containing the comma separated string names of the
 *                      entities in the bit mask.  The caller is responsible for
 *                      freeing the string buffer.
 * @param bitMask (OUT) Resulting bitmask representation.
 *
 * @return VOS_RET_E enum.
 */
VOS_RET_E vosEid_getBitMaskFromStringNames(const char *buf, UINT16 *bitMask);


/** Get the process/thread id of the caller.
 *
 * Unlike the entity id, a pid is unique for all processes.  So two
 * instances of the same entity id will have different pid's.  This is
 * useful when you need to distinguish among multiple instances of
 * the same eid.
 *
 * @return SINT32 representing the pid.
 */
SINT32 vosEid_getPid(void);


/** Get the number of enties in the entityInfoArray.
 *
 * This function is for use by smd/oal_event.c.  Other callers should not use it.
 * 
 * @return number of entries in the entityInfoArray.
 */
UINT32 vosEid_getNumberOfEntityInfoEntries(void);


/** Get the first entry in the entityInfoArray.
 *
 * This function is for use by smd/oal_event.c.  Other callers should not use it.
 * 
 * @return pointer to the first entry in the entityInfoArray.
 */
const VosEntityInfo *vosEid_getFirstEntityInfo(void);

VOS_RET_E vosEid_setServerPort(VosEntityId eid, UINT16 port);

VosEntityId vosEid_addDynamicEntityInfo(const char *appName, const char *args);

VOS_RET_E vosEid_delDynamicEntityInfo(const char * appName);

void vosEid_init();


/** Mask to get the generic eid out of a UINT32 eid field. */
#define EID_EID_MASK  0x0000ffff

/** Mask to get the pid out of a UINT32 eid field */
#define EID_PID_MASK  0xffff0000

/** Get the generic eid out of a UINT32 eid field */
#define GENERIC_EID(e) (((UINT32) e) & EID_EID_MASK)

/** Get the pid out of a UINT32 eid field */
#define PID_IN_EID(e) ((((UINT32) e) & EID_PID_MASK) >> 16)

/** Make a specific UINT32 eid field out of a pid and a generic eid */
#define MAKE_SPECIFIC_EID(p, e) ((p << 16) | (((UINT32) e) & EID_EID_MASK))

#endif  /* __VOS_EID_H__ */
