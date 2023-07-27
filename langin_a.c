/**
 * @file langin_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the langinfo functions.
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

#pragma langlvl( extended )
 
#include <langinfo.h>
#include "global_a.h"
 
#pragma export(__nl_langinfo_a)
 
#pragma map(__nl_langinfo_a, "\174\174A00077")

/*%PAGE																*/
/**
 * @brief Retrieve Locale Information
 */
char *
__nl_langinfo_a(nl_item item)
{
	return __getAstring1_a( nl_langinfo(item) );
}
