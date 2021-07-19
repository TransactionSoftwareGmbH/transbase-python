/******************************************************** 

                      T r a n s B a s e / C D
  @(#) tci.h:   V8.4.1.2.306
		Project: 4.7787
		2021/03/11 16:56:50

  Copyright (c) 1995 - 
  Transaction Software GmbH
  D 81739 Munich 

 *********************************************************/

/* Module begin */
#ifndef _TCI_H
#define _TCI_H

#include <stdlib.h>
#if defined(_TCI_COMPAT_)
#include "tcicompat.h"
#endif
#if defined(_TCI_EXTENSIONS_)
#include "tciext.h"
#endif
#if defined(__cplusplus)
extern "C"
{
#endif

#ifndef _BASICS_H
#define _BASICS_H

#if defined(_WIN32)

#define TBADM_API __stdcall
#define TCI_API __stdcall
#define TBX_API __cdecl

#else

#define __int64	long long
#define __int32	int
#define __int16	short
#define __int8	char
#define TBADM_API
#define TCI_API
#define TBX_API

#endif


#if !defined(TRUE)
#define TRUE (1)
#endif

#if !defined(FALSE)
#define FALSE (0)
#endif
typedef   signed __int8     Int1;
typedef unsigned __int8    Uint1;
typedef   signed __int16    Int2;
typedef unsigned __int16   Uint2;
typedef   signed __int32    Int4;
typedef unsigned __int32   Uint4;
typedef   signed __int64    Int8;
typedef unsigned __int64   Uint8;

#endif

#ifndef DB_H
#define DB_H

#define MAXTUPLESIZE    (32768-96) /* 32K - 96, so tuple and page header fit in 32K page */

#define PARAMMODE_UNKNOWN	'U'	/* Unknown */
#define PARAMMODE_IN		'I'	/* In */
#define PARAMMODE_OUT		'O'	/* Out */
#define PARAMMODE_INOUT		'B'	/* Both */

#define MAXSTRINGSIZE   MAXTUPLESIZE
#define MAXATTRSIZE     MAXSTRINGSIZE
/* _tbadmlib_h_include_begin_ */

#define MAXIDENTSIZE	127

/* session- and transaction-control */
#define UNDEF_STATE		(1<<0)
#define TA_UNDEF		UNDEF_STATE
#define TA_NOT_ACTIVE		(1<<1)
#define TA_ACTIVE		(1<<2)
#define TA_PREP			(1<<3)
#define TA_COMMITTED		(1<<4)
#define TA_ABORTED		(1<<5)

typedef  int	           TBErrorCode;

typedef	 Int8	       TBBigint;
typedef	 Int2	       TBSmallint;
typedef	 Int1	       TBTinyint;
typedef  float	       TBFloat;
typedef  char          TBIdentifier[MAXSTRINGSIZE];
typedef  double        TBDouble;
typedef  double        TBReal;
/* _tbadmlib_h_include_begin_ */

typedef	 Int4	       TBInteger;
typedef  char	       TBBool;


#define MAXPREC 30
typedef  struct
{  
   char bcd[(MAXPREC+1)/2 + 2];    /* contains prec,scale, then 
				    BCD-digits in reversed order */
}  TBNumeric;

			/* Vorzeichen, Dez.pkt., N, Exponent mit VZ (4), EOS */
#define MAXNUMSTRING 	(MAXPREC+8)
#define MAXFIX		(MAXNUMSTRING)

/**  type DATETIME/TIMESPAN  **/

#define    MAXQUAL 	7	/* 7 elements in [MS:YY]  */
typedef struct
{
   Int4 qual;			/* sign: PLUSS, MINUSS */
   short val[MAXQUAL];  /* array of datetime field short VALues */
}  TBDatetime;		   /* order from low to high, from the least */
			   /* significant to the most significant */
typedef struct
{
   Int4 qual;			/* sign: PLUSS, MINUSS */
   Int4 val[MAXQUAL];	/* Timespan field Long4 VALues */
}  TBTimespan; 			/* order from the least */

/**  type BINCHAR  **/

typedef Int4  Bincharlen;
#define MAXBINCHAR 	(MAXATTRSIZE-(int)sizeof(Bincharlen))
typedef struct
{
   Bincharlen length;		/* length of array: no \0  */
   char       binchar[MAXBINCHAR];
}  TBBinchar;

#define BINCHARARR(b)	( ((Binchar *)(b))->binchar)
#define BINCHARLEN(b)	( ((Binchar *)(b))->length)
#define ASCIMAXBINCHAR 	(MAXBINCHAR*2+3)


/**  type BITSS  **/

typedef Int4  Bitslen;
typedef Uint2  Bits2len;

/* internal size of Bits array */ 
#define MAXBITS 	(4096-(int)sizeof(Bitslen))
/* such that number of bits fits in the 2-byte length field of type Bits2 (Ub_value) */
#define MAXUSERBITS 	(MAXBITS * 8)

typedef struct
{
   Bitslen       length;		/* number of bits, not bytes */
   unsigned char bits[MAXBITS];
}  TBBits;
#define BITS_TO_BYTE(b)	(((b)+7)/8)		/* b bits in bytes */

#define BITSARR(b)	( ( ((Bits *)(b))->bits))
#define BITSLEN(b)	( ((Bits *)(b))->length)
#define BITSLEN_BYTE(b)	(BITS_TO_BYTE(BITSLEN(b))) /* used area bytes */

#define BINCHARLEN_TOT(b)	(BINCHARLEN(b)+(int)sizeof(Bincharlen))
#define BITSLEN_TOT(b)		(BITSLEN_BYTE(b)+(int)sizeof(Bitslen))

/* for UBTREE_VARIANT only */
/****  type Bits2 used as Ub_value   **/
/* is like Bits, but with short as length indicator */

typedef struct
{
   Bits2len       length;		/* number of bits, not bytes */
   unsigned char bits[MAXBITS];
}  TBBits2;
#define BITS2ARR(bs)	( ( ((Bits2 *)(bs))->bits))
#define BITS2LEN(bs)	( ((Bits2 *)(bs))->length)
#define BITS2LEN_BYTE(bs)	(BITS_TO_BYTE(BITS2LEN(bs))) /* used area bytes */
#define BITS2LEN_TOT(bs)  ((char*)&(BITS2ARR(bs)[BITS2LEN_BYTE(bs)]) - (char*)(bs))


typedef struct {
    int             mode;  /* MMADR or FILENAME */
    Uint4 size;
    union {
            char   *filename;
            struct {
                     short     usrmalloc;  /* 0 or 1 */
                     unsigned  mallocsize;
                     char *mmadr;    /* main memory address  **/
                   } mmem;
           } loc;
	short			codepage;
	char			locale[MAXIDENTSIZE+1];
} TBBlobdesc;
typedef TBBlobdesc TBClobdesc;

typedef Int4 TBTimeout;


#endif

#if !defined(_TBCOMM_H_)
#define _TBCOMM_H

#define DB_DCONN		1
#define DB_CONN			2
#define DB_KILLED		3
#define DB_LOGGED		4
#define DB_CONTACTED		5

#define QU_ACTIVE		1
#define QU_NOT_ACTIVE		2
#define QU_CANCELLING		3

#endif

#ifndef _RELMAN_H
#define _RELMAN_H

#define QT_DROPTABLE	1 
#define QT_DROPINDEX	2 
#define QT_CREATETABLE	3 
#define QT_CREATEINDEX	4
#define QT_CREATEVIEW	5
#define QT_DROPVIEW	8
#define QT_DROPDOMAIN	6
#define QT_CREATEDOMAIN	7
#define QT_GRANT_ACCESS		9
#define QT_GRANT_PRIVILEGE		10
#define QT_REVOKE_ACCESS		11
#define QT_REVOKE_PRIVILEGE 	12

#define QT_PASSWORD		14
#define QT_FOREIGN_PASSWORD	15

#define QT_DDLCL		QT_FOREIGN_PASSWORD

#define QT_ALTERTABLE_ADDCOL	110	/* nicht einsortiert  ddl_class */
#define QT_ALTERTABLE_DROPCOL	111
#define QT_ALTERTABLE_SETDEFAULT 	112
#define QT_ALTERTABLE_DROPDEFAULT 	113
#define QT_ALTERTABLE_ADDCONSTR	114
#define QT_ALTERTABLE_DROPCONSTR	115
#define QT_ALTERDOMAIN_SETDEFAULT 	116
#define QT_ALTERDOMAIN_DROPDEFAULT	117
#define QT_ALTERDOMAIN_ADDCONSTR	118
#define QT_ALTERDOMAIN_DROPCONSTR	119
#define QT_CREATESEQUENCE			120 
#define QT_DROPSEQUENCE			121 

#define QT_CREATEEXTERNAL			122	/* first in macro load_external */
#define QT_CREATEEXTSRC			123
#define QT_ALTEREXTERNAL			124
#define QT_ALTEREXTSRC			125	/* last in macro load_external */
#define QT_CREATEFUNCTION			126
#define QT_CREATEPROCEDURE			127
#define QT_CREATETRIGGER			128
#define QT_ALTERFUNCTION			129
#define QT_ALTERPROCEDURE			130
#define QT_CREATEORREPLACE			131			
#define QT_DROPEXTERNAL			132
#define QT_DROPPROCEDURE			133
#define QT_DROPPSMPROCEDURE			134
#define QT_DROPFUNCTION			135
#define QT_DROPPSMFUNCTION			136
#define QT_DROPTRIGGER			137
#define QT_CHECKTRIGGER			138
#define QT_GRANTEXTERNAL			139
#define QT_REVOKEEXTERNAL			140
#define QT_CALL_TYPE			141	/* do not change this! has to be consistent with JDBC ... */
#define QT_GRANTPROCFUNC			142
#define QT_GRANTPSMPROCFUNC		143
#define QT_REVOKEPROCFUNC			144
#define QT_REVOKEPSMPROCFUNC		145

#define QT_ALTERTABLE_MOVE		150
#define QT_ALTERTABLE_RENAME	152
#define QT_ALTERTABLE_RENAMECOL	153
#define QT_ALTERTABLE_INMEM_NONE	154
#define QT_ALTERTABLE_INMEM	155
#define QT_ALTERTABLE_INMEMPRELOAD	156

#define QT_ALTERTABLE_MODCOL	160
#define QT_ALTERDATASPACE		161
#define QT_CREATEDATASPACE		162  
#define QT_CREATEMATVIEW		163  
#define QT_DROPMATVIEW		164  
#define QT_DDLCL2		QT_DROPMATVIEW  

#define QT_CREATEDATABASE		165		/* macro dal_class */
#define QT_DROPDATABASE		166
#define QT_LINKDATABASE		167
#define QT_UNLINKDATABASE		168
#define QT_DROPGRID		169
#define QT_ALTERGRID		170
#define QT_ALTERGRIDADDDB		171
#define QT_ALTERGRIDDROPDB		172
#define QT_BOOTDATABASE		173
#define QT_SHUTDOWNDATABASE	174
#define QT_MIGRATEDATABASE		175
#define QT_DUMPDATABASE		176
#define QT_RESTOREDATABASE		177
#define QT_ALTERDATABASE		178
#define QT_REPLCREATEDATABASE	180
#define QT_REPLUPDATEONCE		181
#define QT_REPLUPDATEASYNC		182
#define QT_REPLUPDATENONE		183
#define QT_ATTACHROMFILES		184
#define QT_PUBLISHDATABASE		185
#define QT_CREATEGRID		188
#define QT_REPLUPDATESYNC		189
#define QT_REPLUPDATESEMISYNC	190
#define QT_CROWDCONNECT		191
#define QT_CROWDDISCONNECT		192
#define QT_FLUSHDATABASE		193
#define QT_IMPORTDATABASE		194
#define QT_EXPORTDATABASE		195
#define QT_ALTERMONITOR		196
#define QT_DALCL	QT_ALTERMONITOR		/* macro dal_class */

			/* attention to macro "ddl_class" */

#define QT_SPOOL_FILE	21
#define QT_SPOOL_RELATION	22

#define QT_SPCL		QT_SPOOL_RELATION


#define QT_UPD_POS_TYPE	29	
#define QT_DEL_POS_TYPE	30

#define QT_POSICL		QT_DEL_POS_TYPE


#define QT_INS_TYPE	31
#define QT_DEL_TYPE	32
#define QT_UPD_TYPE	33
#define QT_MERGE_TYPE	34
#define QT_PSM_TYPE	35
#define QT_DMLCL		QT_PSM_TYPE


#define QT_SEL_TYPE	41
#define QT_SEL_FOR_UPD	42

#define QT_MQLCL		QT_SEL_FOR_UPD

#define QT_LOCK		51
#define QT_UNLOCK		52

#define QT_LCKCL		QT_UNLOCK

#define QT_LOAD_TABLE	60  /* ROM */
#define QT_LOAD_INDEX	61
#define QT_LOAD_DEFAULT	62
#define QT_LOAD_BY_QUERY	63
#define QT_LOAD_SWITCH_ON	64
#define QT_LOAD_SWITCH_OFF	65
#define QT_UNLOAD_TABLE	66
#define QT_UNLOAD_INDEX	67
#define QT_UNLOAD_ALL	68
#define QT_LOAD_TABLE_PRELOAD	69

#define QT_LOADCL		QT_UNLOAD_ALL

#define QT_TBMODE_TYPE	80
#define QT_TBMODECL	TBMODE_TYPE

#define QT_GENTREE_TYPE	90
#define QT_GENTREECL	QT_GENTREE_TYPE
#define QT_PROPERTY_TYPE	100
#define QT_PROPERTYCL	QT_PROPERTY_TYPE

#define QT_GETSORT_TYPE		(200)
#define QT_SETSORT_TYPE		(201)
#define QT_GETPLAN_TYPE	(202)

#define QT_UPD_RETURNING_TYPE	220
#define QT_DEL_RETURNING_TYPE	221
#define QT_INS_RETURNING_TYPE	222
#define QT_DML_RETURNING_CL	QT_INS_RETURNING_TYPE
/* tciccl_incl_const_end */

#define dml_returning_class(type) (((type) >= QT_UPD_RETURNING_TYPE && (type) <= QT_DML_RETURNING_CL))
#define gentree_class(type)	(((type) >= QT_GENTREE_TYPE && (type) <= QT_GENTREECL))
#define ddl_class(type)		((((type) >= QT_DROPTABLE && (type) <= QT_DDLCL) || \
							((type) >= QT_ALTERTABLE_ADDCOL && (type) <= QT_DDLCL2)) && \
							(type) != QT_CALL_TYPE)
