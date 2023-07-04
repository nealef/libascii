/**
 * @file fcntl_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the fcntl functions.
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
 ********************************************************************/

/********************************************************************
 * Copyright:   Licensed Materials - Property of IBM.               *
 *              (C) Copyright IBM Corp. 1997.                       *
 *              All rights reserved.                                *
 ********************************************************************/

#include <stdarg.h>
#include <fcntl.h>
#include "global_a.h"
 
#ifdef GEN_PRAGMA_EXPORT
#pragma export(__creat_a)
#pragma export(__open_a)
#endif
 
#pragma map(__creat_a, "\174\174A00143")
#pragma map(__open_a, "\174\174A00144")

/*%PAGE																*/
/********************************************************************/
/*																	*/
/* ASCII front-end routines for FCNTL functions						*/
/*																	*/
/********************************************************************/

/**
 * @brief Create a file and check its translation requirements
 *
 * @param pathname Path name
 * @param mode     Creation mode flags
 * @returns File Descriptor
 */
int
__creat_a(const char* path, mode_t mode)
{
    int fd;

	fd = creat(__getEstring1_a(path), mode);

    __insertFD(fd);

    return fd;
}
 
/**
 * @brief Open a file and check its translation requirements
 *
 * @param pathname Path name
 * @param oflag    Open flags
 * @param mode     Mode flags (optional)
 * @returns File Descriptor
 */
int
__open_a(const char *path, int oflag, ...)
{
	va_list ap;
	mode_t tmpmode;
	int fd;
 
	if (oflag & O_CREAT) {
		va_start(ap, oflag);
		tmpmode = va_arg(ap, mode_t);
		fd = open((const char *) __getEstring1_a(path), oflag, tmpmode);
		va_end(ap);
	} else
		fd = open((const char *) __getEstring1_a(path), oflag);

    __insertFD(fd);

	return(fd);
}
