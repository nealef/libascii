/**
 * @file init_a.c
 * @brief Initialization and support routines
 * 
 * Miscallaneous routines required by ASCII/EBCDIC
 * interface code. All these routines are internal
 * use only and thus the functions are not exported.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <ctest.h>
#include <sys/utsname.h>
#include "global_a.h"
#include "envtable.h"
#include <leawi.h>
#include <ceeedcct.h>

_ENTRY hdlr = { .address = NULL };

#pragma export(__initASCIIlib_a)
#pragma export(__isVM)

char version[20]="libascii V2.0.0";
pthread_key_t *keyptr = (pthread_key_t *) NULL;
pthread_key_t key;
char athdid[5] = "ATHD";

/**
 * Prototypes
 */
ATHD_t *  __initASCIIlib_a();
void __termASCIIlib_a(void *);

/*%PAGE																*/
/**
 * @brief Returns pointer to current thread's ATHD thread
 *
 */
struct ATHD *
getathdp()
{
	int status;
	ATHD_t *atp;
	/*
	 * Call pthread_getspecific() to get the address of the current thread's
	 * ATHD structure.  If the current thread doesn't have a ATHD structure
	 * then call __initASCIIlib_a() to build one.
	 */
	if (((status = pthread_getspecific(key, (void **) &atp)) == -1)  ||
		(atp == NULL) ){
		atp = __initASCIIlib_a();
	}
	return(atp);
}

/**
 * @brief Abnormal termination handling and setup
 *
 */
static void 
handler(int sig)
{
    fprintf(stderr, "Signal Handler Invoked - signal: %d\n", sig);
    ctrace("Signal");
    _exit(1);
}

static void
abendHandler(_FEEDBACK *fc, _INT4 *token, _INT4 *result, _FEEDBACK *newfc)
{
    _FEEDBACK ufc;

    CEEHDLU(&hdlr, &ufc);

    fprintf(stderr,"sev: %d msg: %d case: %d sever: %d ctrL: %d fac: %02x %02x %02x\n",
            fc->tok_sev, fc->tok_msgno, fc->tok_case, fc->tok_sever, fc->tok_ctrl,
            fc->tok_facid[0], fc->tok_facid[1], fc->tok_facid[2]);
    fprintf(stderr,"token: %08x result: %08x\n", *token, *result);
    /*
     * If entry wasn't due to an exit() statement
     */
    if (fc->tok_msgno != 199) {
        fprintf(stderr, "Abnormal Termination Handler Invoked\n");
        cdump("abend");
        _exit(2);
     }
}

/*%PAGE																*/
/**
 * @brief Main initialization for all ASCII library routines
 *
 */
ATHD_t * 
__initASCIIlib_a()
{
	ATHD_t *atp;
	int athdsz;
    struct sigaction sa;
    struct utsname ut;
    _FEEDBACK fc;
    _INT4 token;

	/* Perform key create for process if necessary */
	if (keyptr == (pthread_key_t *) NULL) {
		keyptr = &key;
		pthread_key_create(keyptr,__termASCIIlib_a);
	}

	/* Assume the current thread doesn't have a valid athd data area. */
	athdsz = sizeof(ATHD_t);
	atp = (ATHD_t *) calloc(1,athdsz); 
    if (atp == NULL) 
        __panic_a("Error allocating thread pointer data area\n");
	if ((pthread_setspecific(key, (void *) atp) == -1) &&
	    (errno == EINVAL) ) {
		/*
		 * Pthread_setspecific failed because parm key is invalid.
		 * At this point I am not sure if this code will ever be
		 * needed.
		 */
		keyptr = &key;
		pthread_key_create(keyptr, __termASCIIlib_a);
		pthread_setspecific(key, (void *) atp);
	}		

	/* Initialize athd structure. */

	memcpy(atp->cthdeye,athdid,4); 
	atp->pid = getpid();
	atp->threadid = pthread_self();

	/* Initialize ASCII translation routines. */
	init_trans_a();	

    /* Initialize the mbstate area for mb<->wc conversions */
    memset(&atp->mb, 0, sizeof(atp->mb));

	/* Initialize ebcdic path name used my many routines. */
	atp->epathname = malloc((size_t) _POSIX_PATH_MAX);

	/* Set flag indiating athd initialization completed.  */
	atp->initdone = 1;

    /* Prepare FD translate entries for stdin/out/err */
    (void)__insertFD(fileno(stdin), NULL, 0);
    (void)__insertFD(fileno(stdout), NULL, 0);
    (void)__insertFD(fileno(stderr), NULL, 0);

    /* Prepare the environment variable handling */
    atp->envtbl = malloc(sizeof(hashTable_t));
    htInitTable(atp->envtbl);

    uname(&ut);
    atp->isVM = (strcmp(ut.sysname, "z/VM") == 0);

    if (atp->isVM) {
        virtClock_t vc;

        asm ("\tLA\t1,%0\n"
             "\tDC\tXL4'8310000C'\n"
             : "=m" (vc) : : "cc", "1");
        atp->lastVirt = vc.virtCPU;
    }

    if (getenv("LIBASCII_DEBUG") != NULL)
        atp->debug = fopen("/dev/tty", "w");
    else
        atp->debug = NULL;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        perror("sigaction");
    if (sigaction(SIGFPE, &sa, NULL) == -1)
        perror("sigaction");
    if (sigaction(SIGILL, &sa, NULL) == -1)
        perror("sigaction");
    if (sigaction(SIGABND, &sa, NULL) == -1)
        perror("sigaction");
    if (sigaction(SIGABRT, &sa, NULL) == -1)
        perror("sigaction");

    if (hdlr.address == NULL) {
        token = 0;
        hdlr.address = (_POINTER) &abendHandler;
        hdlr.nesting = NULL;

        CEEHDLR(&hdlr, &token, &fc);
        
        /* verify that CEEHDLR was successful */
        if (_FBCHECK(fc , CEE000) != 0) {
            fprintf(stderr, "CEEHDLR failed with message number %d\n", fc.tok_msgno);
            exit (2999);
        }
    }

	return(atp);
}

