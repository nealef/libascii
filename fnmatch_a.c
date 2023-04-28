/**
 * @file fnmatch_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the fnmatch functions.
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

#include <fnmatch.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__fnmatch_a)
#endif
#pragma map(__fnmatch_a, "\174\174A00037")

/*%PAGE																*/
/**
 * @brief Match File Name or Path Name
 */
int 
__fnmatch_a(const char *pattern, const char *string, int flags)
{
	return fnmatch((const char *) __getEstring1_a(pattern), 
                   (const char *) __getEstring2_a(string), flags);
}
