/**
 * @file utime_a.h
 * @brief Contains ASCII-to-EBCDIC front end for the utime functions.
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

#include <utime.h>
#include "global_a.h"
 
#pragma export(__utime_a)
#pragma export(__utimes_a)
 
#pragma map(__utime_a, "\174\174A00328")
#pragma map(__utimes_a, "\174\174A00297")

/*%PAGE																*/
/**
 * ASCII front-end routines for UTIME functions
 */
 
/**
 * @brief Set file access and modification times
 */
int 
__utime_a(const char *path, const struct utimbuf *times)
{
	return utime((const char *) __getEstring1_a(path), times);
}
 
/**
 * @brief Set file access and modification times
 */
int 
__utimes_a(const char *path, const struct timeval *times)
{
	return utimes((const char *) __getEstring1_a(path), times);
}
