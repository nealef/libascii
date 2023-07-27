/**
 * @file wchar_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the wchar.h functions.
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

#include <wchar.h>
#include "global_a.h"

#pragma export(__wcscoll_a)
#pragma export(__wcstol_a)
#pragma export(__wcstoul_a)
#pragma export(__wcsxfrm_a)
#pragma export(__wcwidth_a)

#pragma map(__wcscoll_a, "\174\174A00057")
#pragma map(__wcstol_a, "\174\174A00374")
#pragma map(__wcstoul_a, "\174\174A00375")
#pragma map(__wcsxfrm_a, "\174\174A00060")
#pragma map(__wcwidth_a, "\174\174A00027")

/*%PAGE																*/

/**
 * @brief Language collation string comparison
 */
int 
__wcscoll_a(const wchar_t *wcs1, const wchar_t *wcs2)
{
    return wcscoll(wcs1, wcs2);
}

/**
 * @brief Convert wide characters to long integer
 */
long int 
__wcstol_a(const wchar_t *nptr, wchar_t **endptr, int base)
{
    return(wcstol(nptr, endptr, base));
}

/**
 * @brief Convert wide characters to unsigned long integer
 */
unsigned long int 
__wcstoul_a(const wchar_t *nptr, wchar_t **endptr, int base)
{
    return(wcstoul(nptr, endptr, base));
}

/**
 * @brief Get width of wide character string
 */
int
__wcwidth_a(const wchar_t wc)
{
    return(wcwidth(wc));
}

/**
 * @brief Transform a wide character string
 */
size_t 
__wcsxfrm_a(wchar_t *wcs1, const wchar_t *wcs2, size_t n)
{
    return wcsxfrm(wcs1, wcs2, n);
}
