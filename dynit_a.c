/**
 * @file dynit_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the dynit.h functions.
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

#include <dynit.h>
#include "global_a.h"

#pragma map(__dynalloc_a, "\174\174A00385")
 
#pragma export(__dynalloc_a)
 
/*%PAGE																*/
/********************************************************************/
/*																	*/
/* ASCII front-end routines for DYNIT functions						*/
/*																	*/
/********************************************************************/

int 
__dynalloc_a(__dyn_t *dyn_parms)
{
	int dynalloc_rc;

	__toebcdic_a(dyn_parms->__ddname,dyn_parms->__ddname);
	__toebcdic_a(dyn_parms->__dsname,dyn_parms->__dsname);
	dynalloc_rc = dynalloc(dyn_parms);
	return(dynalloc_rc);
}
