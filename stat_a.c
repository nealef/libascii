/**
 * @file stat_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the stat functions.
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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include "global_a.h"
 
#ifdef GEN_PRAGMA_EXPORT
#pragma export(__chmod_a)
#pragma export(__lstat_a)
#pragma export(__mkdir_a)
#pragma export(__mkfifo_a)
#pragma export(__mknod_a)
#pragma export(__stat_a)
#pragma export(__statvfs_a)
#endif
 
#pragma map(__chmod_a, "\174\174A00129")
#pragma map(__lstat_a, "\174\174A00135")
#pragma map(__mkdir_a, "\174\174A00130")
#pragma map(__mkfifo_a, "\174\174A00133")
#pragma map(__mknod_a, "\174\174A00137")
#pragma map(__stat_a, "\174\174A00131")
#pragma map(__statvfs_a, "\174\174A00204")

/*%PAGE																*/
/**
 * ASCII front-end routines for STAT functions
 */
 
/**
 * @brief Change mode
 */
int 
__chmod_a(const char *path, mode_t mode)
{
	return chmod((const char *) __getEstring1_a(path), mode);
}
 
/**
 * @brief File information
 */
int 
__lstat_a(const char *path, struct stat *buf)
{
    return lstat((const char *) __getEstring1_a(path), buf);
}
 
/**
 * @brief Make directory
 */
int 
__mkdir_a(const char *path, mode_t mode)
{
	return mkdir((const char *) __getEstring1_a(path), mode);
}
 
/**
 * @brief Make fifo
 */
int 
__mkfifo_a(const char *pathname, mode_t mode)
{
	return mkfifo((const char *)__getEstring1_a(pathname), mode);
}
 
/**
 * @brief Make node
 */
int 
__mknod_a(const char *path, mode_t mode, dev_t dev_identifier)
{
	return mknod((const char *)__getEstring1_a(path),
				 mode, dev_identifier);
}
 
/**
 * @brief File information
 */
int 
__stat_a(const char *path, struct stat *buf)
{
	return stat((const char *) __getEstring1_a(path), buf);
}
 
/**
 * @brief File system information
 */
int 
__statvfs_a(const char *path, struct statvfs *fsinfo)
{
	int rc;
 
	rc = statvfs((const char *) __getEstring1_a(path), fsinfo);
	if (rc == 0)
		__toascii_a(fsinfo->f_OEcbid, fsinfo->f_OEcbid);
	return rc;
}
