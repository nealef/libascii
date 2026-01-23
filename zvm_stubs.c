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
#pragma export(__pthread_key_delete)
#pragma export(__pthread_sigmask)
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
#pragma export(pthread_atexit_zvm)
#pragma export(pthread_atfork_zvm)
#pragma export(pthread_key_create_zvm)
#pragma export(pthread_mutex_destroy_zvm)
#pragma export(pthread_mutex_init_zvm)
#pragma export(pthread_mutex_lock_zvm)
#pragma export(pthread_mutex_unlock_zvm)
#pragma export(pthread_once_zvm)
#pragma export(pthread_setspecific_zvm)
#pragma export(pthread_mutexattr_settype_zvm)
#pragma export(pthread_mutexattr_init_zvm)
#pragma export(pthread_mutexattr_destroy_zvm)
#pragma export(pthread_getspecific_zvm)
#pragma export(pthread_key_delete_zvm)

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
#pragma map(__pthread_key_delete, "@@PT@KD")
#pragma map(__pthread_sigmask, "@@PT@SM")
#pragma map(__sched_yield, "@@SCHD@Y")
#pragma map(__setgrent, "setgrent")
#pragma map(__setgroups, "@@SETGRP")
#pragma map(__setlogmask, "\174\174SLOGM")
#pragma map(__shm_open, "shm_open")
#pragma map(__shm_unlink, "shm_unlink")
#pragma map(__syslog_a, "\174\174A00366")
#pragma map(__toCcsid, "\174\174A00125")
#pragma map(__toCSName, "\174\174A00126")
#pragma map(pthread_atexit_zvm, "pthread_atexit")
#pragma map(pthread_atfork_zvm, "pthread_atfork")
#pragma map(pthread_key_create_zvm, "\174\174PT3KC")
#pragma map(pthread_mutex_destroy_zvm, "\174\174PT3MD")
#pragma map(pthread_mutex_init_zvm, "\174\174PT3MI")
#pragma map(pthread_mutex_lock_zvm, "\174\174PT3ML")
#pragma map(pthread_mutex_unlock_zvm, "\174\174PT3MU")
#pragma map(pthread_once_zvm, "\174\174PT3O")
#pragma map(pthread_setspecific_zvm, "\174\174PT3SS")
#pragma map(pthread_mutexattr_settype_zvm, "\174\174PT3TS")
#pragma map(pthread_mutexattr_init_zvm, "\174\174PT3XI")
#pragma map(pthread_mutexattr_destroy_zvm, "\174\174PT3XS")
#pragma map(pthread_getspecific_zvm, "\174\174PT8GS")
#pragma map(pthread_key_delete_zvm, "pthread_key_delete")

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
 * @brief pthread_key_delete stub
 *
 */
int 
__pthread_key_delete(pthread_key_t key) 
{
    return 0;
}

/**
 * @brief pthread_key_delete stub
 *
 */
int 
pthread_key_delete_zvm(pthread_key_t key) 
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
 * @brief pthread atexit sub
 */
int
pthread_atexit_zvm(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    return ENOMEM;
}

/**
 * @brief pthread atfork sub
 */
int
pthread_atfork_zvm(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    return ENOMEM;
}

/**
 * @brief pthread_key_create front-end
 */
int
pthread_key_create_zvm(pthread_key_t *key, void (*destructor)(void *))
{
    return pthread_key_create(key, destructor);
}

/**
 * @brief pthread_mutext_detroy front-end
 */
int
pthread_mutex_destroy_zvm(pthread_mutex_t *mutex)
{
    return pthread_mutex_destroy(mutex);
}

/**
 * @brief pthread_mutext_init front-end
 */
int
pthread_mutex_init_zvm(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    return pthread_mutex_init(mutex, (pthread_mutexattr_t *) attr);
}

/**
 * @brief pthread_mutex_lock front-end
 */
int
pthread_mutex_lock_zvm(pthread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

/**
 * @brief pthread_mutex_unlock front-end
 */
int
pthread_mutex_unlock_zvm(pthread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

/**
 * @brief pthread_once front-end
 */
int
pthread_once_zvm(pthread_once_t *once_control, void (*init_routine)(void))
{
    return pthread_once(once_control, init_routine);
}

/**
 * @brief pthread_getspecific front-end
 */
char *
pthread_getspecific_zvm(pthread_key_t key)
{
    void *res;

    if (pthread_getspecific(key, &res) == 0)
        return (char *) res;
    else
        return NULL;
}

/**
 * @brief pthread_setspecific front-end
 */
int
pthread_setspecific_zvm(pthread_key_t key, const void *value)
{
    return pthread_setspecific(key, (void *) value);
}

/**
 * @brief pthread_mutexattr_init front-end
 */
int
pthread_mutexattr_init_zvm(pthread_mutexattr_t *attr)
{
    return pthread_mutexattr_init(attr);
}

/**
 * @brief pthread_mutexattr_destroyfront-end
 */
int
pthread_mutexattr_destroy_zvm(pthread_mutexattr_t *attr)
{
    return pthread_mutexattr_destroy(attr);
}

/**
 * @brief pthread_mutexattr_setkind_npfront-end
 */
int
pthread_mutexattr_settype_zvm(pthread_mutexattr_t *attr, int type)
{
    return pthread_mutexattr_setkind_np(attr, type);
}
