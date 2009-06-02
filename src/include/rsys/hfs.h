#if !defined(__RSYS_HFS__)
#define __RSYS_HFS__

/*
 * Copyright 1995 by Abacus Research and Development, Inc.
 * All rights reserved.
 *
 * $Id: hfs.h 87 2005-05-25 01:57:33Z ctm $
 */

#define DOSFDBIT 0x40000000	/* or this into a fd to show that we
				   need to use Mat's DOS routines */
#define ASPIFDBIT 0x20000000	/* or this into a fd to show that we
				   need to use ctm's ASPI routines */

#if !defined(USE_WINDOWS_NOT_MAC_TYPEDEFS_AND_DEFINES)

#include <rsys/cruft.h>

#include "OSUtil.h"
#include "FileMgr.h"

#include "rsys/file.h"

#if !defined(UNIX)
#include <string.h>
#endif

#include "ThinkC.h"
#define HVCB VCB



#define DEVNAMELEN	50		/* much bigger than necessary */

#define FLOPPYDREF	-5
#define OURHFSDREF	(-101)
#define PHYSBSIZE	512

#define MADROFFSET	40

typedef struct {
    unsigned short blockstart	PACKED;
    unsigned short blockcount	PACKED;
} xtntdesc, xtntrec[3];	/* WILL NEED Cx() */

typedef struct {
    unsigned short drSigWord	PACKED;	/* 0 */
    LONGINT drCrDate	PACKED;		/* 2 */
    LONGINT drLsMod	PACKED;		/* 6 */
    unsigned short drAtrb	PACKED;	/* 10 */
    unsigned short drNmFls	PACKED;	/* 12 */
    unsigned short drVBMSt	PACKED;	/* 14 */
    unsigned short drAllocPtr	PACKED;	/* 16 */
    unsigned short drNmAlBlks	PACKED;	/* 18 */
    LONGINT drAlBlkSiz	PACKED;		/* 20 */
    LONGINT drClpSiz	PACKED;
    unsigned short drAlBlSt	PACKED;
    LONGINT drNxtCNID	PACKED;
    unsigned short drFreeBks	PACKED;
    unsigned char drVN[28]	PACKED;
    LONGINT drVolBkUp	PACKED;
    unsigned short drVSeqNum	PACKED;
    LONGINT drWrCnt	PACKED;
    LONGINT drXTClpSiz	PACKED;
    LONGINT drCTClpSiz	PACKED;
    unsigned short drNmRtDirs	PACKED;
    LONGINT drFilCnt	PACKED;
    LONGINT drDirCnt	PACKED;
    LONGINT drFndrInfo[8]	PACKED;
    unsigned short drVCSize	PACKED;
    unsigned short drVCBMSize	PACKED;
    unsigned short drCtlCSize	PACKED;
    LONGINT drXTFlSize	PACKED;
    xtntrec drXTExtRec	PACKED;
    LONGINT drCTFlSize	PACKED;
    xtntrec drCTExtRec	PACKED;
} volumeinfo, *volumeinfoPtr;
typedef struct { volumeinfoPtr p PACKED_P; } HIDDEN_volumeinfoPtr;
typedef HIDDEN_volumeinfoPtr *volumeinfoHandle;

#define VOLUMEINFOBLOCKNO   2
#define InternalDrive   1
#define ROUNDUP8(x) ((x+7)/8*8)
#define NPHYSREQ(x) ((x+PHYSBSIZE-1)/PHYSBSIZE)

typedef struct {
    LONGINT ndFLink	PACKED;
    LONGINT ndBLink	PACKED;
    unsigned char ndType	PACKED;
    char ndLevel	PACKED;
    short ndNRecs	PACKED;
    unsigned short idunno	PACKED;
} btnode;

typedef enum { indexnode, mapnode = 2, leafnode = 0xFF } btnodetype;

typedef struct {
    unsigned char ckrKeyLen	PACKED;
    char ckrResrv1	PACKED;
    LONGINT ckrParID	PACKED;
    unsigned char ckrCName[32]	PACKED;
} catkey;

typedef struct {
    unsigned char xkrKeyLen	PACKED;
    unsigned char xkrFkType	PACKED;
    LONGINT xkrFNum	PACKED;
    unsigned short xkrFABN	PACKED;
} xtntkey;

