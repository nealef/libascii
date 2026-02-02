/**
 * @file pthread_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the UNIX03_THREAD functions
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

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "global_a.h"

#pragma export(_pthread_attr_destroy_3)
#pragma export(_pthread_attr_getdetachstate_3)
#pragma export(_pthread_attr_getstacksize_3)
#pragma export(_pthread_attr_init_3)
#pragma export(_pthread_attr_setdetachstate_3)
#pragma export(_pthread_attr_setstacksize_3)
#pragma export(_pthread_cancel_3)
#pragma export(_pthread_cond_broadcast_3)
#pragma export(_pthread_cond_destroy_3)
#pragma export(_pthread_cond_init_3)
#pragma export(_pthread_cond_signal_3)
#pragma export(_pthread_cond_timedwait_3)
#pragma export(_pthread_cond_wait_3)
#pragma export(_pthread_condattr_destroy_3)
#pragma export(_pthread_condattr_init_3)
#pragma export(_pthread_create_3)
#pragma export(_pthread_detach_3)
#pragma export(_pthread_getspecific_3)
#pragma export(_pthread_join_3)
#pragma export(_pthread_key_create_3)
#pragma export(_pthread_mutex_destroy_3)
#pragma export(_pthread_mutex_init_3)
#pragma export(_pthread_mutex_lock_3)
#pragma export(_pthread_mutex_trylock_3)
#pragma export(_pthread_mutex_unlock_3)
#pragma export(_pthread_mutexattr_destroy_3)
#pragma export(_pthread_mutexattr_init_3)
#pragma export(_pthread_once_3)
#pragma export(_pthread_setspecific_3)

#pragma map(_pthread_attr_destroy_3,           "\174\174PT3AD")
#pragma map(_pthread_attr_getdetachstate_3,    "\174\174PT3AGD")
#pragma map(_pthread_attr_getstacksize_3,      "\174\174PT3AGS")
#pragma map(_pthread_attr_init_3,              "\174\174PT3AI")
#pragma map(_pthread_attr_setdetachstate_3,    "\174\174PT3ASD")
#pragma map(_pthread_attr_setstacksize_3,      "\174\174PT3ASS")
#pragma map(_pthread_cancel_3,                 "\174\174PT3CAN")
#pragma map(_pthread_cond_broadcast_3,         "\174\174PT3CB")
#pragma map(_pthread_cond_destroy_3,           "\174\174PT3CD")
#pragma map(_pthread_cond_init_3,              "\174\174PT3CI")
#pragma map(_pthread_cond_signal_3,            "\174\174PT3CS")
#pragma map(_pthread_cond_timedwait_3,         "\174\174PT3CT")
#pragma map(_pthread_cond_wait_3,              "\174\174PT3CW")
#pragma map(_pthread_condattr_destroy_3,       "\174\174PT3DD")
#pragma map(_pthread_condattr_init_3,          "\174\174PT3DI")
#pragma map(_pthread_create_3,                 "\174\174PT3C")
#pragma map(_pthread_detach_3,                 "\174\174PT3D")
#pragma map(_pthread_getspecific_3,            "\174\174PT8GS")
#pragma map(_pthread_join_3,                   "\174\174PT3J")
#pragma map(_pthread_key_create_3,             "\174\174PT3KC")
#pragma map(_pthread_mutex_destroy_3,          "\174\174PT3MD")
#pragma map(_pthread_mutex_init_3,             "\174\174PT3MI")
#pragma map(_pthread_mutex_lock_3,             "\174\174PT3ML")
#pragma map(_pthread_mutex_trylock_3,          "\174\174PT3MT")
#pragma map(_pthread_mutex_unlock_3,           "\174\174PT3MU")
#pragma map(_pthread_mutexattr_destroy_3,      "\174\174PT3XS")
#pragma map(_pthread_mutexattr_init_3,         "\174\174PT3XI")
#pragma map(_pthread_once_3,                   "\174\174PT3O")
#pragma map(_pthread_setspecific_3,            "\174\174PT3SS")

/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_attr_destroy to OE threads
 */
 
