/**
 * @file string_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the string/string funtions.
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

#include <string.h>
#include <strings.h>
#include <errno.h>
#include "global_a.h"

#pragma export(__strcasecmp_a)
#pragma export(__strcoll_a)
#pragma export(__strerror_a)
#pragma export(__strerror_r_a)
#pragma export(__strncasecmp_a)

#pragma map(__strcasecmp_a, "\174\174A00292")
#pragma map(__strcoll_a, "\174\174A00049")
#pragma map(__strerror_a, "\174\174A00177")
#pragma map(__strerror_r_a, "\174\174A00470")
#pragma map(__strncasecmp_a, "\174\174A00293")

/*%PAGE																*/
/**
 * ASCII front-end for strcasecmp
 */

/**
 * @brief Compare strings according to locale
 */
int
__strcoll_a(const char *string1, const char *string2)
{
    return strcoll(string1, string2);
}

/**
 * @brief String case compare
 */
int 
__strcasecmp_a(const char *s1, const char *s2)
{
	return strcasecmp((const char *) __getEstring1_a(s1),
		    		 (const char *) __getEstring2_a(s2));
}

/**
 * @brief String case compare length limited
 */
int 
__strncasecmp_a(const char *s1, const char *s2, size_t n)
{
	return strncasecmp((const char *) __getEstring1_a(s1),
	                  (const char *) __getEstring2_a(s2), n);
}

/*%PAGE																*/
/**
 * @brief String representation of error
 */
char *
__strerror_a(int errnum)
{
	char	*p;

	p = strerror(errnum);
	__toascii_a(p,p);
	return(p);
}

/**
 * @brief String representation of error (re-entrant)
 */
int
__strerror_r_a(int errnum, char *strerrbuf, size_t buflen)
{
	char   *p;
    size_t len; 

	p = strerror(errnum);
    if (strlen(p) > buflen) {
        len = buflen - 1;
        errno = ERANGE;
    } else 
        len = buflen;
    memset(strerrbuf, 0, len);
	__toasciilen_a(strerrbuf, p, (int) len);
	return 0;
}
