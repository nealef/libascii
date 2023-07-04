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
#include <sys/stat.h>
#include "_Ascii_a.h"

 /** 
  * Define the macro used to obtain the pointer to the
  * ASCII library thread structure athd.   
  */
 #define athdp() ((struct ATHD *) getathdp())

 #define MAXSTRING_a 2048      /* Increase for large printf, sprintf, etc. */

 #define DUMP_DATA(t, d, l) do {                        \
        int x,y;		                                \
        char *c = (char *) (d);		                    \
        printf("%s -\n",t);		                        \
        for (x = 0; x < (l);) {		                    \
            for (y = 0; y < 16 & x < (l); x++, y++)		\
                printf("%02x ",c[x]);		            \
            printf("\n");		                        \
        }		                                        \
    } while (0)

/**
 * We use the audit field of the stat structure to determine
 * the type of translation required. This applies to IS_REG()
 * or IS_LINK() files. 
 */
const static int iso8859 = AUDTREADFAIL  | AUDTREADSUCC  | 
                           AUDTWRITEFAIL | AUDTWRITESUCC | 
                           AUDTEXECFAIL  | AUDTEXECSUCC;   
                                                           
const static int ibm1047 = AUDTREADFAIL  |                 
                           AUDTWRITEFAIL |                 
                           AUDTEXECFAIL;                   
                                                           
const static int binary  = AUDTREADFAIL  |                 
                           AUDTWRITEFAIL | AUDTWRITESUCC | 
                           AUDTEXECFAIL  | AUDTEXECSUCC;   
                                                           
const static int mixAsc  = AUDTREADFAIL  | AUDTREADSUCC  | 
                           AUDTWRITEFAIL |                 
                           AUDTEXECFAIL  | AUDTEXECSUCC;   
                                                           
const static int mixEbc  = AUDTREADFAIL  | AUDTREADSUCC  | 
                           AUDTWRITEFAIL | AUDTWRITESUCC | 
                           AUDTEXECFAIL;                   

/**
* @brief Ordered linked list of file descriptors with translation information
*/
struct FDXL {
    void *next;             /* Next entry in list */
    int  fd;                /* File descriptor */
    int  ascii;             /* Translation required flag */
};

typedef struct FDXL fdxl_t;

/**
 * @brief Structure ATHD defines the thread specific data used by ASCII library.
 */
struct ATHD {
	char         cthdeye[4];   /* athd eye catcher                    */
	pid_t        pid;          /* process id                          */
	pthread_t    threadid;     /* thread id                           */
	iconv_t      cd_EtoA;      /* EBCDIC to ASCII iconv descriptor    */
	iconv_t      cd_AtoE;      /* ASCII to EBCDIC iconv descriptor    */
	void         *getenvp;     /* getenv return buffer                */
	int          getenvlen;    /* length of getenv return buffer      */

	char         *epathname;   /* ebcdic path name                    */
	int          initdone;     /* athd data area  initialization complete   */
	char         *estring1_a;  /* ebcdic string returned by __getEstring1_a */
	char         *estring2_a;  /* ebcdic string returned by __getEstring2_a */
	char         *estring3_a;  /* ebcdic string returned by __getEstring3_a */
	char         *estring4_a;  /* ebcdic string returned by __getEstring4_a */
	struct lconv *elconv_a;    /* Ptr to local copy of lconv         */
    fdxl_t       *fdxl;        /* root of fd translation lookup table */
};

typedef struct ATHD ATHD_t;

/**
 * @brief Determine if a file requires translation
 *
 * @param stream File stream
 * @returns 0 if no translation is required, 1 otherwise
 */
#pragma inline (__isAsciiStream)
static int
__isAsciiStream(FILE *stream)
{
    return (stream->__fp->__fcb_ascii);
}

/**
 * @brief Determine if a file requires translation
 *
 * @param fd File descriptor
 * @returns 0 if no translation is required, 1 otherwise
 */
#pragma inline (__isAsciiFD)
static int
__isAsciiFD(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd)
            return (fdxl->ascii);
    }
    return (1);
}

/**
 * @brief Insert fd into the fdxl linked list
 *
 * @param fd File Descriptor
 */
#pragma inline (__insertFD)
static void
__insertFD(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *last = NULL;
    struct stat info;
    int tag = 1;

    if (fstat(fd, &info) == 0) {
        if ((info.st_useraudit == iso8859) || 
            (info.st_useraudit == mixAsc)) 
            tag = 0;
    }

    if (fdxl == NULL) {
        fdxl = malloc(sizeof(fdxl_t));
        fdxl->next = NULL;
        fdxl->fd = fd;
        fdxl->ascii = tag;
    } else {
        for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
            last = fdxl;
        }
        fdxl = malloc(sizeof(fdxl_t));
        fdxl->next = NULL;
        fdxl->fd = fd;
        fdxl->ascii = tag;
        last->next = fdxl;
    }
}

/**
 * @brief Delete fd from the fdxl linked list
 *
 * @param fd File Descriptor
 */
#pragma inline (__deleteFD)
static void
__deleteFD(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *last = NULL;

    if (fdxl == NULL)               /* No entries */
        return;

    if (fdxl->fd == fd) {           /* Only entry */
        myathdp->fdxl = NULL;
        free(fdxl);
        return;
    }

    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fdxl->fd == fd) {
            last = fdxl;
            last->next = fdxl->next;
            free(fdxl);
        }
    }
}
