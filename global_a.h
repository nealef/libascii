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

#define DUMP_DATA(t, d, l) do {                         \
       int x,y;		                                    \
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
    int          csstate;      /* ccsid conversion state */
    int          prvcsstate;   /* Previous ccsid conversion state */
    fdxl_t       *fdxl;        /* root of fd translation lookup table */
};

typedef struct ATHD ATHD_t;

/**
 * @brief Determine if a file requires translation
 *
 * @param stream File stream
 * @returns 0 if no translation is required, 1 otherwise
 */
static inline int
__isAsciiStream(FILE *stream)
{
	return __isAsciiFD(fileno(stream));
}

/**
 * @brief Determine if a file requires translation
 *
 * @param fd File descriptor
 * @returns 0 if no translation is required, 1 otherwise
 */
static inline int
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
static inline void
__insertFD(int fd, char *path)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *last = NULL;
    struct stat info;
    int tag = 0,
        mode = 1;

    if (fstat(fd, &info) == 0) {
        if (S_ISREG(info.st_mode)) {
            if (path != NULL) {
                tag = 1;
                chaudit(path, iso8859, AUDT_USER);
            } else {
                if ((info.st_useraudit == iso8859) || 
                    (info.st_useraudit == mixAsc) ||
                    (info.st_useraudit == binary))
                    tag = 1;
                else
                    tag = 0;
            }
        } else {
            if (S_ISSOCK(info.st_mode))
                tag = 1;
            else 
                tag = 0;
        }
    }

    /*
     * If this is the first
     */
    if (fdxl == NULL) {
        fdxl = malloc(sizeof(fdxl_t));
        fdxl->next = NULL;
        fdxl->fd = fd;
        fdxl->ascii = tag;
        myathdp->fdxl = fdxl;
    } else {
        /*
         * Find a spot to plug us in
         */
        for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
            if (fdxl->fd == fd) {
                fdxl->ascii = tag;
                return;
            }
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
static inline void
__deleteFD(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *last = NULL;

    if (fdxl == NULL)               /* No entries */
        return;

    if (fdxl->fd == fd) {           /* Only entry */
        myathdp->fdxl = fdxl->next;
        free(fdxl);
        return;
    }

    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fdxl->fd == fd) {
            last->next = fdxl->next;
            free(fdxl);
            return;
        }
        last = fdxl;
    }
}