typedef union {
    unsigned char keylen;
    catkey catk;
    xtntkey xtntk;
} anykey;

#define FILETYPE    2

typedef struct {
    char cdrType	PACKED;
    char cdrResrv2	PACKED;
    char filFlags	PACKED;
    char filTyp	PACKED;
    FInfo filUsrWds	PACKED;    /* not sure what form */
    LONGINT filFlNum	PACKED;
    unsigned short filStBlk	PACKED; /* I don't think this is used */
    LONGINT filLgLen	PACKED;
    LONGINT filPyLen	PACKED;
    unsigned short filRStBlk	PACKED;    /* not used? */
    LONGINT filRLgLen	PACKED;
    LONGINT filRPyLen	PACKED;
    LONGINT filCrDat	PACKED;
    LONGINT filMdDat	PACKED;
    LONGINT filBkDat	PACKED;
    LONGINT filFndrInfo[4]	PACKED;
    unsigned short filClpSize	PACKED;
    xtntrec filExtRec	PACKED;
    xtntrec filRExtRec	PACKED;
    LONGINT filResrv	PACKED;
} filerec;

#define DIRTYPE 1

typedef struct {
    char cdrType	PACKED;
    char cdrResrv2	PACKED;
    unsigned short dirFlags	PACKED;
    unsigned short dirVal	PACKED;
    LONGINT dirDirID	PACKED;
    LONGINT dirCrDat	PACKED;
    LONGINT dirMdDat	PACKED;
    LONGINT dirBkDat	PACKED;
    LONGINT dirUsrInfo[4]	PACKED;
    LONGINT dirFndrInfo[4]	PACKED;
    LONGINT dirResrv[4]	PACKED;
} directoryrec;

#define THREADTYPE  3

typedef struct {
    char cdrType	PACKED;
    char cdrResrv2	PACKED;
    char thdResrv[8]	PACKED;
    LONGINT thdParID	PACKED;
    unsigned char thdCName[32]	PACKED;
} threadrec;

typedef enum { firstisless = -1, same, firstisgreater } compretval;

typedef compretval (*compfp)(void *first, void *second);

#define WRITEBIT    (1<<0)
#define RESOURCEBIT (1<<1)
#define SHAREDBIT   (1<<4)
#define FLOCKEDBIT  (1<<5)
#define DIRTYBIT    (1<<7)

typedef struct {
    LONGINT fcbFlNum	PACKED;
    Byte fcbMdRByt	PACKED;
    Byte fcbTypByt	PACKED;
    unsigned short fcbSBlk	PACKED;
    LONGINT fcbEOF	PACKED;
    LONGINT fcbPLen	PACKED;
    LONGINT fcbCrPs	PACKED;
    HVCB *fcbVPtr	PACKED_P;
    Ptr fcbBfAdr	PACKED_P;
    unsigned short fcbFlPos	PACKED;
    LONGINT fcbClmpSize	PACKED;
    LONGINT fcbBTCBPtr	PACKED;
    xtntrec fcbExtRec	PACKED;
    LONGINT fcbFType	PACKED;
    ULONGINT fcbCatPos	PACKED;
    LONGINT fcbDirID	PACKED;
    unsigned char fcbCName[32]	PACKED;
} filecontrolblock;

enum { datafork, resourcefork = 0xFF };
typedef unsigned char forktype;

typedef enum { databusy, resourcebusy, eitherbusy } busyconcern_t;

typedef enum { reading, writing } accesstype;

#define VSOFTLOCKBIT    (1<<15)
#define VHARDLOCKBIT    (1<<7)
#define VNONEJECTABLEBIT	(1<<5)
#define FSOFTLOCKBIT    (1<<0)
#define FILEFLAGSUSERSETTABLEMASK   FSOFTLOCKBIT

