/**
 * @file inet_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the arpa/inet.h functions.
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

#include <arpa/inet.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__inet_addr_a)
#pragma export(__inet_ntoa_a)
#endif

#pragma map(__inet_addr_a, "\174\174A00401")
#pragma map(__inet_ntoa_a, "\174\174A00402")

/*%PAGE																*/
/**
 * @brief ASCII front-end for inet_addr 
 *
 */

in_addr_t 
__inet_addr_a(const char *cp)
{
	return inet_addr((const char *) __getEstring1_a(cp));
}

/*%PAGE																*/
/**
 * @brief ASCII front-end for inet_addr 
 *
 */

char *
__inet_ntoa_a(struct in_addr in)
{
	return __getAstring1_a(inet_ntoa(in));
}
