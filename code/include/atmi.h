#ifndef __LIBTUX_ATMI_HEADER__
#define __LIBTUX_ATMI_HEADER__

#ifdef WIN32
#	ifdef LIBTUX_BUILD_DLL
#		define LIBTUX_EXPORT __declspec(dllexport)
#	else
#		define LIBTUX_EXPORT __declspec(dllimport)
#	endif
#else
#	define LIBTUX_EXPORT
#endif

/*
 * tperrno values - error codes
 */
#define TPMINVAL			0	/* minimum error message */
#define TPEABORT			1
#define TPEBADDESC			2
#define TPEBLOCK			3
#define TPEINVAL			4
#define TPELIMIT			5
#define TPENOENT			6
#define TPEOS				7
#define TPEPERM				8
#define TPEPROTO			9
#define TPESVCERR			10
#define TPESVCFAIL			11
#define TPESYSTEM			12
#define TPETIME				13
#define TPETRAN				14
#define TPGOTSIG			15
#define TPERMERR			16
#define TPEITYPE			17
#define TPEOTYPE			18
#define TPERELEASE			19
#define TPEHAZARD			20
#define TPEHEURISTIC		21
#define TPEEVENT			22
#define TPEMATCH			23
#define TPEDIAGNOSTIC		24
#define TPEMIB				25
#define TPMAXVAL			26	/* maximum error message */

#define TUXONE_SERVICE_NAME_LENGTH		32
#define XATMI_SERVICE_NAME_LENGTH		32
#define MAXTIDENT						30		/* max len of a /T identifier */

/* Flags to service routines */
#define TPNOBLOCK               0x00000001      /* non-blocking send/rcv */
#define TPSIGRSTRT              0x00000002      /* restart rcv on interrupt */
#define TPNOREPLY               0x00000004      /* no reply expected */
#define TPNOTRAN                0x00000008      /* not sent in transaction mode */
#define TPTRAN                  0x00000010      /* sent in transaction mode */
#define TPNOTIME                0x00000020      /* no timeout */
#define TPABSOLUTE              0x00000040      /* absolute value on tmsetprio */
#define TPGETANY                0x00000080      /* get any valid reply */
#define TPNOCHANGE              0x00000100      /* force incoming buffer to match */
#define RESERVED_BIT1           0x00000200      /* reserved for future use */
#define TPCONV                  0x00000400      /* conversational service */
#define TPSENDONLY              0x00000800      /* send-only mode */
#define TPRECVONLY              0x00001000      /* recv-only mode */
#define TPACK                   0x00002000      /* */
#define RESERVED_BIT2           0x00004000      /* reserved for future use */
#define RESERVED_BIT3           0x00008000      /* reserved for future use */
#define RESERVED_BIT4           0x00010000      /* reserved for future use */
#define RESERVED_BIT5           0x00020000      /* reserved for future use */

/**
 * Flags to tpreturn()
 */
#define TPFAIL		0x00000001	/* service FAILure for tpreturn */
#define TPSUCCESS	0x00000002	/* service SUCCESS for tpreturn */
#define TPEXIT		0x08000000	/* service failue with server exit */

/**
 * tpinit(3) interface structure
 */
typedef struct	tpinfo_t
{
	char	usrname[MAXTIDENT + 2];	/* client user name */
	char	cltname[MAXTIDENT + 2];	/* application client name */
	char	passwd[MAXTIDENT + 2];	/* application password */
	char	grpname[MAXTIDENT + 2];	/* client group name */
	long	flags;					/* initialization flags */
	long	datalen;				/* length of app specific data */
	long	data;					/* placeholder for app data */
} TPINIT;

/**
 * client identifier structure
 */
typedef struct clientid_t
{
	long	clientdata[4];		/* reserved for internal use */
} CLIENTID;

/**
 * interface to service routines
 */
typedef struct tpsvcinfo
{
	char	name[XATMI_SERVICE_NAME_LENGTH];/* service name invoked */
	long	flags;		/* describes service attributes */
	char	*data;		/* pointer to data */
	long	len;		/* request data length */
	int		cd;			/* connection descriptor */
	long	appkey;		/* application authentication client key */
	CLIENTID cltid;		/* client identifier for originating client */
} TPSVCINFO;

