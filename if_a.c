/**
 * @file if_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the if.h functions
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

#include <net/if.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__if_indextoname_a)
#pragma export(__if_nameindex_a)
#pragma export(__if_nametoindex_a)
#endif

#pragma map(__if_indextoname_a, "\174\174A00117")
#pragma map(__if_nameindex_a, "\174\174A00201")
#pragma map(__if_nametoindex_a, "\174\174A00116")

/*%PAGE																*/
/**
 * @brief ASCII front-end for if_indextoname
 *
 */
unsigned int 
__if_nametoindex_a(const char *ifname)
{
    return if_nametoindex(__getEstring1_a((const char *)ifname));
}

/**
 * @brief ASCII front-end for if_indextoname
 *
 */
char *
__if_indextoname_a(unsigned int ifindex, char *ifname) 
{
    char *res;
    res = if_indextoname(ifindex, __getEstring2_a((const char *)ifname));
    if (res != NULL) 
        __toascii_a(res, res);
    return res;
}

/**
 * @brief ASCII front-end for if_nameindex
 *
 */
struct if_nameindex *
__if_nameindex_a() 
{
    struct if_nameindex *res = if_nameindex();
    if (res != NULL)
        __toascii_a(res->if_name, res->if_name);
    return res;
}
