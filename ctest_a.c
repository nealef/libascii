/**
 * @file ctest_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the ctest functions.
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

#include <ctest.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__cdump_a)
#pragma export(__ctrace_a)
#pragma export(__csnap_a)
#endif

#pragma map(__cdump_a, "\174\174A00360")
#pragma map(__csnap_a, "\174\174A00361")
#pragma map(__ctrace_a, "\174\174A00363")

/*%PAGE																*/
/**
 * @brief Produce dump of the process
 */
int 
__cdump_a(char *s)
{
	return (cdump(__getEstring1_a(s)));
}

/**
 * @brief Produce condensed dump of the process
 */
int
__csnap_a(char *s)
{
	return (csnap(__getEstring1_a(s)));
}

/**
 * @brief Produce strack trace of the process
 */
int
__ctrace_a(char *s)
{
	return (ctrace(__getEstring1_a(s)));
}
