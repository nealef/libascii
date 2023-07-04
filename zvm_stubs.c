/**
 * @file zvm_stubs.c
 * @brief Contains missing APIs for z/VM OpenEdition
 * 
 * Compile	:	GEN_PRAGMA_EXPORT - generate PRAGMA statements to
 * Options						export these entry points from the
 *								DLL								
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

#include <syslog.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <unistd.h>
#include <grp.h>
#include <errno.h>
#include <sys/mman.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__crypt_a)
#pragma export(__getgrent_a)
#pragma export(__setgrent)
#pragma export(__endgrent)
#pragma export(__getrusage)
#pragma export(__sched_yield)
#pragma export(__setgroups)
#pragma export(__nice)
#pragma export(__openlog_a)
#pragma export(__syslog_a)
#pragma export(__closelog)
#pragma export(__setlogmask)
#pragma export(__pthread_key_delete)
#pragma export(__pthread_sigmask)
#pragma export(__mmap)
#pragma export(__msync)
#pragma export(__munmap)
#pragma export(__shm_open)
#pragma export(__shm_unlink)
#endif

#pragma map(__crypt_a, "\174\174A00367")
#pragma map(__getgrent_a, "\174\174A00253")
#pragma map(__setgrent, "setgrent")
#pragma map(__endgrent, "endgrent")
#pragma map(__getrusage, "getrusage")
#pragma map(__sched_yield, "sched_yield")
#pragma map(__setgroups, "setgroups")
#pragma map(__nice, "nice")
#pragma map(__openlog_a, "openlog")
#pragma map(__syslog_a, "\174\174A00366")
#pragma map(__closelog, "closelog")
#pragma map(__setlogmask, "setlogmask")
#pragma map(__pthread_key_delete, "pthread_key_delete")
#pragma map(__pthread_sigmask, "pthread_sigmask")
#pragma map(__mmap, "mmap")
#pragma map(__msync, "msync")
#pragma map(__munmap, "munmap")
#pragma map(__shm_open, "shm_open")
#pragma map(__shm_unlink, "shm_unlink")

static int niceValue = 10;  /** simulated nice value */

/*%PAGE																*/
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
 * @brief pthread_key_delete stub
 *
 */
int 
__pthread_key_delete(pthread_key_t key) 
{
    return 0;
}

/**
 * @brief pthread_sigmask stub
 *
 */
int
__pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
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
    errno = EINVAL;
    return ((void *) -1);
}

/**
 * @brief munmap stub
 *
 */
int 
__munmap(void *addr, size_t length)
{
    errno = EINVAL;
    return -1;
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