#define dml_class(type)		((type) >= QT_INS_TYPE && (type) <= QT_DMLCL)
#define dal_class(type)		((type) >= QT_CREATEDATABASE && (type) <= QT_DALCL)	/* Administration */

#define spool_class(type) 	((type) >=QT_SPOOL_FILE && (type) <= QT_SPCL)
#define upd_class(type)		(dml_class(type))
#define sel_class(type)		(((type) >=QT_SEL_TYPE && (type) <= QT_MQLCL) || (type)==QT_CALL_TYPE)
#define sel_insret_class(type)	(sel_class(type)||((type)==QT_INS_RETURNING_TYPE))
#define lock_class(type)	((type) >=QT_LOCK && (type) <= QT_LCKCL)
#define load_class(type)	((type) >=QT_LOAD_TABLE && (type) <= QT_LOADCL)
#define posi_class(type)	((type) >=QT_UPD_POS_TYPE && (type) <= QT_POSICL)
#define tbmode_class(type)	((type) >=QT_TBMODE_TYPE && (type) <= QT_TBMODECL)
#define property_class(type)	((type) >=QT_PROPERTY_TYPE && (type) <= QT_PROPERTYCL)
#define cursor_class(type)	((gentree_class(type))||(sel_class(type)))


#define is_select(type)	(sel_class(type))

#define ill_class(type) (!sel_class(type) && \
			 !ddl_class(type) && \
			 !spool_class(type) && \
			 !lock_class(type) && \
			 !load_class(type) && \
			 !upd_class(type)   && \
			 !posi_class(type))

#endif
#ifndef _TCIBASIC_H
#define _TCIBASIC_H
typedef Uint2 TCIColumnnumber;
typedef Uint2 TCIAttribute;
typedef Uint2 TCIDatatype;
typedef Uint2 TCIQuerytype;
typedef Uint1 TCIConsistency;
typedef Uint1 TCIParametermode;
typedef int   TCIState;


/* structs */

/* C data types */

/* TCINumeric: max. representable decimal: 340282366920938463463374607431768211455n127 */
#define TCI_MAX_NUMERIC_LEN 16
typedef struct {
    unsigned char   precision;
    signed   char   scale;
    unsigned char   sign;	/* 1 if positive, 0 if negative */
    unsigned char   val[TCI_MAX_NUMERIC_LEN];
} TCINumeric;

typedef struct {
    short year;
    unsigned short month;
    unsigned short day;
} TCIDate;

typedef struct {
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
} TCITime;

typedef struct {
    short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned int   fraction;
} TCITimestamp;

typedef enum {
    TCI_IS_YEAR				=  1,
    TCI_IS_MONTH			=  2,
    TCI_IS_DAY				=  3,
    TCI_IS_HOUR				=  4,
    TCI_IS_MINUTE			=  5,
    TCI_IS_SECOND_TO_MILLISECOND	=  6,
    TCI_IS_YEAR_TO_MONTH		=  7,
    TCI_IS_DAY_TO_HOUR			=  8,
    TCI_IS_DAY_TO_MINUTE		=  9,
    TCI_IS_DAY_TO_MILLISECOND		= 10,
    TCI_IS_HOUR_TO_MINUTE		= 11,
    TCI_IS_HOUR_TO_MILLISECOND		= 12,
    TCI_IS_MINUTE_TO_MILLISECOND	= 13,
    TCI_IS_DAY_TO_SECOND		= 14,
    TCI_IS_HOUR_TO_SECOND		= 15,
    TCI_IS_MINUTE_TO_SECOND		= 16,
    TCI_IS_SECOND			= 17,
    TCI_IS_MILLISECOND			= 18
} TCIIntervalType;

typedef struct {
    unsigned int year;
    unsigned int month;
} TCIYearMonth;

typedef struct {
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int fraction;
} TCIDaySecond;

typedef struct {
    TCIIntervalType interval_type;
    short interval_sign;
	short filler;
    union {
	TCIYearMonth  year_month;
	TCIDaySecond  day_second;
    } intval;
} TCIInterval;

