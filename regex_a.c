/**
 * @file regex_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the regex functions.
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
 
#include <regex.h>
#include "global_a.h"

#pragma export(__regcomp_a)
#pragma export(__regerror_a)
#pragma export(__regexec_a)
#pragma export(__regfree_a)

#pragma map(__regcomp_a, "\174\174A00041")
#pragma map(__regerror_a, "\174\174A00043")
#pragma map(__regexec_a, "\174\174A00045")
#pragma map(__regfree_a, "\174\174A00047")
 
/*%PAGE																*/
/**
 * @brief Compile a regular expression
 */
int 
__regcomp_a(regex_t *preg, const char *pattern, int cflags)
{
	return regcomp(preg, (const char *) __getEstring1_a(pattern), cflags);
}
 
/*%PAGE																*/
/**
 * @brief Execute a regular expression
 */
int 
__regexec_a(regex_t *preg, const char *pattern, size_t nmatch, regmatch_t *pmatch, int eflags)
{
	return regexec(preg, (const char *) __getEstring1_a(pattern), nmatch, pmatch, eflags);
}

/**
 * @brief Return an error message
 */
size_t 
__regerror_a(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size)
{
    int res;

    res = regerror(errcode, preg, errbuf, errbuf_size);
    if (res > 0)
        __toasciilen_a(errbuf, errbuf, res);
    return res;
}

/**
 * @brief Free a regular expression
 */
void
__regfree_a(regex_t *re)
{
    regfree(re);
}
