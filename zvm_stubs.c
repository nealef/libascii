/**
 * @file zvm_stubs.c
 * @brief Contains missing APIs for z/VM OpenEdition
 * 
 * Notes	:	All the procedures are name "__xxxxxxxx_a" where
 *				xxxxxxxx is the name of the standard C run-time
 *				function name. Unless otherwise noted, all functions
 * 				take the same argument,produce the same output and
 *				return the same values as the standard functions.
 */

/********************************************************************
 * Copyright:   Licensed Materials - Property of IBM.               *
 *              (C) Copyright IBM Corp. 1997.                       *
 *              All rights reserved.                                *
 ********************************************************************/

#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <unistd.h>
#include <grp.h>
#include <errno.h>
#include <sys/mman.h>
#include <pthread.h>
#include <_Ccsid.h>
#include <_Nascii.h>

#include "global_a.h"

#pragma export(__ae_autoconvert_state_a)
#pragma export(__closelog)
#pragma export(__crypt_a)
#pragma export(__endgrent)
#pragma export(__getgrent_a)
#pragma export(__getrusage)
#pragma export(__mmap)
#pragma export(__msync)
#pragma export(__munmap)
#pragma export(__nice)
#pragma export(__openlog_a)
#pragma export(__sched_yield)
#pragma export(set_tag_fd_binary)
#pragma export(set_tag_fd_text)
#pragma export(set_tag_fd_text_ro)
#pragma export(__setgrent)
#pragma export(__setgroups)
#pragma export(__setlogmask)
#pragma export(__shm_open)
#pragma export(__shm_unlink)
#pragma export(__syslog_a)
#pragma export(__toCcsid)
#pragma export(__toCSName)
#pragma export(nanosleep)

#pragma map(__ae_autoconvert_state_a, "__ae_autoconvert_state")
#pragma map(__closelog, "closelog")
#pragma map(__crypt_a, "\174\174A00367")
#pragma map(__endgrent, "endgrent")
#pragma map(__getgrent_a, "\174\174A00253")
#pragma map(__getrusage, "@@GRUSE")
#pragma map(__mmap, "mmap")
#pragma map(__msync, "msync")
#pragma map(__munmap, "munmap")
#pragma map(__nice, "nice")
#pragma map(__openlog_a, "openlog")
#pragma map(__sched_yield, "@@SCHD@Y")
#pragma map(__setgrent, "setgrent")
#pragma map(__setgroups, "@@SETGRP")
#pragma map(__setlogmask, "\174\174SLOGM")
#pragma map(__shm_open, "shm_open")
#pragma map(__shm_unlink, "shm_unlink")
#pragma map(__syslog_a, "\174\174A00366")
#pragma map(__toCcsid, "\174\174A00125")
#pragma map(__toCSName, "\174\174A00126")

static int niceValue = 10;  /** simulated nice value */

typedef struct {
    __ccsid_t id;       /* Character set id */
    char      *csname;  /* Character set name */
} ccsidTable_t;

static const ccsidTable_t ccsidTable[] = {
    {819, "ISO8859-1"},
    {1047, "IBM-1047"},
    {0, NULL}
};

void
init_z_handler()
{
}
 
/*%PAGE																*/
/**
 * @brief Set/query conversion state
 */
int
__ae_autoconvert_state_a(int cmd)
{
    ATHD_t *myathdp;
    myathdp = athdp();
    int curState = myathdp->csstate;

    switch(cmd) {
    case _CVTSTATE_QUERY :
        return myathdp->csstate;
        break;
    case _CVTSTATE_OFF :
        myathdp->prvcsstate = myathdp->csstate;
    case _CVTSTATE_ON :
    case _CVTSTATE_ALL :
        myathdp->csstate = cmd;
        break;
    case _CVTSTATE_SWAP :
        if (myathdp->csstate == _CVTSTATE_OFF) {
            myathdp->csstate = myathdp->prvcsstate;
            myathdp->prvcsstate = _CVTSTATE_OFF;
        } else {
            myathdp->prvcsstate = myathdp->csstate;
            myathdp->csstate = _CVTSTATE_OFF;
        }
        break;
    default:
        errno = EINVAL;
        return -1;
    }
    return curState;
}

/**
 * @brief getgrent stub
 *
 */
struct group *
__getgrent_a()
{
    return NULL;
}

/**
 * @brief set conversion mode to binary
 */
void
set_tag_fd_binary(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd) {
            fdxl->textbin = 0;
            return;
        }
    }
}

/**
 * @brief set conversion mode to text
 */
void
set_tag_fd_text(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd) {
            fdxl->textbin = 1;
            return;
        }
    }
}

/**
 * @brief set conversion mode to text
 */
void
set_tag_fd_text_ro(int fd)
{
	ATHD_t *myathdp = athdp();
    fdxl_t *fdxl;
    for (fdxl = myathdp->fdxl; fdxl != NULL; fdxl = fdxl->next) {
        if (fd == fdxl->fd) {
            fdxl->textbin = 1;
            return;
        }
    }
}