#define TMQNAMELEN	15
#define TMMSGIDLEN	32
#define TMCORRIDLEN	32

/**
 * control parameters to queue primitives
 */
typedef struct tpqctl_t
{
	long flags;			/* indicates which of the values are set */
	long deq_time;		/* absolute/relative  time for dequeuing */
	long priority;		/* enqueue priority */
	long diagnostic;	/* indicates reason for failure */
	char msgid[TMMSGIDLEN];				/* id of message before which to queue */
	char corrid[TMCORRIDLEN];			/* correlation id used to identify message */
	char replyqueue[TMQNAMELEN + 1];	/* queue name for reply message */
	char failurequeue[TMQNAMELEN + 1];	/* queue name for failure message */
	CLIENTID cltid;		/* client identifier for originating client */
	long urcode;		/* application user-return code */
	long appkey;		/* application authentication client key */
	long delivery_qos;	/* delivery quality of service  */
	long reply_qos;		/* reply message quality of service  */
	long exp_time;		/* expiration time  */
} TPQCTL;

/**
 * structure elements that are valid - set in flags
 */
#define TPNOFLAGS			0x00000
#define	TPQCORRID			0x00001		/* set/get correlation id */
#define	TPQFAILUREQ			0x00002		/* set/get failure queue */
#define	TPQBEFOREMSGID		0x00004		/* enqueue before message id */
#define	TPQGETBYMSGIDOLD	0x00008		/* deprecated */
#define	TPQMSGID			0x00010		/* get msgid of enq/deq message */
#define	TPQPRIORITY			0x00020		/* set/get message priority */
#define	TPQTOP				0x00040		/* enqueue at queue top */
#define	TPQWAIT				0x00080		/* wait for dequeuing */
#define	TPQREPLYQ			0x00100		/* set/get reply queue */
#define	TPQTIME_ABS			0x00200		/* set absolute time */
#define	TPQTIME_REL			0x00400		/* set absolute time */
#define	TPQGETBYCORRIDOLD	0x00800		/* deprecated */
#define	TPQPEEK				0x01000		/* peek */
#define TPQDELIVERYQOS		0x02000		/* delivery quality of service */
#define TPQREPLYQOS			0x04000		/* reply message quality of service */
#define TPQEXPTIME_ABS		0x08000		/* absolute expiration time */
#define TPQEXPTIME_REL		0x10000		/* relative expiration time */
#define TPQEXPTIME_NONE		0x20000		/* never expire */
#define	TPQGETBYMSGID		0x40008		/* dequeue by msgid */
#define	TPQGETBYCORRID		0x80800		/* dequeue by corrid */

/**
 * Valid flags for the quality of service fileds in the TPQCTL structure
 */
#define TPQQOSDEFAULTPERSIST  0x00001   /* queue's default persistence policy */
#define TPQQOSPERSISTENT      0x00002   /* disk message */
#define TPQQOSNONPERSISTENT   0x00004   /* memory message */

/**
 * Function pointer declare
 */
typedef void (*SVCFUNC_FP)(TPSVCINFO *);
typedef int (*INITFUNC_FP)(int, char **);
typedef void (*DONEFUNC_FP)(void);

/**
 * BEGIN buildserver section
 *
 * WARNING: Modification or use of these structures in any way, may
 *          cause system failures. DO NOT USE!
 */
typedef struct tmdsptchtbl_t
{
	char *svcname;
	SVCFUNC_FP svcfunc;
} TMDSPTCHTBL_T;

typedef TMDSPTCHTBL_T TMSVCFUNC;

typedef struct tmsvrargs_t
{
	TMDSPTCHTBL_T *tmdsptchtbl;				/* Created by buildserver				*/
	int tmdsptchtbllen;
	/* INITFUNC_FP initfunc; */
	/* DONEFUNC_FP donefunc; */
	/* INITFUNC_FP thrinitfunc; */
	/* DONEFUNC_FP thrdonefunc; */
} TMSVRARGS_T;

