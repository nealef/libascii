/**
 * @file rexec_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the rexec.h functions.
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

#include <rexec.h>
#include <errno.h>
#include "global_a.h"

#pragma export(__rexec_a)

#pragma map(__rexec_a, "\174\174A00424")

/*%PAGE																*/
/**
 * @brief Remotely execute a command
 */
int 
__rexec_a(char **Host, int Port, char *User, char *Password,
              char *Command, int *ErrFileDescParam) 
{
    errno = EINVAL;
    return -1;
#if 0
	char *tmphostptr;

	tmphostptr = __getEstring1_a(*Host);
	return rexec(&tmphostptr, Port, __getEstring2_a(User),
                 __getEstring3_a(Password), __getEstring4_a(Command),
                 ErrFileDescParam);
#endif
}
