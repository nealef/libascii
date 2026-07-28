/**
 * @file zvm_stubs.c
 * @brief Contains missing APIs for z/VM OpenEdition
 * 
 * Notes	:	All the procedures are name "__xxxxxxxx" where
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

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <grp.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <_Ccsid.h>
#include "global_a.h"

#pragma export(__closelog)
#pragma export(__crypt)
#pragma export(__endgrent)
#pragma export(__getgrent)
#pragma export(__getlogin2)
#pragma export(__getrusage)
#pragma export(__mmap)
#pragma export(__msync)
#pragma export(__munmap)
#pragma export(__nice)
#pragma export(__openlog)
#pragma export(__sched_yield)
#pragma export(__setgrent)
#pragma export(__setgroups)
#pragma export(__setlogmask)
#pragma export(__shm_open)
#pragma export(__shm_unlink)
#pragma export(__mysyslog)
#pragma export(__toCcsid)
#pragma export(__toCSName)
#pragma export(nanosleep)
#pragma export(__select_ovr)
#pragma export(modf)
#pragma export(modfl)
#pragma export(futimes)

#pragma map(__closelog, "closelog")
#pragma map(__crypt, "\174\174A00367")
#pragma map(__endgrent, "endgrent")
#pragma map(__getgrent, "\174\174A00253")
#pragma map(__getlogin2, "\174\174GETLG2")
#pragma map(__getrusage, "@@GRUSE")
#pragma map(__mmap, "mmap")
#pragma map(__msync, "msync")
#pragma map(__munmap, "munmap")
#pragma map(__nice, "nice")
#pragma map(__openlog, "openlog")
#pragma map(__sched_yield, "@@SCHD@Y")
#pragma map(__setgrent, "setgrent")
#pragma map(__setgroups, "@@SETGRP")
#pragma map(__setlogmask, "setlogmask")
#pragma map(__shm_open, "shm_open")
#pragma map(__shm_unlink, "shm_unlink")
#pragma map(__mysyslog, "syslog")
#pragma map(__toCcsid, "\174\174A00125")
#pragma map(__toCSName, "\174\174A00126")
#pragma map(__select_ovr, "SLCTOVRA")
#pragma map(__futimes, "futimes")

void *valloc(size_t);

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
 * @brief getgrent stub
 *
 */
struct group *
__getgrent()
{
    return NULL;
}

/**
 * @brief getlogin2 stub
 *
 */
char *
__getlogin2(char *buf)
{
    return strcpy(buf, getlogin());
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
__openlog(const char *ident, int option, int facility)
{
    return;
}

/**
 * @brief syslog stub
 *
 */
void 
__mysyslog(int priority, const char *format, ...)
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
__crypt(const char *phrase, const char *setting)
{
    char *res = strdup("*****");
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

/**
 * @brief Front-end for select() to handle FIFOs which CMS doesn't
 */
int
__select_ovr(int max, fd_set *r, fd_set *w, fd_set *x, struct timeval *t)
{
    int fd,
        nmax = 0,
        found = 0,
        res;
    fd_set *rr = (r != NULL ? __alloca(sizeof(fd_set)) : NULL),
           *ww = (w != NULL ? __alloca(sizeof(fd_set)) : NULL),
           *xx = (x != NULL ? __alloca(sizeof(fd_set)) : NULL);

    /**
     * Search for FIFOs in the sets. Set shadows for non-FIFOS
     */
    for (fd = 0; fd < max; fd++) {
        struct stat st;
        int flags;

        if ((r != NULL) && (FD_ISSET(fd, r))) {
            fstat(fd, &st);
            if (S_ISFIFO(st.st_mode)) {
                found++;
                flags = fcntl(fd, F_GETFL);
                fcntl(fd, F_SETFL, (flags & ~O_NONBLOCK));
            } else
                FD_SET(fd, rr);
        } 

        if ((w != NULL) && (FD_ISSET(fd, w))) {
            fstat(fd, &st);
            if (S_ISFIFO(st.st_mode))
                found++;
            else
                FD_SET(fd, ww);
        } 
        
        if ((x != NULL) && (FD_ISSET(fd, x))) {
            fstat(fd, &st);
            if (S_ISFIFO(st.st_mode))
                found++;
            else
                FD_SET(fd, xx);
        }
    }

    /**
     * If no FIFOs were found then just run the select as is
     */
    if (!found) {
        return select(max, r, w, x, t);
    } else {
        /**
         * Otherwise construct a new select() for any non-FIFOs
         */
        for (fd = 0; fd < max; fd++) {

            if ((rr) && (FD_ISSET(fd, rr)))
                if (fd > (nmax + 1))
                    nmax = fd + 1;

            if ((ww) && (FD_ISSET(fd, ww)))
                if (fd > (nmax + 1))
                    nmax = fd + 1;

            if ((xx) && (FD_ISSET(fd, xx)))
                if (fd > (nmax + 1))
                    nmax = fd + 1;
        }

        /**
         * If non-FIFOs found then run a select() with an (almost) immediate
         * return just so any fds that were ready are captured
         */
        if (nmax > 0) {
            struct timeval tt;

            tt.tv_sec = 0;
            tt.tv_usec = 1;
            res = select(nmax, rr, ww, xx, &tt);
            if (res > 0) {
                /**
                 * Update the real fd_sets with the results of the
                 * shadow select
                 */
                for (fd = 0; fd < nmax; fd++) {
                    if ((rr) && FD_ISSET(fd, rr))
                        FD_SET(fd, r);
                    if ((ww) && FD_ISSET(fd, ww))
                        FD_SET(fd, w);
                    if ((xx) && FD_ISSET(fd, xx))
                        FD_SET(fd, x);
                }
                res += found;
            }
        } else
            res = found;

        return res;
    }
}

/**
 * @brief Implement modf() API
 */
double 
modf(double x, double *iptr) 
{
    // fmod(x, 1.0) returns the remainder of x / 1.0.
    // This is mathematically equivalent to the fractional part of x.
    double fractional_part = fmod(x, 1.0);
    
    // The integral part is simply the original number minus the fractional part.
    if (iptr) {
        *iptr = x - fractional_part;
    }
    
    return fractional_part;
}

/**
 * @brief Implement modfl() API
 */
long double 
modfl(long double x, long double *iptr) 
{
    // fmodl(x, 1.0) returns the remainder of x / 1.0.
    // This is mathematically equivalent to the fractional part of x.
    long double fractional_part = fmodl(x, 1.0);
    
    // The integral part is simply the original number minus the fractional part.
    if (iptr) {
        *iptr = x - fractional_part;
    }
    
    return fractional_part;
}

/**
 * @brief Implement futimes() API
 */
int 
__futimes(int fd, const struct timeval tv[2])
{
    FILE *stream;
    fldata_t fileInfo;
    char fileName[FILENAME_MAX];
    int rc = -1, f;

    memset(fileName, 0, sizeof(fileName));
    if ((f = dup(fd)) != -1) {
        stream = fdopen(f, "r");
        if (stream != NULL) {
            if (fldata(stream, fileName, &fileInfo) == 0) {
                if (fileName[0] != 0) {
                    rc = utimes(fileName, tv);
                } else {
                    errno = EBADF;
                }
            }
            fclose(stream);
        }
        close(f);
    }
    return rc;
}