int
_pthread_attr_destroy_3(pthread_attr_t *attr)
{
    return pthread_attr_destroy(attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_attr_getdetachstate to OE threads
 */
 
int
_pthread_attr_getdetachstate_3(pthread_attr_t *attr)
{
    return pthread_attr_getdetachstate(attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_attr_getstacksize to OE threads
 */
 
int
_pthread_attr_getstacksize_3(pthread_attr_t *attr, size_t *stacksize)
{
    return pthread_attr_getstacksize(attr, stacksize);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_attr_init to OE threads
 */
 
int
_pthread_attr_init_3(pthread_attr_t *attr)
{
    return pthread_attr_init(attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_attr_setdetachstate to OE threads
 */
 
int
_pthread_attr_setdetachstate_3(pthread_attr_t *attr, int *detachstate)
{
    return pthread_attr_setdetachstate(attr, detachstate);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_attr_setstacksize to OE threads
 */
 
int
_pthread_attr_setstacksize_3(pthread_attr_t *attr, size_t stacksize)
{
    return pthread_attr_setstacksize(attr, stacksize);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cancel to OE threads
 */
 
int
_pthread_cancel_3(pthread_t thread)
{
    return pthread_cancel(thread);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cond_broadcast to OE threads
 */
 
int
_pthread_cond_broadcast_3(pthread_cond_t *cond)
{
    return pthread_cond_broadcast(cond);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cond_destroy to OE threads
 */
 
int
_pthread_cond_destroy_3(pthread_cond_t *cond)
{
    return pthread_cond_destroy(cond);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cond_init to OE threads
 */
 
int
_pthread_cond_init_3(pthread_cond_t *cond, pthread_condattr_t *attr)
{
    return pthread_cond_init(cond, attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cond_signal to OE threads
 */
 
int
_pthread_cond_signal_3(pthread_cond_t *cond)
{
    return pthread_cond_signal(cond);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cond_timedwait to OE threads
 */
 
int
_pthread_cond_timedwait_3(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    return pthread_cond_timedwait(cond, mutex, abstime);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_cond_wait to OE threads
 */
 
int
_pthread_cond_wait_3(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    return pthread_cond_wait(cond, mutex);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_condattr_destroy to OE threads
 */
 
int
_pthread_condattr_destroy_3(pthread_condattr_t *attr)
{
    return pthread_condattr_destroy(attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_condattr_init to OE threads
 */
 
int
_pthread_condattr_init_3(pthread_condattr_t *attr)
{
    return pthread_condattr_init(attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_create to OE threads
 */
 
int
_pthread_create_3(pthread_t *thread, pthread_attr_t *attr,
                  void * (*start_routine) (void *), void *arg)
{
    return pthread_create(thread, attr, start_routine, arg);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_detach to OE threads
 */
 
int
_pthread_detach_3(pthread_t *thread)
{
    return pthread_detach(thread);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_getspecific to OE threads
 */
 
int
_pthread_getspecific_3(pthread_key_t key, void **value)
{
    return pthread_getspecific(key, value);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_join to OE threads
 */
 
int
_pthread_join_3(pthread_t thread, void **status)
{
    return pthread_join(thread, status);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_key_create to OE threads
 */
 
int
_pthread_key_create_3(pthread_key_t *key, void (*destructor) (void *))
{
    return pthread_key_create(key, destructor);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutex_destroy to OE threads
 */
 
int
_pthread_mutex_destroy_3(pthread_mutex_t *mutex)
{
    return pthread_mutex_destroy(mutex);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutex_init to OE threads
 */
 
int
_pthread_mutex_init_3(pthread_mutex_t *mutex, pthread_mutexattr_t *attr)
{
    return pthread_mutex_init(mutex, attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutex_lock to OE threads
 */
 
int
_pthread_mutex_lock_3(pthread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutex_trylock to OE threads
 */
 
int
_pthread_mutex_trylock_3(pthread_mutex_t *mutex)
{
    return pthread_mutex_trylock(mutex);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutex_unlock to OE threads
 */
 
int
_pthread_mutex_unlock_3(pthread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutexattr_destroy  to OE threads
 */
 
int
_pthread_mutexattr_destroy_3(pthread_mutexattr_t *attr)
{
    return pthread_mutexattr_destroy (attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_mutexattr_init to OE threads
 */
 
int
_pthread_mutexattr_init_3(pthread_mutexattr_t *attr)
{
    return pthread_mutexattr_init(attr);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_once to OE threads
 */
 
int
_pthread_once_3(pthread_once_t *once_control, void(*init_routine) ())
{
    return pthread_once(once_control, init_routine);
}
 
/*%PAGE                                           */
/**
 * @brief Map UNIX03_THREAD pthread_setspecific to OE threads
 */
 
int
_pthread_setspecific_3(pthread_key_t key, void *value)
{
    return pthread_setspecific(key, value);
}
