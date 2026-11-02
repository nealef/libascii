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
#pragma export(__wcsstr_ovr)
#pragma export(__wcstod_a)
#pragma export(__wcstol_a)
#pragma export(__wcstoul_a)
#pragma export(__wcsxfrm_a)
#pragma export(__wcwidth_a)
#pragma export(__towlower_a)
#pragma export(__towupper_a)

#pragma map(__wcscoll_a,  "\174\174A00057")
#pragma map(__wcsftime_a, "\174\174A00111")
#pragma map(__wcsstr_ovr, "WCSSTROV")
#pragma map(__wcstod_a,   "\174\174A00373")
#pragma map(__wcstol_a,   "\174\174A00374")
#pragma map(__wcstoul_a,  "\174\174A00375")
#pragma map(__wcsxfrm_a,  "\174\174A00060")
#pragma map(__wcwidth_a,  "\174\174A00027")
#pragma map(__towlower_a, "\174\174A00070")
#pragma map(__towupper_a, "\174\174A00072")

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
 * @brief Finds the first occurrence of needle in haystack.
 */
wchar_t *
__wcsstr_ovr(const wchar_t *haystack, const wchar_t *needle)
{
    if (haystack == NULL || needle == NULL) 
        return NULL;
    if (*needle == L'\0') 
        return (wchar_t *)haystack;

    for (; *haystack != L'\0'; haystack++) {
        if (*haystack == *needle) {
            const wchar_t *h = haystack;
            const wchar_t *n = needle;
            while (*h != L'\0' && *n != L'\0' && *h == *n) {
                h++;
                n++;
            }
            if (*n == L'\0') {
                return (wchar_t *)haystack;
            }
        }
    }
    return NULL;
}

#define WC_SPACE    0x0020
#define WC_TAB      0x0009
#define WC_NL       0x000a
#define WC_CR       0x000d
#define WC_VT       0x000b
#define WC_FF       0x000c
#define WC_ZERO     0x0030
#define WC_NINE     0x0039
#define WC_PERIOD   0x002e
#define WC_e        0x0065
#define WC_E        0x0045
#define WC_PLUS     0x002b
#define WC_MINUS    0x002d

/**
 * customWcstod Converts a wide string to a double-precision floating point number.
 */
double
__wcstod_a(const wchar_t *nptr, wchar_t **endptr)
{
    if (nptr == NULL) {
        if (endptr) *endptr = NULL;
        return 0.0;
    }

    const wchar_t *p = nptr;

    // 1. Skip leading whitespace
    while (*p == WC_SPACE || *p == WC_TAB || *p == WC_NL || *p == WC_CR || *p == WC_VT || *p == WC_FF) {
        p++;
    }

    // 2. Check sign
    int sign = 1;
    if (*p == WC_MINUS) {
        sign = -1;
        p++;
    } else if (*p == WC_PLUS) {
        p++;
    }

    // Convert wide string chunk to multibyte so standard strtod can handle parsing
    char mbBuffer[128];
    const wchar_t *startPtr = p;
    size_t charCount = 0;

    // Parse numeric characters into narrow buffer
    while ((*p >= WC_ZERO && *p <= WC_NINE) || 
            *p == WC_PERIOD || *p == WC_e || *p == WC_E || *p == WC_PLUS || *p == WC_MINUS) {
        if (charCount < sizeof(mbBuffer) - 1) {
            mbBuffer[charCount++] = (char)*p;
        }
        p++;
    }
    mbBuffer[charCount] = '\0';

    if (charCount == 0) {
        if (endptr) *endptr = (wchar_t *)nptr;
        return 0.0;
    }

    char *mbEndPtr = NULL;
    double result = strtod(__getEstring1_a(mbBuffer), &mbEndPtr);
    size_t parsedChars = mbEndPtr - mbBuffer;

    if (endptr) {
        *endptr = (wchar_t *)(startPtr + parsedChars);
    }

    return result * sign;
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

/**
 * @brief Convert to upper case
 */

typedef struct {
    wchar_t range_start;
    wchar_t range_end;
    int offset;
} casemap_t;

// Example mappings for ASCII, Latin Extended, and Cyrillic
static const casemap_t upperMap [] = {
    { L'a',    L'z',    -32   },
    { 0x00E0,  0x00F6,  -32   }, // Latin-1
    { 0x00F8,  0x00FE,  -32   },
    { 0x0101,  0x0177,   -1   }, // Even/Odd pair blocks in Latin Extended-A
    { 0x0391,  0x03a9,  -32   }, // Greek 
    { 0x03c2,  0x03c2,  -49   }, // Greek - capital sigma
    { 0x0430,  0x044F,  -32   }, // Cyrillic small a-ya
    { 0x0530,  0x058f,  -32   }, // Armenian
    { 0x1c90,  0x1cbf,  -4128 }, // Georgian
};

static const casemap_t lowerMap [] = {
    { L'A',    L'Z',    32   },
    { 0x00C0,  0x00D6,  32   }, // Latin-1
    { 0x00D8,  0x00DE,  32   },
    { 0x03b1,  0x03c9,  32   }, // Greek 
    { 0x03c2,  0x03c2,  49   }, // Greek - final sigma 
    { 0x0410,  0x042F,  32   }, // Cyrillic capital A-YA
    { 0x0561,  0x0586,  32   }, // Armenian
};

wchar_t
__towlower_a(wchar_t wc) {
    size_t count = sizeof(lowerMap) / sizeof(lowerMap[0]);

    for (size_t i = 0; i < count; i++) {
        if (wc >= lowerMap[i].range_start && wc <= lowerMap[i].range_end) {
            return wc + lowerMap[i].offset;
        }
    }
    return wc;
}

wchar_t 
__towupper_a(wchar_t wc) {
    size_t count = sizeof(upperMap) / sizeof(upperMap[0]);

    for (size_t i = 0; i < count; i++) {
        if (wc >= upperMap[i].range_start && wc <= upperMap[i].range_end) {
            // For alternating pairs where lower is odd (e.g. 0x0101 -> 0x0100)
            if (upperMap[i].offset == -1 && (wc % 2 == 0)) {
                continue; 
            }
            return wc + upperMap[i].offset;
        }
    }
    return wc;
}
