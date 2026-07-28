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
	 * ATHD structure.  If the current thread doesn't havee a ATHD structure
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
handler(int sig, siginfo_t *si, void *unused)
{
    fprintf(stderr, "Signal Handler Invoked\n");
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
	    ( errno == EINVAL) ) {
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

	/* Initialize ebcdic path name used my many routines. */
	atp->epathname = malloc((size_t) _POSIX_PATH_MAX);

	/* Set flag indiating athd initialization completed.  */
	atp->initdone = 1;

    /* Prepare FD translate entries for stdin/out/err */
    (void)__insertFD(fileno(stdin), NULL);
    (void)__insertFD(fileno(stdout), NULL);
    (void)__insertFD(fileno(stderr), NULL);

    /* Prepare the environment variable handling */
    atp->envtbl = malloc(sizeof(hashTable_t));
    htInitTable(atp->envtbl);

    uname(&ut);
    atp->isVM = (strcmp(ut.sysname, "z/VM") == 0);

    memset(&sa, 0, sizeof(sa));

    sa.sa_sigaction = handler;
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
	 * If athd data area exists and initializatione completed then
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
