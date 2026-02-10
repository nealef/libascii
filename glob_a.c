/**
 * @file glob_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the glob functions.
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

#pragma langlvl( extended )
 
#include <glob.h>
#include "global_a.h"

#pragma export(__glob_a)

#pragma map(__glob_a,   "\174\174A00378")

static int __globerr_a(const char *, int);
int (*usrerr) (const char *, int);
 
/*%PAGE																*/
/**
 * @brief Generate Path names Matching a Pattern
 */
int 
__glob_a(const char *pattern, int flags, int (*errfunc)(const char *, int), glob_t *pglob)
{
    usrerr = errfunc;
	return glob((const char *) __getEstring1_a(pattern), flags, __globerr_a, pglob);
}

/**
 * @brief Handle error from glob()
 */
static int
__globerr_a(const char *path, int err)
{
    return usrerr(__getEstring2_a(path), err);
}