/* version struct */
typedef struct _TCIVersion {
    Int4 major_version;
    Int4 minor_version;
    Int4 release;
    Int4 patch;
    Int4 build;
    TCIDate date;
} TCIVersion;

/* data type codes */

#define	TCI_TYPE		(0x0100)	/* TCI types starting at 256 */
#define TCI_TYPE_UNSIGNED	(0x0200)	/* flag for unsigned types */
#define TCI_TYPE_DATE		(0x0400)	/* flag for date type */
#define TCI_TYPE_INTERVAL	(0x0800)	/* flag for interval type */
#define TCI_TYPE_SPECIAL	(0x1000)	/* has no Transbase corresponding type */
#define TCI_TYPE_WIDE		(0x2000)	/* is wide char type */
#define TCI_TYPE_SQL		(0x4000)	/* is a SQL type */
#define TCI_TYPE_UNUSED		(0x8000)	/* not used for the moment */

#define IS_TCI_TYPE(t)		(((t) & 0xFFFF) & TCI_TYPE)			/* t is a TCI type */
#define IS_TCI_UNSIGNED(t)	(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_UNSIGNED))	/* t is a TCI unsigned type */
#define IS_TCI_DATE(t)		(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_DATE))	/* t is a TCI date type, low/high in lower byte */
#define IS_TCI_INTERVAL(t)	(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_INTERVAL))	/* t is a TCI interval type, low/high in lower byte */
#define IS_TCI_SPECIAL(t)	(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_SPECIAL))	/* t is a TCI ODBC-like type */
#define IS_TCI_WIDE(t)		(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_WIDE))	/* t is a TCI wide char type */
#define IS_TCI_SQL(t)		(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_SQL))	/* t is a TCI SQL type */
#define IS_TCI_UNUSED(t)	(IS_TCI_TYPE(t) && ((t) & TCI_TYPE_UNUSED))	/* t is a TCI unused type */

/* classification of types */
#define IS_TCI_NATURAL(t)	((t)==TCI_C_SINT1 || (t)==TCI_C_SINT2 || (t)==TCI_C_SINT4 || (t)==TCI_C_SINT8 || \
				 (t)==TCI_C_UINT1 || (t)==TCI_C_UINT2 || (t)==TCI_C_UINT4 || (t)==TCI_C_UINT8)
#define IS_TCI_APPROX(t)	((t)==TCI_C_FLOAT || (t)==TCI_C_DOUBLE)
#define IS_TCI_ARI(t)		(IS_TCI_NATURAL(t) || IS_TCI_APPROX(t))
#define IS_TCI_CHAR(t)		((t)==TCI_C_SCHAR || (t)==TCI_C_WCHAR)
#define IS_TCI_FILE(t)		((t)==TCI_C_SFILE || (t)==TCI_C_WFILE)
#define IS_TCI_BINARY(t)	(IS_TCI_FILE(t) || (t)==TCI_C_BYTE)

#define TCI_MILLISECOND		(0x01)
#define TCI_SECOND		(0x02)
#define TCI_MINUTE		(0x03)
#define TCI_HOUR		(0x04)
#define TCI_DAY			(0x05)
#define TCI_MONTH		(0x06)
#define TCI_YEAR		(0x07)

#define TCI_MAKE_HL(h,l)	(((h) << 4) | (l))
#define TCI_GET_HL_H(hl)	(((hl) & 0x00f0) >> 4)
#define TCI_GET_HL_L(hl)	((hl)  & 0x000f)

#define	TCI_C_INT1			(TCI_TYPE | 0x01)			/* signed char */
#define	TCI_C_INT2			(TCI_TYPE | 0x02)			/* signed short */
#define	TCI_C_INT4			(TCI_TYPE | 0x03)			/* signed int */
#define	TCI_C_FLOAT			(TCI_TYPE | 0x05)			/* float */
#define	TCI_C_DOUBLE			(TCI_TYPE | 0x06)			/* double */

#define	TCI_C_CHAR			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x0a)	/* char * */
#define	TCI_C_INT8			(TCI_TYPE |                    0x0b)	/* signed int8 */
#define	TCI_C_BYTE			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x0c)	/* unsigned char * */
#define	TCI_C_FILE			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x0d)	/* char * */
#define	TCI_C_CHARPTR			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x0e)	/* char ** */
#define	TCI_C_BYTEPTR			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x0f)	/* unsigned char ** */
#define	TCI_C_FILEPTR			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x10)	/* char ** */
#define	TCI_C_CFILE			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x11)	/* char * */
#define	TCI_C_CFILEPTR			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x12)	/* char ** */

#define TCI_C_TCINUMERIC		(TCI_TYPE | TCI_TYPE_SPECIAL | 0x07)	/* Transbase implementation of SQL_NUMERIC_STRUCT (ODBC) */

#define TCI_C_TBNATIVE			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x04)	/* Transbase native type */
#define	TCI_C_TBNUMERIC			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x13)	/* Numeric */
#define	TCI_C_TBBLOB			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x14)	/* Blob */
#define	TCI_C_TBCLOB			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x15)	/* Clob */
#define	TCI_C_TBBINCHAR			(TCI_TYPE | TCI_TYPE_SPECIAL | 0x16)	/* Binchar */
#define	TCI_C_TBBITSLONG		(TCI_TYPE | TCI_TYPE_SPECIAL | 0x17)	/* Bits */
#define	TCI_C_TBBITSHORT		(TCI_TYPE | TCI_TYPE_SPECIAL | 0x18)	/* Bits2 */
#define	TCI_C_TBDATETIME		(TCI_TYPE | TCI_TYPE_SPECIAL | 0x19)	/* Datetime; no lower byte: reserved for high/low field */
#define	TCI_C_TBTIMESPAN		(TCI_TYPE | TCI_TYPE_SPECIAL | 0x1a)	/* Timespan; no lower byte: reserved for high/low field */

#define	TCI_C_TYPE_DATE			(TCI_TYPE | TCI_TYPE_SPECIAL | TCI_TYPE_DATE)

#define	TCI_C_DATE_YEAR			(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_YEAR))			/* TCIDate */
#define	TCI_C_DATE_YEAR_TO_MONTH	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_MONTH))			/* TCIDate */
#define	TCI_C_DATE_YEAR_TO_DAY		(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_DAY))			/* TCIDate */
#define	TCI_C_DATE_MONTH		(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_MONTH))			/* TCIDate */
#define	TCI_C_DATE_MONTH_TO_DAY		(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_DAY))			/* TCIDate */
#define	TCI_C_DATE_DAY			(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_DAY))			/* TCIDate */
#define	TCI_C_DATE			(TCI_C_DATE_YEAR_TO_DAY)												/* TCIDate */

#define	TCI_C_TIME_HOUR			(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_HOUR))			/* TCITime */
#define	TCI_C_TIME_HOUR_TO_MINUTE	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_MINUTE))			/* TCITime */
#define	TCI_C_TIME_HOUR_TO_SECOND	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_SECOND))			/* TCITime */
#define	TCI_C_TIME_MINUTE		(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MINUTE, TCI_MINUTE))			/* TCITime */
#define	TCI_C_TIME_MINUTE_TO_SECOND	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MINUTE, TCI_SECOND))			/* TCITime */
#define	TCI_C_TIME_SECOND		(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_SECOND, TCI_SECOND))			/* TCITime */
#define	TCI_C_TIME			(TCI_C_TIME_HOUR_TO_SECOND)												/* TCITime */

#define	TCI_C_TIMESTAMP_YEAR_TO_HOUR	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_HOUR))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_YEAR_TO_MINUTE	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_MINUTE))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_YEAR_TO_SECOND	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_SECOND))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_YEAR_TO_MILLISECOND (TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_MILLISECOND))		/* TCITimestamp */
#define	TCI_C_TIMESTAMP_MONTH_TO_HOUR	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_HOUR))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_MONTH_TO_MINUTE	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_MINUTE))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_MONTH_TO_SECOND	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_SECOND))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_MONTH_TO_MILLISECOND (TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_MILLISECOND))	/* TCITimestamp */
#define	TCI_C_TIMESTAMP_DAY_TO_HOUR	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_HOUR))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_DAY_TO_MINUTE	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_MINUTE))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_DAY_TO_SECOND	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_SECOND))			/* TCITimestamp */
#define	TCI_C_TIMESTAMP_DAY_TO_MILLISECOND (TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_MILLISECOND))		/* TCITimestamp */
#define	TCI_C_TIMESTAMP_HOUR_TO_MILLISECOND (TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_MILLISECOND))		/* TCITimestamp */
#define	TCI_C_TIMESTAMP_MINUTE_TO_MILLISECOND (TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MINUTE, TCI_MILLISECOND))	/* TCITimestamp */
#define	TCI_C_TIMESTAMP_SECOND_TO_MILLISECOND (TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_SECOND, TCI_MILLISECOND))	/* TCITimestamp */
#define	TCI_C_TIMESTAMP_MILLISECOND	(TCI_C_TYPE_DATE | TCI_MAKE_HL(TCI_MILLISECOND, TCI_MILLISECOND))	/* TCITimestamp */
#define	TCI_C_TIMESTAMP			(TCI_C_TIMESTAMP_YEAR_TO_MILLISECOND)					/* TCITimestamp */

#define	TCI_C_TYPE_INTERVAL		(TCI_TYPE | TCI_TYPE_SPECIAL | TCI_TYPE_INTERVAL)			/* TCIInterval */

