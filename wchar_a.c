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
#pragma export(__wcsftime_a)
#pragma export(__wcstol_a)
#pragma export(__wcstoul_a)
#pragma export(__wcsxfrm_a)
#pragma export(__wcwidth_a)

#pragma map(__wcscoll_a,  "\174\174A00057")
#pragma map(__wcsftime_a, "\174\174A00111")
#pragma map(__wcstol_a,   "\174\174A00374")
#pragma map(__wcstoul_a,  "\174\174A00375")
#pragma map(__wcsxfrm_a,  "\174\174A00060")
#pragma map(__wcwidth_a,  "\174\174A00027")

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
 * @brief Format Date and Time
 *
 * The API doesn't support the epoch '%s' format so we have to do it
 * ourselves.
 */
size_t 
__wcsftime_a(wchar_t *wcs, size_t maxsize, const wchar_t *format,
             const struct tm *time_ptr)
{
    size_t len;
    wchar_t *epoch;

    len = wcsftime(wcs, maxsize, __getEwstring3_a(format), time_ptr);
    if ((epoch = wcsstr(wcs, L"%s"))) {
        wchar_t *res = __alloca(4 * maxsize),
                epsec[22],
                *cursor = wcs,
                *eos = &wcs[len];
        time_t secs = mktime((struct tm *)time_ptr);
        int l = 0;

        swprintf(epsec, sizeof(epsec), L"%u", secs);
        memset(res, 0, (4 * maxsize));

        do {
            /*
             * Check for any leading characters
             */
            l = ((uintptr_t) epoch - (uintptr_t) cursor) / 2;
            if (l > 0)
                wcsncat(res, cursor, l);

            /*
             * Replace %s with epoch seconds
             */
            epoch += 2;
            cursor = epoch;
            wcscat(res, epsec);

            /*
             * Check for any more %s instancs
             */
            epoch = wcsstr(cursor, L"%s");
        } while (epoch != NULL);

        /*
         * Handle any remaining characters
         */
        if (cursor < eos)
            wcscat(res, cursor);

        /*
         * Copy to result area and set length
         */
        wcsncpy(wcs, res, maxsize - 2);
        len = wcslen(wcs);
    }

    if (len > 0)
        __towasciilen_a(wcs, wcs, len);

    return len;
}

/**
 * @brief Convert wide characters to long integer
 */
long int 
__wcstol_a(const wchar_t *nptr, wchar_t **endptr, int base)
{
    return(wcstol(__getEwstring1_a(nptr), endptr, base));
}

/**
 * @brief Convert wide characters to unsigned long integer
 */
unsigned long int 
__wcstoul_a(const wchar_t *nptr, wchar_t **endptr, int base)
{
    return(wcstoul(__getEwstring1_a(nptr), endptr, base));
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
    size_t l;

    l = wcsxfrm(wcs1, __getEwstring2_a(wcs2), n);
    if ((wcs1 != NULL) && (l > 0))
        __towascii_a(wcs1, wcs1);
    return l;
}