/* END buildserver section */

#if defined(__cplusplus)
extern "C" {
#endif
	extern int*  LIBTUX_EXPORT tx_get_tperrno_addr(void);
#define tperrno (*tx_get_tperrno_addr())

	/**
	 * Buffer management
	 */
	extern char* LIBTUX_EXPORT tpalloc(char *type, char *subtype, long size);
	extern char* LIBTUX_EXPORT tprealloc(char *ptr, long size);
	extern long  LIBTUX_EXPORT tptypes(char *ptr, char *type, char *subtype);
	extern void  LIBTUX_EXPORT tpfree(char *ptr);

	/**
	 * Client membership
	 */
	extern int   LIBTUX_EXPORT tpinit(TPINIT *tpinfo);
	extern int   LIBTUX_EXPORT tpterm();

	/**
	 * Request/response communications
	 */
	extern int   LIBTUX_EXPORT tpcall(char *svc, char *idata, long ilen, char **odata, long *olen, long flags);
	extern int   LIBTUX_EXPORT tpacall(char *svc, char *data, long len, long flags);
	extern int   LIBTUX_EXPORT tpgetrply(int *cd, char **data, long *len, long flags);
	extern void  LIBTUX_EXPORT tpcancel(int cd);

	/**
	 * Conversational communication
	 */
	extern int   LIBTUX_EXPORT tpconnect(char *name, char *data, long len, long flags);
	extern int   LIBTUX_EXPORT tpdiscon(int cd);
	extern int   LIBTUX_EXPORT tpsend(int cd, char *data, long len, long flags, long *revent);
	extern int   LIBTUX_EXPORT tprecv(int cd, char **data, long *len, long flags, long *revent);

	/**
	 * Reliable queuing
	 */
	extern int   LIBTUX_EXPORT tpenqueue(char *qspace, char *qname, TPQCTL *ctl, char *data, long len, long flags);
	extern int   LIBTUX_EXPORT tpdequeue(char *qspace, char *qname, TPQCTL *ctl, char **data, long *len, long flags);

	/**
	 * For server
	 */
	extern int   LIBTUX_EXPORT tpsvrinit(int argc, char **argv);
	extern void  LIBTUX_EXPORT tpsvrdone();
	extern void  LIBTUX_EXPORT tpreturn(int rval, int rcode, char *data, long len, long flags);
	extern void  LIBTUX_EXPORT tpforward(char *svc, char *data, long len, long flags);
	extern int   LIBTUX_EXPORT tpsvrthrinit(int argc, char **argv);
	extern void  LIBTUX_EXPORT tpsvrthrdone(void);
	extern int   LIBTUX_EXPORT tpadvertise(char *svcname, void (*func)(TPSVCINFO *));
	extern int   LIBTUX_EXPORT tpunadvertise(char *svcname);

	/**
	 * Other function
	 */
	extern char* LIBTUX_EXPORT tpstrerror(int err);
	extern int   LIBTUX_EXPORT tperrordetail(long flags);
	extern char* LIBTUX_EXPORT tpstrerrordetail(int err, long flags);
	extern int   LIBTUX_EXPORT tuxputenv(char *string);
	extern char* LIBTUX_EXPORT tuxgetenv(char *name);

	/**
	 * For buildserver
	 */
	extern int   LIBTUX_EXPORT _tmstartserver(int argc, char **argv, TMSVRARGS_T* tmsvrargs);

	/**
	 * Extend for tuxone
	 */
	extern int   LIBTUX_EXPORT tpregister(const TMSVCFUNC* svcs, long len, long flags);
	extern int   LIBTUX_EXPORT tprun(long flags);

	/**
	 * For viewc32 (Internal use)
	 */
	extern int   LIBTUX_EXPORT genviewbinfile(const char* file, int* struct_size, int struct_len, int* item_offset, int item_len);

#if defined(__cplusplus)
}
#endif

#endif