#define	TCI_C_INTERVAL_YEAR		(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_YEAR, TCI_YEAR))			/* TCIInterval */
#define	TCI_C_INTERVAL_YEAR_TO_MONTH	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_YEAR, TCI_MONTH))		/* TCIInterval */
#define	TCI_C_INTERVAL_MONTH		(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MONTH, TCI_MONTH))		/* TCIInterval */

#define	TCI_C_INTERVAL_DAY		(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_DAY))			/* TCIInterval */
#define	TCI_C_INTERVAL_DAY_TO_HOUR	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_HOUR))			/* TCIInterval */
#define	TCI_C_INTERVAL_DAY_TO_MINUTE	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_MINUTE))		/* TCIInterval */
#define	TCI_C_INTERVAL_DAY_TO_SECOND	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_SECOND))		/* TCIInterval */
#define	TCI_C_INTERVAL_DAY_TO_MILLISECOND (TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_MILLISECOND))		/* TCIInterval */
#define	TCI_C_INTERVAL_HOUR		(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_HOUR))			/* TCIInterval */
#define	TCI_C_INTERVAL_HOUR_TO_MINUTE	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_MINUTE))		/* TCIInterval */
#define	TCI_C_INTERVAL_HOUR_TO_SECOND	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_SECOND))		/* TCIInterval */
#define	TCI_C_INTERVAL_HOUR_TO_MILLISECOND (TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_MILLISECOND))	/* TCIInterval */
#define	TCI_C_INTERVAL_MINUTE		(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MINUTE, TCI_MINUTE))		/* TCIInterval */
#define	TCI_C_INTERVAL_MINUTE_TO_SECOND	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MINUTE, TCI_SECOND))		/* TCIInterval */
#define	TCI_C_INTERVAL_MINUTE_TO_MILLISECOND (TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MINUTE, TCI_MILLISECOND))	/* TCIInterval */
#define	TCI_C_INTERVAL_SECOND		(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_SECOND, TCI_SECOND))		/* TCIInterval */
#define	TCI_C_INTERVAL_SECOND_TO_MILLISECOND (TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_SECOND, TCI_MILLISECOND))	/* TCIInterval */
#define	TCI_C_INTERVAL_MILLISECOND	(TCI_C_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MILLISECOND, TCI_MILLISECOND))	/* TCIInterval */

#define	TCI_C_SINT1			TCI_C_INT1
#define	TCI_C_SINT2			TCI_C_INT2
#define	TCI_C_SINT4			TCI_C_INT4
#define	TCI_C_SINT8			TCI_C_INT8

#define	TCI_C_UINT1			(TCI_C_INT1 | TCI_TYPE_UNSIGNED)	/* unsigned char */
#define	TCI_C_UINT2			(TCI_C_INT2 | TCI_TYPE_UNSIGNED)	/* unsigned short */
#define	TCI_C_UINT4			(TCI_C_INT4 | TCI_TYPE_UNSIGNED)	/* unsigned int */
#define	TCI_C_UINT8			(TCI_C_INT8 | TCI_TYPE_UNSIGNED)	/* TCIUBigint */

#define	TCI_C_SCHAR			TCI_C_CHAR
#define	TCI_C_WCHAR			(TCI_C_CHAR | TCI_TYPE_WIDE)		/* wchar_t * */
#define	TCI_C_SCHARPTR			TCI_C_CHARPTR
#define	TCI_C_WCHARPTR			(TCI_C_CHARPTR | TCI_TYPE_WIDE)		/* wchar_t ** */

#define	TCI_C_SFILE			TCI_C_FILE
#define	TCI_C_WFILE			(TCI_C_FILE | TCI_TYPE_WIDE)		/* wchar_t * */
#define	TCI_C_SCFILE			TCI_C_CFILE
#define	TCI_C_WCFILE			(TCI_C_CFILE | TCI_TYPE_WIDE)		/* wchar_t * */
#define	TCI_C_SFILEPTR			TCI_C_FILEPTR
#define	TCI_C_WFILEPTR			(TCI_C_FILEPTR | TCI_TYPE_WIDE)		/* wchar_t ** */
#define	TCI_C_SCFILEPTR			TCI_C_CFILEPTR
#define	TCI_C_WCFILEPTR			(TCI_C_CFILEPTR | TCI_TYPE_WIDE)		/* wchar_t ** */

/* SQL data types */

#define TCI_SQL_TYPE		(TCI_TYPE | TCI_TYPE_SQL)

#define	TCI_SQL_UNDEFTYPE	(TCI_SQL_TYPE | 0x00)	/* undefined type (mapping etc.) */

#define	TCI_SQL_BOOL		(TCI_SQL_TYPE | 0x01)	/* true / false */
#define	TCI_SQL_TINYINT		(TCI_SQL_TYPE | 0x02)	/* -128 .. 127 */
#define	TCI_SQL_SMALLINT	(TCI_SQL_TYPE | 0x03)	/* -2**15 .. 2**15-1 */
#define	TCI_SQL_INTEGER		(TCI_SQL_TYPE | 0x04)	/* -2**31 .. 2**31-1 */
#define	TCI_SQL_NUMERIC		(TCI_SQL_TYPE | 0x05)	/* no prec, prec, prec/scale */
#define	TCI_SQL_FLOAT		(TCI_SQL_TYPE | 0x06)
#define	TCI_SQL_DOUBLE		(TCI_SQL_TYPE | 0x07)
#define	TCI_SQL_CHAR		(TCI_SQL_TYPE | 0x08)	/* prec, *, with blank padding */
#define	TCI_SQL_VARCHAR		(TCI_SQL_TYPE | 0x09)	/* prec, *, without blank padding */
#define	TCI_SQL_BINARY		(TCI_SQL_TYPE | 0x0a)	/* prec, * */
#define	TCI_SQL_BIT		(TCI_SQL_TYPE | 0x0b)	/* prec, * */
#define	TCI_SQL_BLOB		(TCI_SQL_TYPE | 0x0c)
#define	TCI_SQL_FILEREF		(TCI_SQL_TYPE | 0x0d)
#define	TCI_SQL_BITSHORT	(TCI_SQL_TYPE | 0x0e)	/* prec, * */
#define	TCI_SQL_BIGINT		(TCI_SQL_TYPE | 0x0f)	/* -2**63 .. 2**63-1 */
#define	TCI_SQL_CLOB		(TCI_SQL_TYPE | 0x10)

#define	TCI_SQL_TYPE_DATE	(TCI_SQL_TYPE | TCI_TYPE_DATE)

#define	TCI_SQL_DATE_YEAR			(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_YEAR))
#define	TCI_SQL_DATE_YEAR_TO_MONTH		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_MONTH))
#define	TCI_SQL_DATE_YEAR_TO_DAY		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_DAY))
#define	TCI_SQL_DATE_YEAR_TO_HOUR		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_HOUR))
#define	TCI_SQL_DATE_YEAR_TO_MINUTE		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_MINUTE))
#define	TCI_SQL_DATE_YEAR_TO_SECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_SECOND))
#define	TCI_SQL_DATE_YEAR_TO_MILLISECOND	(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_YEAR, TCI_MILLISECOND))
#define	TCI_SQL_DATE_MONTH			(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_MONTH))
#define	TCI_SQL_DATE_MONTH_TO_DAY		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_DAY))
#define	TCI_SQL_DATE_MONTH_TO_HOUR		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_HOUR))
#define	TCI_SQL_DATE_MONTH_TO_MINUTE		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_MINUTE))
#define	TCI_SQL_DATE_MONTH_TO_SECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_SECOND))
#define	TCI_SQL_DATE_MONTH_TO_MILLISECOND	(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MONTH, TCI_MILLISECOND))
#define	TCI_SQL_DATE_DAY			(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_DAY))
#define	TCI_SQL_DATE_DAY_TO_HOUR		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_HOUR))
#define	TCI_SQL_DATE_DAY_TO_MINUTE		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_MINUTE))
#define	TCI_SQL_DATE_DAY_TO_SECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_SECOND))
#define	TCI_SQL_DATE_DAY_TO_MILLISECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_DAY, TCI_MILLISECOND))
#define	TCI_SQL_DATE_HOUR			(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_HOUR))
#define	TCI_SQL_DATE_HOUR_TO_MINUTE		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_MINUTE))
#define	TCI_SQL_DATE_HOUR_TO_SECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_SECOND))
#define	TCI_SQL_DATE_HOUR_TO_MILLISECOND	(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_HOUR, TCI_MILLISECOND))
#define	TCI_SQL_DATE_MINUTE			(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MINUTE, TCI_MINUTE))
#define	TCI_SQL_DATE_MINUTE_TO_SECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MINUTE, TCI_SECOND))
#define	TCI_SQL_DATE_MINUTE_TO_MILLISECOND	(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MINUTE, TCI_MILLISECOND))
#define	TCI_SQL_DATE_SECOND			(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_SECOND, TCI_SECOND))
#define	TCI_SQL_DATE_SECOND_TO_MILLISECOND	(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_SECOND, TCI_MILLISECOND))
#define	TCI_SQL_DATE_MILLISECOND		(TCI_SQL_TYPE_DATE | TCI_MAKE_HL(TCI_MILLISECOND, TCI_MILLISECOND))