/**
 * @brief Thread termination routine for ASCII library.
 *
 */
void 
__termASCIIlib_a(void *inparm)
{
	ATHD_t *atp;
	/*
	 * If athd data area exists and initialization completed then
	 * perform termination.
	 */
	atp = (ATHD_t *) inparm;
	if (atp != NULL) {
		if (atp->initdone == 1) {
			atp->initdone = 0; /* just to be sure no recursive calls. */
			term_trans(atp);  /* call translation thread termination. */
			term_locale(atp); /* call locale thread termination       */
			htFreeTable(atp->envtbl);
			free(atp->epathname);
            if (atp->locale)
                free(atp->locale);
            if (atp->debug)
                fclose(atp->debug);
		}
		free(atp);     /* free athd data area for current thread */
	}
    return;  /* for now just return */
}
 
/**
 * @brief Return indicaction of whether we're running under z/VM
 *
 */
int
__isVM()
{
	int status;
	ATHD_t *atp;

	/*
	 * Call pthread_getspecific() to get the address of the current thread's
	 * ATHD structure.  If the current thread doesn't havee a ATHD structure
	 * then call __initASCIIlib_a() to build one.
	 */
	if (((status = pthread_getspecific(key, (void **) &atp)) == -1)  ||
		(atp == NULL) ){
		atp = __initASCIIlib_a();
	}
    return atp->isVM;
}

/*%PAGE																*/
/**
 * @brief Routine called when unusual condition encountered  for which there is no recovery.
 *
 */
void 
__panic_a(char *reason)
{
	int	S_errno = errno;
	int	S_errno2 = __errno2();

	fprintf(stderr, "Reason - %s\n", reason);
    fprintf(stderr, "errno: %d errno2: %d\n", S_errno, S_errno2);
    if (S_errno != 0)
        fprintf(stderr, "Error - %s (%d)\n", strerror(S_errno), S_errno);
    fflush(stderr);
	__cdump(reason);
    exit(S_errno);
}

/**
 * @brief Dump data to the debug stream
 *
 * @param title String title
 * @param buf Pointer to data to dump
 * @param len Size of data to dump
 */
void
__dump_data(char *title, void *buf, size_t len)
{
    ATHD_t *atp = athdp();

    if (atp->debug != NULL) {
        int x,y;
        char *c = (char *) buf;
        fprintf(atp->debug, "%s - %p.%x\n",
                title, c, len);
        for (x = 0; x < len;) {
            fprintf(atp->debug, "%04x ", x);
            for (y = 0; y < 16 & x < len; x++, y++)
                fprintf(atp->debug, "%02x ",c[x]);
            fputc('\n', atp->debug);
        }
    }
}

/**
 * @brief Print a message to the debug stream
 *
 * @param func Function calling this
 * @param line Line number within function
 * @param fmt Format string
 * @param len Size of data to dump
 */