typedef struct {
    LONGINT    flink	PACKED;              /* 0 */
    LONGINT    blink	PACKED;              /* 4 */
    unsigned char type	PACKED;         /* 8 */
    unsigned char dummy	PACKED;        /* 9 */
    unsigned short   hesthreejim	PACKED;        /* 10 */
    INTEGER macdisk_uses_it PACKED; /* 12 */
    INTEGER    height	PACKED;             /* 14 */
    LONGINT    root	PACKED;               /* 16 */
    LONGINT    numentries	PACKED;         /* 20 */
    ULONGINT    firstleaf	PACKED;          /* 24 */
    ULONGINT    lastleaf	PACKED;           /* 28 */
    unsigned short   btnodesize	PACKED;         /* 32 */
    unsigned short   indexkeylen	PACKED;        /* 34 */
    LONGINT    nnodes	PACKED;             /* 36 */
    LONGINT    nfreenodes	PACKED;         /* 40 */
    unsigned char reserved[72]	PACKED; /* 44 */
    unsigned char dummy2[132]	PACKED;  /* 116 */
    unsigned char map[256]	PACKED;     /* 248 */
    LONGINT unknown2[2]	PACKED;           /* 504 */
} btblock0;

#define DATAPFROMKEY(p) ((char *)(p) + ((((catkey *)p)->ckrKeyLen + 2) & ~1))


typedef enum { mfs, hfs } fstype;
typedef enum { regular = 1, directory = 2, thread = 4 } filekind;

#if defined(MAC)
#define THINKCMESSED
#endif /* defined(MAC) */

#if defined (THINKCMESSED)
#define vcbClpSiz   vcbClpSIz
#endif /* THINKCMESSED */

/*
 * TODO: do the ioCompletion routine when necessary
 */
 
#define PBRETURN(pb, x) return (((ParmBlkPtr) (pb))->ioParam.ioResult = CW(x), (x))

typedef struct _cacheentry {
    struct _cacheentry *flink	PACKED_P;
    struct _cacheentry *blink	PACKED_P;
    HVCB *vptr	PACKED_P;
    LONGINT fileno	PACKED;
    uint16 refnum	PACKED;
    ULONGINT physblock	PACKED;
    ULONGINT logblk	PACKED;
    unsigned char flags	PACKED;
    forktype forktype	PACKED;
    char buf[PHYSBSIZE]	PACKED;
} cacheentry;

#define CACHEDIRTY  (1 << 7)
#define CACHEBUSY   (1 << 6)
#define CACHEFREE   (1 << 5)

typedef struct {
    cacheentry *flink	PACKED_P;
    cacheentry *blink	PACKED_P;
    unsigned short   nitems	PACKED;
    uint16  flags	PACKED;
} cachehead;

#define NCACHEENTRIES   16

#define MAXTRAILS   8

typedef struct {
    cacheentry *cachep	PACKED_P;
    unsigned short logbno	PACKED;
    unsigned short after	PACKED;
} trailentry;

/*
 * a btblock is used to save the state as a b-tree is walked to search for
 * a node.  The saved state can then be used to do an insert, rename or delete.
 * The first cut of code had all these arguments being pushed and popped on
 * the stack as arguments.
 */
 
typedef struct {
    HVCB *vcbp;                     /* in */
    anykey tofind;                  /* in */
    compfp fp;                      /* in */
    unsigned short refnum;                   /* in */
    BOOLEAN success;                /* out */
    anykey *foundp;                 /* out */
    short leafindex;                /* out */
    trailentry trail[MAXTRAILS];    /* out */
} btparam;

typedef struct {    /* from MPW equates */
    HVCB *vcbp	PACKED_P;
    LONGINT dirid	PACKED;
    LONGINT cathint	PACKED;    /* ??? */
    LONGINT procid	PACKED;
} wdentry;

#if defined(MAC)
extern Ptr WDCBsPtr : 0x372;
extern LONGINT BufTgFNum : 0x2FC;
extern INTEGER BufTgFFlag : 0x300;
extern INTEGER BufTgFBkNum : 0x302;
extern LONGINT BufTgDate : 0x304;
extern LONGINT TFSTagData0 : 0x38A;
extern LONGINT TFSTagData1 : 0x38E;
#else

#if !defined (WDCBsPtr_H)
extern HIDDEN_Ptr WDCBsPtr_H;
extern LONGINT BufTgFNum;
extern INTEGER BufTgFFlag;
extern INTEGER BufTgFBkNum;
extern LONGINT BufTgDate;
extern LONGINT TFSTagData0;
extern LONGINT TFSTagData1;
extern INTEGER SCSIFlags;
#endif

