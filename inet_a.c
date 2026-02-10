/**
 * @file inet_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the arpa/inet.h functions.
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

#include <arpa/inet.h>
#include <string.h>
#include "global_a.h"

#pragma export(__inet_addr_a)
#pragma export(__inet_ntoa_a)
#pragma export(__inet_ntop_a)
#pragma export(__inet_pton_a)

#pragma map(__inet_addr_a, "\174\174A00401")
#pragma map(__inet_ntoa_a, "\174\174A00402")
#pragma map(__inet_ntop_a, "\174\174A00262")
#pragma map(__inet_pton_a, "\174\174A00365")

/*%PAGE																*/
/**
 * @brief ASCII front-end for inet_addr 
 *
 */

in_addr_t 
__inet_addr_a(const char *cp)
{
	return inet_addr((const char *) __getEstring1_a(cp));
}

/**
 * @brief ASCII front-end for inet_ntoa
 *
 */

char *
__inet_ntoa_a(struct in_addr in)
{
	return strdup(__getAstring1_a(inet_ntoa(in)));
}

/**
 * @brief ASCII front-end for inet_ntop
 *
 */

const char *
__inet_ntop_a(int af, const void *src, char *dst, socklen_t size)
{
    if (inet_ntop(af, src, dst, size) == NULL)
        return NULL;
    __toascii_a(dst, dst);
	return (const char *) dst;
}

/**
 * @brief ASCII front-end for inet_pton
 *
 */

int
__inet_pton_a(int af, const char *src, void *dst)
{
    return inet_pton(af, __getEstring1_a(src), dst);
}
