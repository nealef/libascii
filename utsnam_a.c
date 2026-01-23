/**
 * @file utsname_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the sys/utsname.h functions.
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

#include <sys/utsname.h>
#include "global_a.h"

#pragma export(__uname_a)

#pragma map(__uname_a, "\174\174A00296")

/*%PAGE																*/
/**
 * @brief Display Current Operating System Name
 */
int 
__uname_a(struct utsname *name)
{
	int	rc;

	rc = uname(name);
	if (rc == 0) {
		__toascii_a(name->sysname, name->sysname);
		__toascii_a(name->nodename, name->nodename);
		__toascii_a(name->release, name->release);
		__toascii_a(name->version, name->version);
		__toascii_a(name->machine, name->machine);
	}
	return rc;
}
