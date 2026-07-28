/**
 * @file pwd_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the pwd.h functions.
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

#define _XOPEN_SOURCE_EXTENDED 1
#include <pwd.h>
#include "global_a.h"

#pragma export(__getpwent_a)
#pragma export(__getpwnam_a)
#pragma export(__getpwnam_r_a)
#pragma export(__getpwuid_a)
#pragma export(__getpwuid_r_a)

#ifdef __VM__
#pragma export(setpwent)
#pragma export(endpwent)
#endif

#pragma map(__getpwent_a, "\174\174A00264")
#pragma map(__getpwnam_a, "\174\174A00265")
#pragma map(__getpwnam_r_a,  "\174\174A00031")
#pragma map(__getpwuid_a, "\174\174A00266")
#pragma map(__getpwuid_r_a, "\174\174A00032")

/*%PAGE																*/
/**
 * @brief Get next password entry
 */
struct passwd *
__getpwent_a(void)
{
#ifndef __VM__
	struct passwd *p;

	p = getpwent();
	if (p != NULL) {
		__toascii_a(p->pw_name,p->pw_name);
		__toascii_a(p->pw_dir,p->pw_dir);
		__toascii_a(p->pw_shell,p->pw_shell);
		return(p);
	} else
		return(NULL);
#else
    return (NULL);
#endif
}

/**
 * @brief Get password entry by name
 */
struct passwd *
__getpwnam_a(const char *name) 
{
	struct passwd *p;

	p = getpwnam((const char *) __getEstring1_a(name));
	if (p != NULL) {
		__toascii_a(p->pw_name,p->pw_name);
		__toascii_a(p->pw_dir,p->pw_dir);
		__toascii_a(p->pw_shell,p->pw_shell);
		return(p);
	} else
		return(NULL);
}

/**
 * @brief Get password entry by name - re-entrant
 */
struct passwd *
__getpwnam_r_a(const char *name) 
{
	struct passwd *p, *ap;

	p = getpwnam((const char *) __getEstring1_a(name));
	if (p != NULL) { 
        ap = malloc(sizeof(*p));
        if (ap != NULL) {
            ap->pw_name = strdup(p->pw_name);
            ap->pw_dir = strdup(p->pw_dir);
            ap->pw_shell = strdup(p->pw_shell);
            __toascii_a(ap->pw_name,ap->pw_name);
            __toascii_a(ap->pw_dir,ap->pw_dir);
            __toascii_a(ap->pw_shell,ap->pw_shell);
        }
		return(ap);
	} else
		return(NULL);
}

/**
 * @brief Get password entry by uid
 */
struct passwd *
__getpwuid_a(uid_t uid) 
{
	struct passwd  * p;

	p = getpwuid(uid);
	if (p != NULL) {
		__toascii_a(p->pw_name,p->pw_name);
		__toascii_a(p->pw_dir,p->pw_dir);
		__toascii_a(p->pw_shell,p->pw_shell);
		return(p);
	} else
		return(NULL);
}

/**
 * @brief Get password entry by uid - re-entrant
 */
struct passwd *
__getpwuid_r_a(uid_t uid) 
{
	struct passwd *p, *ap;
    char *cp;

	p = getpwuid(uid);
	if (p != NULL) {
        ap = malloc(sizeof(*p));
        if (ap != NULL) {
            ap->pw_name = strdup(p->pw_name);
            ap->pw_dir = strdup(p->pw_dir);
            ap->pw_shell = strdup(p->pw_shell);
            __toascii_a(ap->pw_name, ap->pw_name);
            __toascii_a(ap->pw_dir, ap->pw_dir);
            __toascii_a(ap->pw_shell, ap->pw_shell);
        }
		return(ap);
	} else
		return(NULL);
}

#ifdef __VM__
void
setpwent()
{
}

void
endpwent()
{
}
#endif