#define	TCI_SQL_DATE				(TCI_SQL_DATE_YEAR_TO_DAY)
#define	TCI_SQL_TIME				(TCI_SQL_DATE_HOUR_TO_SECOND)
#define	TCI_SQL_TIMESTAMP			(TCI_SQL_DATE_YEAR_TO_MILLISECOND)

#define	TCI_SQL_TYPE_INTERVAL			(TCI_SQL_TYPE | TCI_TYPE_INTERVAL)

#define	TCI_SQL_INTERVAL_YEAR			(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_YEAR, TCI_YEAR))
#define	TCI_SQL_INTERVAL_YEAR_TO_MONTH		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_YEAR, TCI_MONTH))
#define	TCI_SQL_INTERVAL_MONTH			(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MONTH, TCI_MONTH))
#define	TCI_SQL_INTERVAL_DAY			(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_DAY))
#define	TCI_SQL_INTERVAL_DAY_TO_HOUR		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_HOUR))
#define	TCI_SQL_INTERVAL_DAY_TO_MINUTE		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_MINUTE))
#define	TCI_SQL_INTERVAL_DAY_TO_SECOND		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_SECOND))
#define	TCI_SQL_INTERVAL_DAY_TO_MILLISECOND	(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_DAY, TCI_MILLISECOND))
#define	TCI_SQL_INTERVAL_HOUR			(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_HOUR))
#define	TCI_SQL_INTERVAL_HOUR_TO_MINUTE		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_MINUTE))
#define	TCI_SQL_INTERVAL_HOUR_TO_SECOND		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_SECOND))
#define	TCI_SQL_INTERVAL_HOUR_TO_MILLISECOND	(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_HOUR, TCI_MILLISECOND))
#define	TCI_SQL_INTERVAL_MINUTE			(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MINUTE, TCI_MINUTE))
#define	TCI_SQL_INTERVAL_MINUTE_TO_SECOND	(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MINUTE, TCI_SECOND))
#define	TCI_SQL_INTERVAL_MINUTE_TO_MILLISECOND	(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MINUTE, TCI_MILLISECOND))
#define	TCI_SQL_INTERVAL_SECOND			(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_SECOND, TCI_SECOND))
#define	TCI_SQL_INTERVAL_SECOND_TO_MILLISECOND	(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_SECOND, TCI_MILLISECOND))
#define	TCI_SQL_INTERVAL_MILLISECOND		(TCI_SQL_TYPE_INTERVAL | TCI_MAKE_HL(TCI_MILLISECOND, TCI_MILLISECOND))

/* attribute ids */
#define	TCI_ATTR_BULKSIZE			 1
#define	TCI_ATTR_CODEPAGE			 2
#define	TCI_ATTR_COLUMN_ACTSIZE			 3
#define	TCI_ATTR_COLUMN_BOUND			 4
#define	TCI_ATTR_COLUMN_BUFFER			 5
#define	TCI_ATTR_COLUMN_BUFFERTYPE		 6
#define	TCI_ATTR_COLUMN_COUNT			 7
#define	TCI_ATTR_COLUMN_INDICATOR		 8
#define	TCI_ATTR_COLUMN_MAXSIZE			 9
#define	TCI_ATTR_COLUMN_NAME			10
#define	TCI_ATTR_COLUMN_NAME_FROM_DB		11
#define	TCI_ATTR_COLUMN_PRECISION		12
#define	TCI_ATTR_COLUMN_SCALE			13
#define	TCI_ATTR_COLUMN_TYPE			14
#define	TCI_ATTR_CONNECTION			15
#define	TCI_ATTR_CONNECTION_COUNT		16
#define	TCI_ATTR_CONNECTION_TIMEOUT		17
#define	TCI_ATTR_CONSISTENCY_LEVEL		18
#define	TCI_ATTR_DATA_DIRECTORY			19
#define	TCI_ATTR_DATABASE_NAME			20
#define	TCI_ATTR_ENVIRONMENT			21
#define	TCI_ATTR_ERROR				22
#define	TCI_ATTR_ERROR_COUNT			23
#define	TCI_ATTR_SCROLLABLE			24
#define	TCI_ATTR_NAME				25
#define	TCI_ATTR_PARAMETER_BOUND		26
#define	TCI_ATTR_PARAMETER_BUFFER		27
#define	TCI_ATTR_PARAMETER_BUFFERTYPE		28
#define	TCI_ATTR_PARAMETER_COUNT		29
#define	TCI_ATTR_PARAMETER_INDICATOR		30
#define	TCI_ATTR_PARAMETER_MAXSIZE		31
#define	TCI_ATTR_PARAMETER_PRECISION		32
#define	TCI_ATTR_PARAMETER_SCALE		33
#define	TCI_ATTR_PARAMETER_TYPE			34
#define	TCI_ATTR_POSITION			35
#define	TCI_ATTR_QUERY_TYPE			36
#define	TCI_ATTR_QUERY_UPDATABLE		37
#define	TCI_ATTR_RESULTSET			38
#define	TCI_ATTR_RESULTSET_COUNT		39
#define	TCI_ATTR_ROWCOUNT			40
#define	TCI_ATTR_SORTORDER			42
#define	TCI_ATTR_SQL				43
#define	TCI_ATTR_STATE				44
#define	TCI_ATTR_STATEMENT			45
#define	TCI_ATTR_STATEMENT_COUNT		46
#define	TCI_ATTR_STATEMENT_TIMEOUT		47
#define	TCI_ATTR_TIMEOUT			48
#define	TCI_ATTR_TRANSACTION			49
#define	TCI_ATTR_TRANSACTION_COUNT		50
#define	TCI_ATTR_TRANSACTION_TIMEOUT		51
#define	TCI_ATTR_USER_NAME			52
#define	TCI_ATTR_WIDECHAR_SUBSTITUTE		53
#define	TCI_ATTR_RECORDS_TRIED			54
#define	TCI_ATTR_RECORDS_TOUCHED		55
#define TCI_ATTR_INIFILE			56
#define TCI_ATTR_INSENSITIVE			57
#define TCI_ATTR_EOD				58
#define	TCI_ATTR_TRACEFILE			59
#define	TCI_ATTR_TRACE				60
#define	TCI_ATTR_REFERENCED_RESULTSET		61
#define TCI_ATTR_ABORT_CALLBACK			62
#define TCI_ATTR_CLIENTCODEPAGE			63
#define TCI_ATTR_NUMBER_OPEN_RESULTSETS		64
#define	TCI_ATTR_CHAR_SUBSTITUTE		65
#define	TCI_ATTR_PLANFILE			66
#define	TCI_ATTR_PLANFILE_MODE			67
#define	TCI_ATTR_VERSION			68
#define	TCI_ATTR_NUMBER_PARAMETER_BINDINGS	69
#define	TCI_ATTR_NUMBER_COLUMN_BINDINGS		70
#define	TCI_ATTR_PARAMETER_NAME			71
#define	TCI_ATTR_PARAMETER_MODE			72
#define	TCI_ATTR_PARAMETER_TYPENAME		73
#define	TCI_ATTR_COLUMN_TYPENAME		74
#define TCI_ATTR_PARAMETER_DISPSIZE		75
#define TCI_ATTR_COLUMN_DISPSIZE		76
#define TCI_ATTR_CONTINUE_ON_BULKERROR		77
#define TCI_ATTR_DECIMAL_POINT			78
#define TCI_ATTR_THOUSANDS_SEP			79
#define TCI_ATTR_THOUSANDS_GROUP		80
#define TCI_ATTR_PARAMETER_POSITION		81
#define TCI_ATTR_COLUMN_POSITION		82
#define TCI_ATTR_PARAMETER_BUFFER_SKIP		83
#define TCI_ATTR_PARAMETER_INDICATOR_SKIP	84
#define TCI_ATTR_PARAMETER_ACTSIZE_SKIP		85
#define TCI_ATTR_COLUMN_BUFFER_SKIP		86
#define TCI_ATTR_COLUMN_INDICATOR_SKIP		87
#define TCI_ATTR_COLUMN_ACTSIZE_SKIP		88
#define	TCI_ATTR_PARAMETER_ACTSIZE		89
#define	TCI_ATTR_EXPECT_NTS			90
#define	TCI_ATTR_TRUNCATE_NUMBERS		91
#define	TCI_ATTR_AUTO_COMMIT			92
#define	TCI_ATTR_BYTE_STR_COMPAT		93
#define TCI_ATTR_AUTO_COMMIT_CONS_LEVEL     94
#define TCI_ATTR_AUTO_COMMIT_TIMEOUT        95
#define TCI_ATTR_COLUMN_DATAPOINTER		96
#define TCI_ATTR_SUPPORTED_C_TYPES		97
#define TCI_ATTR_FREE_RECURSIVELY		98
#define	TCI_ATTR_COLUMN_DELIVERED_BYTES		99
#define TCI_ATTR_CFILE_LOCALE			100
#define TCI_ATTR_CFILE_CODEPAGE			101
#define	TCI_ATTR_COLUMN_ORIG_COLNAME		102
#define	TCI_ATTR_COLUMN_ORIG_TABNAME		103
#define	TCI_ATTR_COLUMN_ORIG_SCHNAME		104
#define TCI_ATTR_SCHEMA				105
#define TCI_ATTR_TRANSBASE_D			106
#define	TCI_ATTR_SINGLECHAR			107
#define	TCI_ATTR_LOCALE				108
#define	TCI_ATTR_DELIVER_NTS			109
#define TCI_ATTR_ACT_ROW		110
#define	TCI_ATTR_ONEROW_ERROR			111
#define	TCI_ATTR_CONNECTION_VALID		112
#define	TCI_ATTR_DATABASE_UUID			113
#define	TCI_ATTR_LOCALE4TRANSBASE_D			114
#define	TCI_ATTR_COMM_PROTOCOL			115
#define	TCI_ATTR_COMM_TRUSTED			116


