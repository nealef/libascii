/**
 * @file global_a.h
 * @brief Define global structures for the ASCII library.
 *
 */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iconv.h>
#include <locale.h>
#include <wchar.h>
#include <sys/stat.h>
#include <sys/modes.h>
#include "_Ascii_a.h"
#include "envtable.h"

/** 
 * Define the macro used to obtain the pointer to the
 * ASCII library thread structure athd.   
 */
#define athdp() ((struct ATHD *) getathdp())

#define MAXSTRING_a 2048      /* Increase for large printf, sprintf, etc. */

/**
 * We use the audit field of the stat structure to determine
 * the type of translation required. This applies to IS_REG()
 * or IS_LINK() files. 
 */
const static int iso8859 = AUDTREADFAIL  | AUDTREADSUCC  |
                           AUDTWRITEFAIL | AUDTWRITESUCC |
                           AUDTEXECFAIL;                  

const static int ibm1047 = AUDTREADFAIL  |                
                           AUDTWRITEFAIL |                
                           AUDTEXECFAIL;                  

const static int binary  = AUDTREADFAIL  |                
                           AUDTWRITEFAIL |                
                           AUDTEXECFAIL  | AUDTEXECSUCC;  

const static int mixAsc  = AUDTREADFAIL  | AUDTREADSUCC  |
                           AUDTWRITEFAIL |                
                           AUDTEXECFAIL  | AUDTEXECSUCC;  

const static int mixEbc  = AUDTREADFAIL  |               
                           AUDTWRITEFAIL | AUDTWRITESUCC |
                           AUDTEXECFAIL  | AUDTEXECSUCC;  

/**
 * @brief Ordered linked list of file descriptors with translation information
 */
struct FDXL {
    void *next;             /* Next entry in list */
    int  fd;                /* File descriptor */
    int  ascii;             /* Translation required flag */
    int  textbin;           /* Text/Binary mode */
    char *path;             /* Pathname (if applicable) */
};

typedef struct FDXL fdxl_t;

/**
 * @brief Wide and multibyte support
 */
typedef enum {
    ENCODING_AUTO_DETECT = 0,
    ENCODING_UTF8,
    ENCODING_UTF16_BE,
    ENCODING_UTF16_LE,
    ENCODING_UTF32_BE,
    ENCODING_UTF32_LE,
    ENCODING_ASCII_EXTENDED
} unicodeEncoding_t;

/**
 * @brief Preserves internal state across restartable calls.
 */
typedef struct {
    wchar_t pendingLowSurrogate;        // Stores low surrogate for 2-pass wchar_t extraction
    wchar_t pendingHighSurrogate;       // Stores high surrogate for 2-pass wchar_t encoding
    unicodeEncoding_t resolvedEncoding; // Holds auto-detected encoding mode
} mbstate_a_t;

/**
 * @brief Structure ATHD defines the thread specific data used by ASCII library.
 */
struct ATHD {
	char         cthdeye[4];   /* athd eye catcher                    */
	int          initdone;     /* athd data area  initialization complete   */
	pid_t        pid;          /* process id                          */
	pthread_t    threadid;     /* thread id                           */
    clock_t      lastVirt;     /* Last virtual CPU from diag 0x0c     */
	iconv_t      cd_EtoA;      /* EBCDIC to ASCII iconv descriptor    */
	iconv_t      cd_AtoE;      /* ASCII to EBCDIC iconv descriptor    */
    mbstate_a_t  mb;           /* mb<->wc conversions                 */
    void         *locale;      /* Table of locale settings            */
    FILE         *debug;       /* Debug file                          */

	char         *epathname;   /* ebcdic path name                    */
	char         *astring1_a;  /* ascii string returned by __getAstring1_a */
	char         *astring2_a;  /* ascii string returned by __getAstring2_a */
	char         *estring1_a;  /* ebcdic string returned by __getEstring1_a */
	char         *estring2_a;  /* ebcdic string returned by __getEstring2_a */
	char         *estring3_a;  /* ebcdic string returned by __getEstring3_a */
	char         *estring4_a;  /* ebcdic string returned by __getEstring4_a */
	wchar_t      *awstring1_a; /* ascii string returned by __getAwstring1_a */
	wchar_t      *awstring2_a; /* ascii string returned by __getAwstring2_a */
	wchar_t      *ewstring1_a; /* ebcdic string returned by __getEwstring1_a */
	wchar_t      *ewstring2_a; /* ebcdic string returned by __getEwstring2_a */
	wchar_t      *ewstring3_a; /* ebcdic string returned by __getEwstring3_a */
	wchar_t      *ewstring4_a; /* ebcdic string returned by __getEwstring4_a */
	struct lconv *elconv_a;    /* Ptr to local copy of lconv         */
    int          csstate;      /* ccsid conversion state */
    int          prvcsstate;   /* Previous ccsid conversion state */
    fdxl_t       *fdxl;        /* root of fd translation lookup table */
    hashTable_t  *envtbl;      /* Hash table for caching ASCII versions of env vars */
    int          isVM;         /* Running under z/VM or not */
};

typedef struct ATHD ATHD_t;

/**
 * @brief DIAGNOSE 0x0c returned structure
 */
typedef struct {
    char     date[8];
    char     time[8];
    uint64_t virtCPU;
    uint64_t totalCPU;
} virtClock_t __attribute__ ((aligned (8)));

/*
 * Debug MACROs
 */
#ifdef __LIBASCII_DEBUG
void __dump__data(char *, void *, size_t);
void __debug_print(const char *, int, char *, ...);
# define DUMP_DATA(t, d, l) __dump_data(t, d, l)

# define DEBUG_PRINT(fmt, ...) __debug_print(__func__, __LINE__, fmt, __VA_ARGS__)

#else
# define DUMP_DATA(t, d, l)
# define DEBUG_PRINT(fmt, ...) 
#endif

/**
 * @brief Utility routine prototypes
 */
int __isAsciiStream(FILE *);
int __isAsciiFD(int);
char * __getPathname(int);
void __setAsciiFD(int, int);
int __insertFD(int, char *, int);
void __deleteFD(int);
void __updateFD(int, int);
const char ** mkNew(const char **);
void freeNew(const char **);



