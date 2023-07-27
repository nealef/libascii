/**
 * @file grp_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the grp.h functions. 
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

#include <grp.h>
#include "global_a.h"

#pragma export(__getgrgid_a)
#pragma export(__getgrnam_a)

#pragma map(__getgrgid_a, "\174\174A00254")
#pragma map(__getgrnam_a, "\174\174A00255")

/*%PAGE																*/
/**
 * @brief ASCII front-end for getgrgid
 */
struct group *
__getgrgid_a(gid_t gid)
{
	char 	**curr;
	struct	group *grp;

	grp = getgrgid(gid);
	if ((grp) != NULL) {
		__toascii_a(grp->gr_name,grp->gr_name);
		for (curr=grp->gr_mem; (*curr) != NULL; curr++)
			__toascii_a(*curr, *curr);
		return(grp);
	} else
		return(NULL);
}

/**
 * @brief ASCII front-end for getgrnam
 */
struct group *
__getgrnam_a(const char *name) 
{
	char 	**curr;
	struct	group *grp;

	grp = getgrnam((const char *) __getEstring1_a(name));
	if ((grp) != NULL) {
		__toascii_a(grp->gr_name,grp->gr_name);
		for (curr=grp->gr_mem; (*curr) != NULL; curr++)
			__toascii_a(*curr, *curr);
		return(grp);
	} else
		return(NULL);
}