/**
 * @brief setgrent stub
 *
 */
void
__setgrent()
{
    return;
}

/**
 * @brief endgrent stub
 *
 */
void
__endgrent()
{
    return;
}

/**
 * @brief getrusage stub
 *
 */
int 
__getrusage(int who, struct rusage *usage)
{
    memset(usage, 0, sizeof(*usage));
    return 0;
}

/**
 * @brief sched_yield implemented with pthread_yield()
 *
 */
int 
__sched_yield(void)
{
    pthread_yield(NULL);
    return 0;
}

/**
 * @brief setgroups stub
 *
 */
int 
__setgroups(size_t size, const gid_t *list)
{
    return 0;
}

/**
 * @brief Simulated nice implementation
 *
 */
int
__nice(int inc)
{
    int newNice = niceValue + inc;

    if (newNice < -19) 
        niceValue = -19;
    else if (newNice > 20)
        niceValue = 20;
    else
        niceValue = newNice;
    return niceValue;
}

/**
 * @brief openlog stub
 *
 */
void 
__openlog_a(const char *ident, int option, int facility)
{
    return;
}

/**
 * @brief syslog stub
 *
 */
void 
__syslog_a(int priority, const char *format, ...)
{
    return;
}

/**
 * @brief closelog stub
 *
 */
void
__closelog()
{
    return;
}

/**
 * @brief setlogmask stub
 *
 */
int 
__setlogmask(int mask)
{
    return 0;
}

/**
 * @brief mmap stub
 *
 */
void *
__mmap(void *addr, size_t length, int prot, int flags,
       int fd, off_t offset)
{
    void *mm = valloc(length);

    if (mm != NULL) {
        if (fd != -1) {
            ssize_t rem = length,
                    lRead;
            void *loc = mm;
            if ((lseek(fd, offset, SEEK_SET)) < 0) {
                free(mm);
                return ((void *) -1);
            }
            lRead = read(fd, mm, rem);
            while (lRead < rem) {
                if ((lRead < 0) && (errno != EINTR)) {
                    free(mm);
                    return ((void *) -1);
                } else if (lRead == 0)
                    return mm;
                else {
                    rem -= lRead;
                    loc += lRead;
                }
                lRead = read(fd, loc, rem);
            }
            return mm;
        } else
            return mm; 
    } else
        return ((void *) -1);
}

/**
 * @brief munmap stub
 *
 */
int 
__munmap(void *addr, size_t length)
{
    free(addr);
    return 0;
}

/**
 * @brief msync stub
 *
 */
int 
__msync(void *addr, size_t length, int flags)
{
    return 0;
}

/**
 * @brief shm_open stub
 *
 */
int 
__shm_open(const char *name, int oflag, mode_t mode)
{
    errno = EINVAL;
    return -1;
}

/**
 * @brief shm_unlink stub
 *
 */
int 
__shm_unlink(const char *name)
{
    errno = EINVAL;
    return -1;
}

/**
 * @brief ASCII front-end for crypt
 *
 */
char *
__crypt_a(const char *phrase, const char *setting)
{
    char *res = strdup("*****");
    __toascii_a(res, res);
    return res;
}

/**
 * @brief __toCcsid emulation
 */
__ccsid_t
__toCcsid(char *ccsname)
{
    int i;

    if (ccsname != NULL) {
        for (i = 0; ccsidTable[i].csname != NULL; i++) {
            if (strcmp(ccsname, ccsidTable[i].csname) == 0)
                return ccsidTable[i].id;
        }
    }
    errno = EINVAL;
    return 0;
}

/**
 * @brief __toCSName emulation
 */
int
__toCSName(__ccsid_t id, char *csname)
{
    int i;

    if (id != 0) {
        for (i = 0; ccsidTable[i].csname != NULL; i++) {
            if (id == ccsidTable[i].id) {
                csname = ccsidTable[i].csname;
                return 0;
            }
        }
    }
    errno = EINVAL;
    return -1;
}

/**
 * @brief Implement nanosleep via select
 */
int
nanosleep(const struct timespec *req, struct timespec *rem)
{
    struct timeval end, now, timeout;
    int64_t endUsecs, remUsecs;
    int32_t usec, sec;
    int rc;

    endUsecs = req->tv_nsec * 1000 + req->tv_sec * 1000000;
    gettimeofday(&end, NULL);
    endUsecs += end.tv_sec * 1000000 + end.tv_usec;
    timeout.tv_sec = endUsecs / 1000000;
    timeout.tv_usec = endUsecs % 1000000;
    rc = select(0, NULL, NULL, NULL, &timeout);
    while ((rc == -1) && (errno = EINTR)) {
        gettimeofday(&now, NULL);
        remUsecs = endUsecs - (now.tv_sec * 1000000 + now.tv_usec);
        timeout.tv_sec = remUsecs / 1000000;
        timeout.tv_usec = remUsecs % 1000000;
        rc = select(0, NULL, NULL, NULL, &timeout);
    }

    return rc;
}
