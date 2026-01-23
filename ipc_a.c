/**
 * @file ipc_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the sys/ipc functions.
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

#include <sys/ipc.h>
#include "global_a.h"

#pragma export(__ftok_a)

#pragma map(__ftok_a, "\174\174A00239")

/*%PAGE																*/
/**
 * @brief Generate IPC key
 */ 
key_t 
__ftok_a(const char *path, int id)
{
	return ftok((const char *) __getEstring1_a(path), id );
}