/* states */
#define TCI_SUCCESS				0
#define TCI_ERROR				(-1)
#define TCI_NO_DATA_FOUND			100

#define TCI_INVALID_HANDLE			17001
#define TCI_TRANSACTIONS_ROLLEDBACK			17027
#define TCI_DATA_TRUNCATION			17031

/* planfile modes */
#define TCI_APPEND					0
#define TCI_OVERWRITE					1
#define TCI_PROFILE					2

/* additional states */
#define TCI_NOT_PREPARED					0
#define TCI_PREPARED					1

/* TA consistency states */
#define TCI_CONS_1					1
#define TCI_CONS_2					2
#define TCI_CONS_3					3

/* query eod states */
#define	TCI_NO_TUPLE					0
#define	TCI_ONE_TUPLE					1
#define	TCI_MORE_TUPLES					2

/* scroll modes */
#define	TCI_FETCH_NEXT					1		/* Fetch next tuple (nTupleOffset ignored) */
#define	TCI_FETCH_PRIOR					2		/* Fetch previous tuple (nTupleOffset ignored) */
#define	TCI_FETCH_FIRST					3		/* Fetch first tuple (nTupleOffset ignored) */
#define	TCI_FETCH_LAST					4		/* Fetch last tuple (nTupleOffset ignored) */
#define	TCI_FETCH_ABSOLUTE				5		/* Fetch tuple nTupleOffset from start */
#define	TCI_FETCH_RELATIVE				6		/* Fetch tuple nTupleOffset from here */

/* codepages */
#define TCI_CPG_INVALID					0
#define TCI_CPG_UNICODE					1
#define TCI_CPG_PROPRIET				2
#define TCI_CPG_ASCII					3
#define TCI_CPG_LOCALE					4
#define TCI_CPG_UTF8					5
#define TCI_CPG_EUC					6
#define TCI_CPG_JIS					7
#define TCI_CPG_UCS2					8		/* unicode 2 byte */
#define TCI_CPG_UCS4					10		/* unicode 4 byte */
#define TCI_CPG_UCS2_LE					8		/* unicode little endian */
#define TCI_CPG_UTF16_LE				TCI_CPG_UCS2_LE
#define TCI_CPG_UCS2_BE					9		/* unicode big endian */
#define TCI_CPG_UTF16_BE				TCI_CPG_UCS2_BE
#define TCI_CPG_UCS4_LE					10		/* unicode little endian */
#define TCI_CPG_UTF32_LE				TCI_CPG_UCS4_LE
#define TCI_CPG_UCS4_BE					11		/* unicode big endian */
#define TCI_CPG_UTF32_BE				TCI_CPG_UCS4_BE
#define TCI_CPG_MACHINE					20

/* compatibility to ODBC */
#define TCI_NTS						(-3)
#define TCI_NULL_VALUE					(-1)

typedef struct TCIEnvironment TCIEnvironment;
typedef struct TCIError TCIError;
typedef struct TCIConnection TCIConnection;
typedef struct TCITransaction TCITransaction;
typedef struct TCIStatement TCIStatement;
typedef struct TCIResultSet TCIResultSet;

#endif /* _TCIBASIC_H */
#if !defined(_UNICODE)

#define	TCI_C_TCHAR			TCI_C_SCHAR
#define	TCI_C_TFILE			TCI_C_SFILE
#define	TCI_C_TCFILE			TCI_C_SCFILE
#define	TCI_C_TCHARPTR			TCI_C_SCHARPTR
#define	TCI_C_TFILEPTR			TCI_C_SFILEPTR
#define	TCI_C_TCFILEPTR			TCI_C_SCFILEPTR

#define TCIInitSSL TCIInitSSLA
void     TCI_API TCIInitSSLA (void);
#define TCICleanupSSL TCICleanupSSLA
void     TCI_API TCICleanupSSLA (void);
#define TCIAllocEnvironment TCIAllocEnvironmentA
TCIState TCI_API TCIAllocEnvironmentA (TCIEnvironment ** ppEnvId );
#define TCIAllocError TCIAllocErrorA
TCIState TCI_API TCIAllocErrorA (TCIEnvironment * pEnvId , TCIError ** ppErrId );
#define TCIAllocConnection TCIAllocConnectionA
TCIState TCI_API TCIAllocConnectionA (TCIEnvironment * pEnvId , TCIError * pErrId , TCIConnection ** ppConnId );
#define TCIAllocTransaction TCIAllocTransactionA
TCIState TCI_API TCIAllocTransactionA (TCIEnvironment * pEnvId , TCIError * pErrId , TCITransaction ** ppTaId );
#define TCIAllocStatement TCIAllocStatementA
TCIState TCI_API TCIAllocStatementA (TCIConnection * pConnId , TCIError * pErrId , TCIStatement ** ppStmtId );
#define TCIAllocResultSet TCIAllocResultSetA
TCIState TCI_API TCIAllocResultSetA (TCIStatement * pStmtId , TCIError * pErrId , TCIResultSet ** ppResId );

#define TCIFreeEnvironment TCIFreeEnvironmentA
TCIState TCI_API TCIFreeEnvironmentA (TCIEnvironment * pEnvId );
#define TCIFreeError TCIFreeErrorA
TCIState TCI_API TCIFreeErrorA (TCIError * pErrId );
#define TCIFreeConnection TCIFreeConnectionA
TCIState TCI_API TCIFreeConnectionA (TCIConnection * pConnId );
#define TCIFreeTransaction TCIFreeTransactionA
TCIState TCI_API TCIFreeTransactionA (TCITransaction * pTaId );
#define TCIFreeStatement TCIFreeStatementA
TCIState TCI_API TCIFreeStatementA (TCIStatement * pStmtId );
#define TCIFreeResultSet TCIFreeResultSetA
TCIState TCI_API TCIFreeResultSetA (TCIResultSet * pResId );

#define TCIConnect TCIConnectA
TCIState TCI_API TCIConnectA (TCIConnection * pConnId , char * pszDatabaseName );
#define TCIDisconnect TCIDisconnectA
TCIState TCI_API TCIDisconnectA (TCIConnection * pConnId );
#define TCILogin TCILoginA
TCIState TCI_API TCILoginA (TCIConnection * pConnId , char * pszUserName , char * pszPassword );
#define TCILogout TCILogoutA
TCIState TCI_API TCILogoutA (TCIConnection * pConnId );

#define TCIBeginTransaction TCIBeginTransactionA
TCIState TCI_API TCIBeginTransactionA (TCITransaction * pTaId , TCIConnection * pConnId );
#define TCICommitTransaction TCICommitTransactionA
TCIState TCI_API TCICommitTransactionA (TCITransaction * pTaId );
#define TCIRollbackTransaction TCIRollbackTransactionA
TCIState TCI_API TCIRollbackTransactionA (TCITransaction * pTaId );
#define TCIBreak TCIBreakA
TCIState TCI_API TCIBreakA (TCITransaction * pTaId );