#define WDCBsPtr	(WDCBsPtr_H.p)

#endif

#define WDMASK  0xC001
#define WDMAGIC 0x8001

#define ISWDNUM(v)						\
({								\
  uint16 _v;							\
								\
  _v = (v);							\
  (_v & WDMASK) == WDMAGIC ?					\
    (_v ^ WDMAGIC) % sizeof (wdentry) == sizeof (INTEGER)	\
  :								\
    FALSE;							\
})

#define WDNUMTOWDP(v) ((wdentry *)(MR(WDCBsPtr) + (INTEGER) ((v) ^ WDMAGIC)))
#define WDPTOWDNUM(p)   (((char *) (p) - (char *)MR(WDCBsPtr)) ^ WDMAGIC)

typedef enum { seteof, allocany, alloccontig } alloctype;

extern compretval ROMlib_catcompare(void *firstp, void *secondp);

typedef enum { GETCACHESAVE = 1, GETCACHENOREAD = 2 } cacheflagtype;

#define VCBDIRTY    (1 << 15)

typedef enum {NOTE, CAUTION, STOP} alerttype;

enum { INHERITED_FLAG_BITS = ATTRIB_ISLOCKED }; 

#if !defined(MAC)
#define CurTime	(GetDateTime(&Time), Cx(Time))
#else
#define CurTime Cx(Time)
#endif

#define EJECTALERTID	(-4061)

/* public entities in btree.c */

extern cacheentry *ROMlib_addrtocachep(Ptr addr, HVCB *vcbp);
extern BOOLEAN ROMlib_searchnode(btnode *btp, void *key, compfp fp, anykey **keypp,
							       INTEGER *afterp);
extern OSErr ROMlib_getcache(cacheentry **retpp, uint16 refnum, ULONGINT logbno,
							   cacheflagtype flags);
extern OSErr ROMlib_putcache(cacheentry *cachep);
extern OSErr ROMlib_keyfind(btparam *btpb);
extern OSErr ROMlib_btnext(anykey **nextpp, anykey *keyp, HVCB *vcbp);
extern OSErr ROMlib_btdelete(btparam *btpb);
extern OSErr ROMlib_makecatparam(btparam *btpb, HVCB *vcbp, LONGINT dirid,
						    INTEGER namelen, Ptr namep);
extern OSErr ROMlib_errortype(btparam *btpb);
extern OSErr ROMlib_filedelete(btparam *btpb, filekind kind);
extern OSErr ROMlib_dirdelete(btparam *btpb);
extern OSErr ROMlib_dirtyleaf(void *p, HVCB *vcbp);
extern OSErr ROMlib_filecreate(btparam *btpb, void *data, filekind kind);
extern OSErr ROMlib_dircreate(btparam *btpb, directoryrec *data);
extern OSErr ROMlib_dircreate(btparam *btpb, directoryrec *data);
extern xtntkey *ROMlib_newextentrecord(filecontrolblock *fcbp, uint16 newabn);
extern OSErr ROMlib_btrename(btparam *btpb, StringPtr newnamep);
extern OSErr ROMlib_btcreateemptyfile(btparam *btpb);
extern OSErr ROMlib_btcreateemptydir(btparam *btpb, LONGINT *newidp);
extern OSErr ROMlib_btpbindex (ioParam *pb, LONGINT dirid, HVCB **vcbpp,
			  filerec **frpp, catkey **catkeypp, BOOLEAN onlyfiles);
extern OSErr ROMlib_cleancache(HVCB *vcbp);
extern OSErr ROMlib_flushcachevcbp(HVCB *vcbp);

/* public entries in changing.c */

extern INTEGER ROMlib_flnumtorefnum(ULONGINT flnum, VCB *vcbp);

/* public entries in file.c */

extern filecontrolblock *ROMlib_getfreefcbp( void );
extern filecontrolblock *ROMlib_refnumtofcbp(uint16 refnum);
extern compretval ROMlib_xtntcompare(void *firstp, void *secondp);
extern compretval ROMlib_catcompare(void *firstp, void *secondp);
extern void ROMlib_makextntkey(xtntkey *keyp, forktype forkwanted, LONGINT flnum,
								    uint16 bno);
