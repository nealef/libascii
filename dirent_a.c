/**
 * @file dirent_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the dirent functions.
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

#include <dirent.h>
#include "global_a.h"
 
#pragma export(__opendir_a)
#pragma export(__readdir_a)
 
#pragma map(__opendir_a, "\174\174A00371")
#pragma map(__readdir_a, "\174\174A00372")

/*%PAGE																*/
/********************************************************************/
/*																	*/
/* ASCII front-end routines for DIRENT functions					*/
/*																	*/
/********************************************************************/

DIR *
__opendir_a(const char *path)
{
 	return opendir((const char *) __getEstring1_a(path));
}

struct dirent *
__readdir_a(DIR *dir)
{
	struct	dirent *oreaddir;

	oreaddir = readdir(dir);
	if (oreaddir != NULL)
		__toascii_a(oreaddir->d_name, oreaddir->d_name);
	return(oreaddir);
}