#define TCIPrepare TCIPrepareA
TCIState TCI_API TCIPrepareA (TCIStatement * pStmtId , char * pszStatement );
#define TCIBindParameter TCIBindParameterA
TCIState TCI_API TCIBindParameterA (TCIResultSet * pResId , TCIColumnnumber nParamNr , void * pBuffer , Int4 nMaxBufferSize , Int4 * pnBufferSize , TCIDatatype nType , Int2 * pnIndicator );
#define TCIBindParameterByName TCIBindParameterByNameA
TCIState TCI_API TCIBindParameterByNameA (TCIResultSet * pResId , char * pszParameterName , void * pBuffer , Int4 nMaxBufferSize , Int4 * pnBufferSize , TCIDatatype nType , Int2 * pnIndicator );
#define TCIBindParameterArrayOfStruct TCIBindParameterArrayOfStructA
TCIState TCI_API TCIBindParameterArrayOfStructA (TCIResultSet * pResId , TCIColumnnumber nParamNr , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIBindParameterArrayOfStructByName TCIBindParameterArrayOfStructByNameA
TCIState TCI_API TCIBindParameterArrayOfStructByNameA (TCIResultSet * pResId , char * pszParameterName , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIUnbindParameters TCIUnbindParametersA
TCIState TCI_API TCIUnbindParametersA (TCIResultSet * pResId );
#define TCIAddBatch TCIAddBatchA
TCIState TCI_API TCIAddBatchA (TCIResultSet * pResId );
#define TCIExecute TCIExecuteA
TCIState TCI_API TCIExecuteA (TCIResultSet * pResId , Uint4 nIterations , Uint4 nTuplesToFetch );
#define TCIExecuteDirect TCIExecuteDirectA
TCIState TCI_API TCIExecuteDirectA (TCIResultSet * pResId , char * pszStatement , Uint4 nIterations , Uint4 nTuplesToFetch );
#define TCIBindColumn TCIBindColumnA
TCIState TCI_API TCIBindColumnA (TCIResultSet * pResId , TCIColumnnumber nColNr , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIBindColumnByName TCIBindColumnByNameA
TCIState TCI_API TCIBindColumnByNameA (TCIResultSet * pResId , char * pszColumnName , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIBindColumnArrayOfStruct TCIBindColumnArrayOfStructA
TCIState TCI_API TCIBindColumnArrayOfStructA (TCIResultSet * pResId , TCIColumnnumber nColNr , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIBindColumnArrayOfStructByName TCIBindColumnArrayOfStructByNameA
TCIState TCI_API TCIBindColumnArrayOfStructByNameA (TCIResultSet * pResId , char * pszColumnName , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIUnbindColumns TCIUnbindColumnsA
TCIState TCI_API TCIUnbindColumnsA (TCIResultSet * pResId );
#define TCIResetColumn TCIResetColumnA
TCIState TCI_API TCIResetColumnA (TCIResultSet * pResId , TCIColumnnumber nColNr );
#define TCIFetch TCIFetchA
TCIState TCI_API TCIFetchA (TCIResultSet * pResId , Uint4 nTuplesToFetch , Uint2 nDirection , Int4 nTupleOffset );
#define TCIClose TCICloseA
TCIState TCI_API TCICloseA (TCIResultSet * pResId );
#define TCICancelResultSet TCICancelResultSetA
TCIState TCI_API TCICancelResultSetA (TCIResultSet * pResId );

#define TCISetData TCISetDataA
TCIState TCI_API TCISetDataA (TCIResultSet * pResId , TCIColumnnumber nParamNr , void * pBuffer , Int4 nBufferSize , TCIDatatype nType , Int2 nIndicator );
#define TCISetDataByName TCISetDataByNameA
TCIState TCI_API TCISetDataByNameA (TCIResultSet * pResId , char * pszParameterName , void * pBuffer , Int4 nBufferSize , TCIDatatype nType , Int2 nIndicator );
#define TCIGetData TCIGetDataA
TCIState TCI_API TCIGetDataA (TCIResultSet * pResId , TCIColumnnumber nColNr , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIGetDataByName TCIGetDataByNameA
TCIState TCI_API TCIGetDataByNameA (TCIResultSet * pResId , char * pszColumnName , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIGetDataSize TCIGetDataSizeA
TCIState TCI_API TCIGetDataSizeA (TCIResultSet * pResId , TCIColumnnumber nColNr , TCIDatatype nType , Int4 * pActualSize , Int2 * pIndicator );
#define TCIGetDataSizeByName TCIGetDataSizeByNameA
TCIState TCI_API TCIGetDataSizeByNameA (TCIResultSet * pResId , char * pszColumnName , TCIDatatype nType , Int4 * pActualSize , Int2 * pIndicator );
#define TCIGetDataCharLength TCIGetDataCharLengthA
TCIState TCI_API TCIGetDataCharLengthA (TCIResultSet * pResId , TCIColumnnumber nColNr , Int4 * pCharLength , Int2 * pIndicator );
#define TCIGetDataCharLengthByName TCIGetDataCharLengthByNameA
TCIState TCI_API TCIGetDataCharLengthByNameA (TCIResultSet * pResId , char * pszColumnName , Int4 * pCharLength , Int2 * pIndicator );

#define TCIGetError TCIGetErrorA
TCIState TCI_API TCIGetErrorA (TCIError * pErrId , Uint4 nRecord , Uint2 nthError , char * pErrorMessage , Uint4 nMaxMessageSize , TBErrorCode * pErrorCode , char * pSqlstate );
#define TCIGetEnvironmentError TCIGetEnvironmentErrorA
TCIState TCI_API TCIGetEnvironmentErrorA (TCIEnvironment * pEnvId , Uint2 nthError , char * pErrorMessage , Uint4 nMaxMessageSize , TBErrorCode * pErrorCode , char * pSqlstate );

#define TCIGetEnvironmentAttribute TCIGetEnvironmentAttributeA
TCIState TCI_API TCIGetEnvironmentAttributeA (TCIEnvironment * pEnvId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetErrorAttribute TCIGetErrorAttributeA
TCIState TCI_API TCIGetErrorAttributeA (TCIError * pErrId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetConnectionAttribute TCIGetConnectionAttributeA
TCIState TCI_API TCIGetConnectionAttributeA (TCIConnection * pConnId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetTransactionAttribute TCIGetTransactionAttributeA
TCIState TCI_API TCIGetTransactionAttributeA (TCITransaction * pTaId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetStatementAttribute TCIGetStatementAttributeA
TCIState TCI_API TCIGetStatementAttributeA (TCIStatement * pStmtId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetStatementAttributeByName TCIGetStatementAttributeByNameA
TCIState TCI_API TCIGetStatementAttributeByNameA (TCIStatement * pStmtId , Uint2 nAttributeCode , char * pszName , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetResultSetAttribute TCIGetResultSetAttributeA
TCIState TCI_API TCIGetResultSetAttributeA (TCIResultSet * pResId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetResultSetAttributeByName TCIGetResultSetAttributeByNameA
TCIState TCI_API TCIGetResultSetAttributeByNameA (TCIResultSet * pResId , Uint2 nAttributeCode , char * pszName , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );

#define TCISetEnvironmentAttribute TCISetEnvironmentAttributeA
TCIState TCI_API TCISetEnvironmentAttributeA (TCIEnvironment * pEnvId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetErrorAttribute TCISetErrorAttributeA
TCIState TCI_API TCISetErrorAttributeA (TCIError * pErrId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetConnectionAttribute TCISetConnectionAttributeA
TCIState TCI_API TCISetConnectionAttributeA (TCIConnection * pConnId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetTransactionAttribute TCISetTransactionAttributeA
TCIState TCI_API TCISetTransactionAttributeA (TCITransaction * pTaId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetStatementAttribute TCISetStatementAttributeA
TCIState TCI_API TCISetStatementAttributeA (TCIStatement * pStmtId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetResultSetAttribute TCISetResultSetAttributeA
TCIState TCI_API TCISetResultSetAttributeA (TCIResultSet * pResId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );

#define TCIReleaseHeap TCIReleaseHeapA
TCIState TCI_API TCIReleaseHeapA (TCIResultSet * pResId , void * pBuffer );

#else

#define	TCI_C_TCHAR			TCI_C_WCHAR
#define	TCI_C_TFILE			TCI_C_WFILE
#define	TCI_C_TCFILE			TCI_C_WCFILE
#define	TCI_C_TCHARPTR			TCI_C_WCHARPTR
#define	TCI_C_TFILEPTR			TCI_C_WFILEPTR
#define	TCI_C_TCFILEPTR			TCI_C_WCFILEPTR

#define TCIInitSSL TCIInitSSLW
void     TCI_API TCIInitSSLW (void);
#define TCICleanupSSL TCICleanupSSLW
void     TCI_API TCICleanupSSLW (void);
#define TCIAllocEnvironment TCIAllocEnvironmentW
TCIState TCI_API TCIAllocEnvironmentW (TCIEnvironment ** ppEnvId );
#define TCIAllocError TCIAllocErrorW
TCIState TCI_API TCIAllocErrorW (TCIEnvironment * pEnvId , TCIError ** ppErrId );
#define TCIAllocConnection TCIAllocConnectionW
TCIState TCI_API TCIAllocConnectionW (TCIEnvironment * pEnvId , TCIError * pErrId , TCIConnection ** ppConnId );
#define TCIAllocTransaction TCIAllocTransactionW
TCIState TCI_API TCIAllocTransactionW (TCIEnvironment * pEnvId , TCIError * pErrId , TCITransaction ** ppTaId );
#define TCIAllocStatement TCIAllocStatementW
TCIState TCI_API TCIAllocStatementW (TCIConnection * pConnId , TCIError * pErrId , TCIStatement ** ppStmtId );
#define TCIAllocResultSet TCIAllocResultSetW
TCIState TCI_API TCIAllocResultSetW (TCIStatement * pStmtId , TCIError * pErrId , TCIResultSet ** ppResId );

#define TCIFreeEnvironment TCIFreeEnvironmentW
TCIState TCI_API TCIFreeEnvironmentW (TCIEnvironment * pEnvId );
#define TCIFreeError TCIFreeErrorW
TCIState TCI_API TCIFreeErrorW (TCIError * pErrId );
#define TCIFreeConnection TCIFreeConnectionW
TCIState TCI_API TCIFreeConnectionW (TCIConnection * pConnId );
#define TCIFreeTransaction TCIFreeTransactionW
TCIState TCI_API TCIFreeTransactionW (TCITransaction * pTaId );
#define TCIFreeStatement TCIFreeStatementW
TCIState TCI_API TCIFreeStatementW (TCIStatement * pStmtId );
#define TCIFreeResultSet TCIFreeResultSetW
TCIState TCI_API TCIFreeResultSetW (TCIResultSet * pResId );

#define TCIConnect TCIConnectW
TCIState TCI_API TCIConnectW (TCIConnection * pConnId , wchar_t * pszDatabaseName );
#define TCIDisconnect TCIDisconnectW
TCIState TCI_API TCIDisconnectW (TCIConnection * pConnId );
#define TCILogin TCILoginW
TCIState TCI_API TCILoginW (TCIConnection * pConnId , wchar_t * pszUserName , wchar_t * pszPassword );
#define TCILogout TCILogoutW
TCIState TCI_API TCILogoutW (TCIConnection * pConnId );

#define TCIBeginTransaction TCIBeginTransactionW
TCIState TCI_API TCIBeginTransactionW (TCITransaction * pTaId , TCIConnection * pConnId );
#define TCICommitTransaction TCICommitTransactionW
TCIState TCI_API TCICommitTransactionW (TCITransaction * pTaId );
#define TCIRollbackTransaction TCIRollbackTransactionW
TCIState TCI_API TCIRollbackTransactionW (TCITransaction * pTaId );
#define TCIBreak TCIBreakW
TCIState TCI_API TCIBreakW (TCITransaction * pTaId );

#define TCIPrepare TCIPrepareW
TCIState TCI_API TCIPrepareW (TCIStatement * pStmtId , wchar_t * pszStatement );
#define TCIBindParameter TCIBindParameterW
TCIState TCI_API TCIBindParameterW (TCIResultSet * pResId , TCIColumnnumber nParamNr , void * pBuffer , Int4 nMaxBufferSize , Int4 * pnBufferSize , TCIDatatype nType , Int2 * pnIndicator );
#define TCIBindParameterByName TCIBindParameterByNameW
TCIState TCI_API TCIBindParameterByNameW (TCIResultSet * pResId , wchar_t * pszParameterName , void * pBuffer , Int4 nMaxBufferSize , Int4 * pnBufferSize , TCIDatatype nType , Int2 * pnIndicator );
#define TCIBindParameterArrayOfStruct TCIBindParameterArrayOfStructW
TCIState TCI_API TCIBindParameterArrayOfStructW (TCIResultSet * pResId , TCIColumnnumber nParamNr , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIBindParameterArrayOfStructByName TCIBindParameterArrayOfStructByNameW
TCIState TCI_API TCIBindParameterArrayOfStructByNameW (TCIResultSet * pResId , wchar_t * pszParameterName , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIUnbindParameters TCIUnbindParametersW
TCIState TCI_API TCIUnbindParametersW (TCIResultSet * pResId );
#define TCIAddBatch TCIAddBatchW
TCIState TCI_API TCIAddBatchW (TCIResultSet * pResId );
#define TCIExecute TCIExecuteW
TCIState TCI_API TCIExecuteW (TCIResultSet * pResId , Uint4 nIterations , Uint4 nTuplesToFetch );
#define TCIExecuteDirect TCIExecuteDirectW
TCIState TCI_API TCIExecuteDirectW (TCIResultSet * pResId , wchar_t * pszStatement , Uint4 nIterations , Uint4 nTuplesToFetch );
#define TCIBindColumn TCIBindColumnW
TCIState TCI_API TCIBindColumnW (TCIResultSet * pResId , TCIColumnnumber nColNr , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIBindColumnByName TCIBindColumnByNameW
TCIState TCI_API TCIBindColumnByNameW (TCIResultSet * pResId , wchar_t * pszColumnName , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIBindColumnArrayOfStruct TCIBindColumnArrayOfStructW
TCIState TCI_API TCIBindColumnArrayOfStructW (TCIResultSet * pResId , TCIColumnnumber nColNr , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIBindColumnArrayOfStructByName TCIBindColumnArrayOfStructByNameW
TCIState TCI_API TCIBindColumnArrayOfStructByNameW (TCIResultSet * pResId , wchar_t * pszColumnName , size_t nBufferSkip , size_t nIndicatorSkip , size_t nBufferSizeSkip );
#define TCIUnbindColumns TCIUnbindColumnsW
TCIState TCI_API TCIUnbindColumnsW (TCIResultSet * pResId );
#define TCIResetColumn TCIResetColumnW
TCIState TCI_API TCIResetColumnW (TCIResultSet * pResId , TCIColumnnumber nColNr );
#define TCIFetch TCIFetchW
TCIState TCI_API TCIFetchW (TCIResultSet * pResId , Uint4 nTuplesToFetch , Uint2 nDirection , Int4 nTupleOffset );
#define TCIClose TCICloseW
TCIState TCI_API TCICloseW (TCIResultSet * pResId );
#define TCICancelResultSet TCICancelResultSetW
TCIState TCI_API TCICancelResultSetW (TCIResultSet * pResId );

#define TCISetData TCISetDataW
TCIState TCI_API TCISetDataW (TCIResultSet * pResId , TCIColumnnumber nParamNr , void * pBuffer , Int4 nBufferSize , TCIDatatype nType , Int2 nIndicator );
#define TCISetDataByName TCISetDataByNameW
TCIState TCI_API TCISetDataByNameW (TCIResultSet * pResId , wchar_t * pszParameterName , void * pBuffer , Int4 nBufferSize , TCIDatatype nType , Int2 nIndicator );
#define TCIGetData TCIGetDataW
TCIState TCI_API TCIGetDataW (TCIResultSet * pResId , TCIColumnnumber nColNr , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIGetDataByName TCIGetDataByNameW
TCIState TCI_API TCIGetDataByNameW (TCIResultSet * pResId , wchar_t * pszColumnName , void * pBuffer , Int4 nMaxBufferSize , Int4 * pActualSize , TCIDatatype nType , Int2 * pIndicator );
#define TCIGetDataSize TCIGetDataSizeW
TCIState TCI_API TCIGetDataSizeW (TCIResultSet * pResId , TCIColumnnumber nColNr , TCIDatatype nType , Int4 * pActualSize , Int2 * pIndicator );
#define TCIGetDataSizeByName TCIGetDataSizeByNameW
TCIState TCI_API TCIGetDataSizeByNameW (TCIResultSet * pResId , wchar_t * pszColumnName , TCIDatatype nType , Int4 * pActualSize , Int2 * pIndicator );
#define TCIGetDataCharLength TCIGetDataCharLengthW
TCIState TCI_API TCIGetDataCharLengthW (TCIResultSet * pResId , TCIColumnnumber nColNr , Int4 * pCharLength , Int2 * pIndicator );
#define TCIGetDataCharLengthByName TCIGetDataCharLengthByNameW
TCIState TCI_API TCIGetDataCharLengthByNameW (TCIResultSet * pResId , wchar_t * pszColumnName , Int4 * pCharLength , Int2 * pIndicator );

#define TCIGetError TCIGetErrorW
TCIState TCI_API TCIGetErrorW (TCIError * pErrId , Uint4 nRecord , Uint2 nthError , wchar_t * pErrorMessage , Uint4 nMaxMessageSize , TBErrorCode * pErrorCode , wchar_t * pSqlstate );
#define TCIGetEnvironmentError TCIGetEnvironmentErrorW
TCIState TCI_API TCIGetEnvironmentErrorW (TCIEnvironment * pEnvId , Uint2 nthError , wchar_t * pErrorMessage , Uint4 nMaxMessageSize , TBErrorCode * pErrorCode , wchar_t * pSqlstate );

#define TCIGetEnvironmentAttribute TCIGetEnvironmentAttributeW
TCIState TCI_API TCIGetEnvironmentAttributeW (TCIEnvironment * pEnvId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetErrorAttribute TCIGetErrorAttributeW
TCIState TCI_API TCIGetErrorAttributeW (TCIError * pErrId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetConnectionAttribute TCIGetConnectionAttributeW
TCIState TCI_API TCIGetConnectionAttributeW (TCIConnection * pConnId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetTransactionAttribute TCIGetTransactionAttributeW
TCIState TCI_API TCIGetTransactionAttributeW (TCITransaction * pTaId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetStatementAttribute TCIGetStatementAttributeW
TCIState TCI_API TCIGetStatementAttributeW (TCIStatement * pStmtId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetStatementAttributeByName TCIGetStatementAttributeByNameW
TCIState TCI_API TCIGetStatementAttributeByNameW (TCIStatement * pStmtId , Uint2 nAttributeCode , wchar_t * pszName , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetResultSetAttribute TCIGetResultSetAttributeW
TCIState TCI_API TCIGetResultSetAttributeW (TCIResultSet * pResId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );
#define TCIGetResultSetAttributeByName TCIGetResultSetAttributeByNameW
TCIState TCI_API TCIGetResultSetAttributeByNameW (TCIResultSet * pResId , Uint2 nAttributeCode , wchar_t * pszName , void * pBuffer , Uint4 nMaxBufferSize , Uint4 * pActualSize );

#define TCISetEnvironmentAttribute TCISetEnvironmentAttributeW
TCIState TCI_API TCISetEnvironmentAttributeW (TCIEnvironment * pEnvId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetErrorAttribute TCISetErrorAttributeW
TCIState TCI_API TCISetErrorAttributeW (TCIError * pErrId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetConnectionAttribute TCISetConnectionAttributeW
TCIState TCI_API TCISetConnectionAttributeW (TCIConnection * pConnId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetTransactionAttribute TCISetTransactionAttributeW
TCIState TCI_API TCISetTransactionAttributeW (TCITransaction * pTaId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetStatementAttribute TCISetStatementAttributeW
TCIState TCI_API TCISetStatementAttributeW (TCIStatement * pStmtId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );
#define TCISetResultSetAttribute TCISetResultSetAttributeW
TCIState TCI_API TCISetResultSetAttributeW (TCIResultSet * pResId , Uint2 nAttributeCode , Uint4 nNumber , void * pBuffer );

#define TCIReleaseHeap TCIReleaseHeapW
TCIState TCI_API TCIReleaseHeapW (TCIResultSet * pResId , void * pBuffer );

#endif
#if defined(__cplusplus)
}
#endif
#endif
