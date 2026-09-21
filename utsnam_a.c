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
#pragma export(__uname_ovr)

#pragma map(__uname_a, "\174\174A00296")
#pragma map(__uname_ovr, "UNAMEOVR")

struct zos_utsname {
    char sysname[16];
    char nodename[32];
    char release[8];
    char version[8];
    char machine[16];
};

/*%PAGE																*/
/**
 * @brief Get Current Operating System Name
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

/**
 * @brief Get Current Operating System Name (z/OS override)
 *
 * The z/OS utsname is smaller than z/VM. We have been compiled
 * with __ZVM__ so we pick that larger version up so we have to 
 * map it back to what z/OS expects.
 *
 * @param[out] name utsname structure as defined by z/OS
 * @returns 0 or -1
 */
int 
__uname_ovr(struct zos_utsname *name)
{
	int	rc;
    struct utsname ut;

	rc = uname(&ut);
	if (rc == 0) {
		__toascii_a(name->sysname, ut.sysname);
		__toascii_a(name->nodename, ut.nodename);
        ut.release[sizeof(name->release) - 1] = 0;
		__toascii_a(name->release, ut.release);
        ut.version[sizeof(name->version) - 1] = 0;
		__toascii_a(name->version, ut.version);
		__toascii_a(name->machine, ut.machine);
	}
	return rc;
}