void
__debug_print(const char *func, int line, char *fmt, ...)
{
    ATHD_t *atp = athdp();

    if (atp->debug != NULL) {
        va_list parg;

        fprintf(atp->debug, "%s:%d - ", func, line);
        va_start(parg, fmt);
        vfprintf(atp->debug, fmt, parg);
        va_end(parg);
        fputc('\n', atp->debug);
    }
}
/**
 * @brief Determine if a file requires translation
 *
 * @param stream File stream
 * @returns 0 if no translation is required, 1 otherwise
 */
int
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
int
__isAsciiFD(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    struct stat st;

    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd)
            return (fdxl->ascii);
    }

    return __insertFD(fd, NULL, 0);
}

/**
 * @brief Get the pathname of the assocated FD
 *
 * @param fd File descriptor
 * @returns pathname or NULL
 */
char *
__getPathname(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    struct stat st;

    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd)
            return (fdxl->path);
    }

    return NULL;
}

/**
 * @brief Indicate that a file requires translation
 *
 * @param fd File descriptor
 * @param fd Data needs to be translated on read/write
 */
void
__setAsciiFD(int fd, int trans)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    struct stat st;

    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd)
            fdxl->ascii = trans;
    }
}

/**
 * @brief Insert fd into the fdxl linked list
 *
 * @param fd File Descriptor
 */
int
__insertFD(int fd, char *path, int new)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *last = NULL;
    struct stat info;
    int tag = 0,
        mode = 1;

    if (fstat(fd, &info) == 0) {
        if (S_ISREG(info.st_mode)) {
            if (new) {
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
        if (path)
            fdxl->path = strdup(path);
        else
            fdxl->path = NULL;
        myathdp->fdxl = fdxl;
    } else {
        /*
         * Find a spot to plug us in
         */
        for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
            if (fdxl->fd == fd) {
                fdxl->ascii = tag;
                if (path)
                    fdxl->path = strdup(path);
                else
                    fdxl->path = NULL;
                return tag;
            }
            last = fdxl;
        }
        fdxl = malloc(sizeof(fdxl_t));
        fdxl->next = NULL;
        fdxl->fd = fd;
        fdxl->ascii = tag;
        last->next = fdxl;
        if (path)
            fdxl->path = strdup(path);
        else
            fdxl->path = NULL;
    }
    return tag;
}

/**
 * @brief Delete fd from the fdxl linked list
 *
 * @param fd File Descriptor
 */
void
__deleteFD(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *last = NULL;

    if (fdxl == NULL)               /* No entries */
        return;

    if (fdxl->fd == fd) {           /* Only entry */
        myathdp->fdxl = fdxl->next;
        if (fdxl->path)
            free(fdxl->path);
        free(fdxl);
        return;
    }

    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fdxl->fd == fd) {
            last->next = fdxl->next;
            if (fdxl->path)
                free(fdxl->path);
            free(fdxl);
            return;
        }
        last = fdxl;
    }
}

/**
 * @brief Update fd from the fdxl linked list
 *
 * @param newfd New File Descriptor
 * @param oldfd Old File Descriptor
 */
void
__updateFD(int newfd, int oldfd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl = myathdp->fdxl,
           *ent = NULL;

    if (fdxl == NULL)               /* No entries */
        return;

    if (fdxl->fd != oldfd) {        /* Not the only entry */
        /* Search for fd in the table */
        for (fdxl = myathdp->fdxl;
             fdxl != NULL && ent == NULL;
             fdxl = fdxl->next) {
            if (fdxl->fd == oldfd) 
                ent = fdxl;
        }
    } else {
        ent = fdxl;
    }

    if (ent != NULL) {
        __insertFD(newfd, ent->path, 0);
        __deleteFD(oldfd);
    }
}

/**
 * @brief Construct new argv or envp strings
 *
 * @param str Array of strings
 * @returns Array of strings in EBCDIC
 */

const char **
mkNew(const char **str)
{
    const char **newStr = NULL;
    int count;
    
    /*
     * Count the strings
     */
    for (count = 0; str[count] != NULL; count++);

    count++;    /* Add a spot for the NULL terminator */

    newStr = malloc(count * sizeof(uintptr_t));

    /*
     * Copy and translate the strings
     */
    for (count = 0; str[count] != NULL; count++) {
        newStr[count] = strdup(str[count]);
        __toebcdic_a((char *) newStr[count], (char *) newStr[count]);
    }
    newStr[count] = NULL;
    return newStr;
}

/**
 * @brief Free argv or envp strings
 *
 * @param str Array of strings
 */
void
freeNew(const char **str)
{
    int i;

    /*
     * Free the strings
     */
    for (i = 0; str[i] != NULL; i++)
        free((void *)str[i]);
    
    free(str);      /* Free the array */
}
