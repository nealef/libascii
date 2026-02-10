/**
 * @file termios_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the termios.h functions.
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

#include <termios.h>
#include "global_a.h"

#pragma export(__tcgetattr_a)
#pragma export(__tcsetattr_a)

#pragma map(__tcgetattr_a, "\174\174A00415")
#pragma map(__tcsetattr_a, "\174\174A00416")

/*%PAGE																*/
/**
 * @brief Get the Attributes for a Terminal
 */
int 
__tcgetattr_a(int fd,struct termios *termptr)
{
	return tcgetattr(fd, termptr);
}

/**
 * @brief Set the Attributes for a Terminal
 */
int 
__tcsetattr_a(int fd, int when, const struct termios *termptr)
{
	return tcsetattr(fd, when, termptr);
}
