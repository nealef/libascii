/**
 * @file dll_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the dll functions.
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

#include <dll.h>
#include "global_a.h"
 
#pragma export(__dllload_a)
#pragma export(__dllqueryfn_a)
 
#pragma map(__dllload_a, "\174\174A00120")
#pragma map(__dllqueryfn_a, "\174\174A00121")

/*%PAGE																*/
/********************************************************************/
/*																	*/
/* ASCII front-end routines for DLL functions						*/
/*																	*/
/********************************************************************/
 
dllhandle *
__dllload_a(const char * dllname)
{
	return dllload((const char *) __getEstring1_a(dllname));
}
 
void (*
__dllqueryfn_a(dllhandle *dllHandle, char *funcName)) ()
{
	return dllqueryfn(dllHandle,  __getEstring1_a(funcName));
}

