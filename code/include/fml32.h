#ifndef __LIBTUX_FML32_HEADER__
#define __LIBTUX_FML32_HEADER__

#include <atmi.h>

#define MAXFBLEN32              0x7ffffffe

#define FSTDXINT                16
#define FMAXNULLSIZE            2660
#define FVIEWCACHESIZE          128
#define FVIEWNAMESIZE           33

#define FADD                    1
#define FMLMOD                  2
#define FDEL                    3

/* Flag options used in Fvstof() */
#define F_OFFSET                1
#define F_SIZE                  2
#define F_PROP                  4           /* P */
#define F_FTOS                  8           /* S */
#define F_STOF                  16          /* F */
#define F_BOTH                  (F_STOF | F_FTOS)   /* S,F */
#define F_OFF                   0           /* Z */
#define F_LENGTH                32          /* L */
#define F_COUNT                 64          /* C */
#define F_NONE                  128         /* NONE flag for null value */

/* These are used in Fstof() */
#define FUPDATE                 1
#define FCONCAT                 2
#define FJOIN                   3
#define FOJOIN                  4

/* Flags used in Fmbpack32(),Fmbupack32() */
#define FBUFENC                 1
#define FTERMENC                2

/* field types */
#define FLD_SHORT               0   /* short int */
#define FLD_LONG                1   /* long int */
#define FLD_CHAR                2   /* character */
#define FLD_FLOAT               3   /* single-precision float */
#define FLD_DOUBLE              4   /* double-precision float */
#define FLD_STRING              5   /* string - null terminated */
#define FLD_CARRAY              6   /* character array */
/* 7 reserved for FLD_INT (see viewmap.h) */
/* 8 reserved for FLD_DECIMAL (see viewmap.h) */
/* field types for FML32 only */
#define FLD_PTR                 9   /* pointer */
#define FLD_FML32               10  /* embedded FML32 buffer */
#define FLD_VIEW32              11  /* embedded VIEW32 */
#define FLD_MBSTRING            12  /* embedded multibyte codeset info */
#define FLD_FML                 13 /* for embedded ptr to FML buffer */

/* invalid field id - returned from functions where field id not found */
#define BADFLDID (FLDID32)0
/* define an invalid field id used for first call to Fnext */
#define FIRSTFLDID (FLDID32)0

/**
 * Field Error Codes
 */
#define FMINVAL                 0       /* bottom of error message codes */
#define FALIGNERR               1       /* fielded buffer not aligned */
#define FNOTFLD                 2       /* buffer not fielded */
#define FNOSPACE                3       /* no space in fielded buffer */
#define FNOTPRES                4       /* field not present */
#define FBADFLD                 5       /* unknown field number or type */
#define FTYPERR                 6       /* illegal field type */
#define FEUNIX                  7       /* unix system call error */
#define FBADNAME                8       /* unknown field name */
#define FMALLOC                 9       /* malloc failed */
#define FSYNTAX                 10      /* bad syntax in boolean expression */
#define FFTOPEN                 11      /* cannot find or open field table */
#define FFTSYNTAX               12      /* syntax error in field table */
#define FEINVAL                 13      /* invalid argument to function */
#define FBADTBL                 14      /* destructive concurrent access to field table */
#define FBADVIEW                15      /* cannot find or get view */
#define FVFSYNTAX               16      /* bad viewfile */
#define FVFOPEN                 17      /* cannot find or open viewfile */
#define FBADACM                 18      /* ACM contains negative value */
#define FNOCNAME                19      /* cname not found */
#define FEBADOP                 20      /* operation invalid for field type */
#define FMAXVAL                 21      /* top of error message codes */

typedef unsigned int FLDID32;
typedef unsigned int FLDLEN32;
typedef int FLDOCC32;
typedef void FBFR32;
#define Ferror32	(*_Fget_Ferror_addr32())
#define Ferror	Ferror32

/**
 * For Internal Implement
 */
#define FML32_BASE_FACTOR		(33554432)

#if defined(__cplusplus)
extern "C" {
#endif
	extern int*     LIBTUX_EXPORT _Fget_Ferror_addr32(void);

	extern long     LIBTUX_EXPORT Fneeded32(FLDOCC32 F, FLDLEN32 V);
	extern int      LIBTUX_EXPORT Fadd32(FBFR32 *fbfr, FLDID32 fieldid, char *value, FLDLEN32 len);
	extern int      LIBTUX_EXPORT Fchg32(FBFR32 *fbfr, FLDID32 fieldid, FLDOCC32 oc, char *value, FLDLEN32 len);
	extern int      LIBTUX_EXPORT Fget32(FBFR32 *fbfr, FLDID32 fieldid, FLDOCC32 oc, char *loc, FLDLEN32 *maxlen);
	extern FLDOCC32 LIBTUX_EXPORT Foccur32(FBFR32 *fbfr, FLDID32 fieldid);
	extern int      LIBTUX_EXPORT Fprint32(FBFR32 *fbfr);
	extern int      LIBTUX_EXPORT Fldtype32(FLDID32 fieldid);

	extern int      LIBTUX_EXPORT Finit32(FBFR32 *fbfr, FLDLEN32 buflen);

#if defined(__cplusplus)
}
#endif

/* typed-buffer types */
#define FMLTYPE32 "FML32"
#define VIEWTYPE32 "VIEW32"

#endif