extern void ROMlib_makextntparam(btparam *btpb, HVCB *vcbp, forktype forkwanted,
						     LONGINT flnum, uint16 bno);
extern LONGINT ROMlib_logtophys(filecontrolblock *fcbp, LONGINT absoffset,
							   LONGINT *nphyscontigp);
extern OSErr ROMlib_makecatkey(catkey *keyp, LONGINT dirid, INTEGER namelen, Ptr namep);
extern OSErr ROMlib_findvcbandfile(ioParam *pb, LONGINT dirid, btparam *btpb,
					   filekind *kindp, BOOLEAN ignorename);
extern OSErr ROMlib_alreadyopen(HVCB *vcbp, LONGINT flnum, SignedByte *permp,
					 INTEGER *refnump, busyconcern_t busy);
extern OSErr ROMlib_allochelper(ioParam *pb, BOOLEAN async, alloctype alloc,
						     BOOLEAN ROMlib_writefcbp);

/* public entries in helper.c */

extern void OurExit( void );
extern OSErr ROMlib_transphysblk(hfs_access_t *hfsp, LONGINT physblock,
				 short nphysblocks,
				 Ptr bufp, accesstype rw, LONGINT *actp);
extern char *ROMlib_indexn(char *str, char tofind, INTEGER length);
#if defined(MAC)
extern void str255assign(StringPtr dstp, StringPtr srcp);
#endif
extern void *ROMlib_indexqueue(QHdr *qp, short index);
extern OSErr ROMlib_writefcbp(filecontrolblock *fcbp);
extern OSErr ROMlib_writevcbp(HVCB *vcbp);
extern void ROMlib_openfloppy( const char *dname, LONGINT *messp );
extern void ROMlib_openharddisk( const char *dname, LONGINT *messp );
extern void ROMlib_hfsinit( void );
extern OSErr ROMlib_ejectfloppy( LONGINT floppyfd );
extern OSErr ROMlib_readwrite( LONGINT fd, char *buffer, LONGINT count, LONGINT off,
			     accesstype rw, LONGINT blocksize, LONGINT maxtransfer );

extern LONGINT ROMlib_sock;

/* public entries in volume.c */

extern HVCB *ROMlib_findvcb(short vrefnum, StringPtr name, LONGINT *diridp,
							   BOOLEAN usedefault);
extern OSErr ROMlib_mkwd(WDPBPtr pb, HVCB *vcbp, LONGINT dirid,
							        LONGINT procid);
extern OSErr ROMlib_pbvolrename(ioParam *pb, StringPtr newnamep);
extern OSErr ROMlib_flushvcbp(HVCB *vcbp);
extern HVCB *ROMlib_vcbbyvrn(short vrefnum);
extern VCBExtra *ROMlib_vcbbyunixname(char *uname);
extern HVCB *ROMlib_vcbbybiggestunixname(const char *uname);
extern HVCB *ROMlib_vcbbydrive(short vrefnum);

extern DrvQExtra *ROMlib_dqbydrive (short vrefnum);

/* public entries in workingdir.c */

extern OSErr ROMlib_dirbusy(LONGINT dirid, HVCB *vcbp);
extern void ROMlib_adjustdirid(LONGINT *diridp, HVCB *vcbp, INTEGER vrefnum);

#if defined(MAC)
/* public entries in misc.c */
extern void bcopy(void *srcp, void *dstp, LONGINT length);
extern void bzero(void *dstp, LONGINT ntozero);
#endif

/* public entry in error.c */
extern void errormessage(StringPtr msg, alerttype severity);

enum { NEWLINEMODE = 1 << 7 };

