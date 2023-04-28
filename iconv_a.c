/**
 * @file iconv_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the iconv functions.
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

#include <iconv.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__iconv_open_a)
#endif
 
/*%PAGE																*/
/**
 * @brief Invoke iconv_open after parameter conversion
 *
 * @param tocode Codepage to convert to
 * @param fromcode Codepage to convert from
 */
 
iconv_t 
__iconv_open_a(const char *tocode, const char *fromcode)
{
	return iconv_open((const char *) __getEstring1_a(tocode), 
					  (const char *) __getEstring2_a(fromcode));
}
