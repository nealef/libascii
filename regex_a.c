/**
 * @file regex_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the regex functions.
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

#pragma langlvl( extended )
 
#include <regex.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__regcomp_a)
#endif
 
/*%PAGE																*/
/**
 * @brief Compile a regular expression
 */
int 
__regcomp_a(regex_t *preg, const char *pattern, int cflags)
{
	return regcomp(preg, (const char *) __getEstring1_a(pattern), cflags);
}