#if !defined (__STDC__)
extern OSErrRET hfsPBOpen(); 
extern OSErrRET hfsPBHOpen(); 
extern OSErrRET hfsPBOpenRF(); 
extern OSErrRET hfsPBHOpenRF();
extern OSErrRET hfsPBLockRange();
extern OSErrRET hfsPBUnlockRange();
extern OSErrRET hfsPBRead(); 
extern OSErrRET hfsPBWrite(); 
extern OSErrRET hfsPBGetFPos();
extern OSErrRET hfsPBSetFPos();
extern OSErrRET hfsPBGetEOF(); 
extern OSErrRET hfsPBSetEOF(); 
extern OSErrRET hfsPBAllocate();
extern OSErrRET hfsPBAllocContig();
extern OSErrRET hfsPBFlushFile();
extern OSErrRET hfsPBClose(); 
extern OSErrRET hfsPBCreate(); 
extern OSErrRET hfsPBHCreate();
extern OSErrRET hfsPBDirCreate();
extern OSErrRET hfsPBDelete(); 
extern OSErrRET hfsPBHDelete();
extern OSErrRET hfsPBGetCatInfo();
extern OSErrRET hfsPBSetCatInfo();
extern OSErrRET hfsPBCatMove();
extern OSErrRET hfsPBOpenWD(); 
extern OSErrRET hfsPBCloseWD(); 
extern OSErrRET hfsPBGetWDInfo(); 
extern OSErrRET hfsPBGetFInfo();
extern OSErrRET hfsPBHGetFInfo();
extern OSErrRET hfsPBSetFInfo();
extern OSErrRET hfsPBHSetFInfo();
extern OSErrRET hfsPBSetFLock();
extern OSErrRET hfsPBHSetFLock();
extern OSErrRET hfsPBRstFLock();
extern OSErrRET hfsPBHRstFLock();
extern OSErrRET hfsPBSetFVers();
extern OSErrRET hfsPBRename();
extern OSErrRET hfsPBHRename();
extern OSErrRET hfsPBGetFCBInfo();
extern OSErr hfsPBMountVol(); 
extern OSErrRET hfsPBGetVInfo();
extern OSErrRET hfsPBHGetVInfo();
extern OSErrRET hfsPBSetVInfo();
extern OSErrRET hfsPBGetVol(); 
extern OSErrRET hfsPBHGetVol(); 
extern OSErrRET hfsPBSetVol(); 
extern OSErrRET hfsPBHSetVol(); 
extern OSErrRET hfsPBFlushVol();
extern OSErrRET hfsPBUnmountVol(); 
extern OSErrRET hfsPBOffLine(); 
extern OSErrRET hfsPBEject(); 
#else /* __STDC__ */
extern OSErr hfsPBOpen( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBHOpen( HParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBOpenRF( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBHOpenRF( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBLockRange( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBUnlockRange( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBRead( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBWrite( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBGetFPos( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBSetFPos( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBGetEOF( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBSetEOF( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBAllocate( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBAllocContig( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBFlushFile( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBClose( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBCreate( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBHCreate( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBDirCreate( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBDelete( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBHDelete( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBGetCatInfo( CInfoPBPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBSetCatInfo( CInfoPBPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBCatMove( CMovePBPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBOpenWD( WDPBPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBCloseWD( WDPBPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBGetWDInfo( WDPBPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBGetFInfo( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBHGetFInfo( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBSetFInfo( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBHSetFInfo( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBSetFLock( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBHSetFLock( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBRstFLock( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBHRstFLock( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBSetFVers( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBRename( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBHRename( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBGetFCBInfo( FCBPBPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBMountVol( ParmBlkPtr ufsPB, LONGINT floppyfd, LONGINT off,
			    LONGINT bsize, LONGINT maxbytes,
			   drive_flags_t flags, DrvQExtra *dqp );
extern OSErr hfsPBGetVInfo( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBHGetVInfo( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBSetVInfo( HParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBGetVol( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBHGetVol( WDPBPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBSetVol( ParmBlkPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBHSetVol( WDPBPtr ufsPB, BOOLEAN a ); 
extern OSErr hfsPBFlushVol( ParmBlkPtr ufsPB, BOOLEAN a );
extern OSErr hfsPBUnmountVol( ParmBlkPtr ufsPB ); 
extern OSErr hfsPBOffLine( ParmBlkPtr ufsPB ); 
extern OSErr hfsPBEject( ParmBlkPtr ufsPB ); 

extern void try_to_mount_disk( const char *dname, LONGINT floppyfd,
			       LONGINT *messp, LONGINT bsize,
			       LONGINT maxbytes, drive_flags_t flags,
			       uint32 offset );

extern void ROMlib_OurClose( void );

extern long ROMlib_priv_open(const char *filename, long mode);
extern void vcbsync(HVCB *vcbp);

#endif /* __STDC__ */

#endif

#endif /* !defined(__RSYS_HFS__) */