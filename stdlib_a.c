/**
 * @file stdlib_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the stdlib functions.
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

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/utsname.h>
#include <grp.h>
#include <pwd.h>
#ifdef GEN_IEEE_FP
#include <ieee_md.h>
#endif
#include "global_a.h"
#include "envtable.h"

#pragma export(__Envna_a)
#pragma export(__a64l_a)
#pragma export(__atof_a)
#pragma export(__atoi_a)
#pragma export(__atol_a)
#pragma export(__ecvt_a)
#pragma export(__fcvt_a)
#pragma export(__gcvt_a)
#pragma export(__getenv_a)
#pragma export(__getenv_ea)
#pragma export(__l64a_a)
#pragma export(__mblen_a)
#pragma export(__mbstowcs_a)
#pragma export(__mbtowc_a)
#pragma export(__mkstemp_a)
#pragma export(__mktemp_a)
#pragma export(__putenv_a)
#pragma export(__realpath_a)
#pragma export(__setenv_a)
#pragma export(__strtod_a)
#pragma export(__strtoimax_a)
#pragma export(__strtol_a)
#pragma export(__strtoul_a)
#pragma export(__strtoumax_a)
#pragma export(__system_a)
#pragma export(__unsetenv_a)
#pragma export(__wcstombs_a)
#pragma export(__wctomb_a)

#pragma map(__Envna_a, "\174\174ENVNA") 
#pragma map(__a64l_a, "\174\174A00172")
#pragma map(__atof_a, "\174\174A00164")
#pragma map(__atoi_a, "\174\174A00165")
#pragma map(__atol_a, "\174\174A00166")
#pragma map(__ecvt_a, "\174\174A00173")
#pragma map(__fcvt_a, "\174\174A00174")
#pragma map(__gcvt_a, "\174\174A00175")
#pragma map(__getenv_a, "\174\174A00181")
#pragma map(__getenv_ea, "\174\174A00423")
#pragma map(__l64a_a, "\174\174A00176")
#pragma map(__mblen_a, "\174\174A00002")
#pragma map(__mbstowcs_a, "\174\174A00006")
#pragma map(__mbtowc_a, "\174\174A00008")
#pragma map(__mkstemp_a, "\174\174A00184")
#pragma map(__mktemp_a, "\174\174A00240")
#pragma map(__putenv_a, "\174\174A00186")
#pragma map(__realpath_a, "\174\174A00187")
#pragma map(__setenv_a, "\174\174A00188")
#pragma map(__strtod_a, "\174\174A00167")
#pragma map(__strtoimax_a, "\174\174A00451")
#pragma map(__strtol_a, "\174\174A00168")
#pragma map(__strtoul_a, "\174\174A00169")
#pragma map(__strtoumax_a, "\174\174A00452")
#pragma map(__system_a, "\174\174A00189")
#pragma map(__unsetenv_a, "\174\174A00471")
#pragma map(__wcstombs_a, "\174\174A00013")
#pragma map(__wctomb_a, "\174\174A00023")

/*%PAGE																*/
/**
 * @brief Convert string to float
 */
double 
__atof_a(const char *nptr)
{
	return atof((const char *) __getEstring1_a(nptr));
}

/**
 * @brief Convert string to integer
 */
int 
__atoi_a(const char *nptr)
{
	return atoi((const char *) __getEstring1_a(nptr));
}

/**
 * @brief Convert string to long
 */
long int 
__atol_a(const char *nptr)
{
	return atol((const char *) __getEstring1_a(nptr));
}

/**
 * @brief Convert double to string
 */
char *
__ecvt_a(double x, int ndigit, int *decpt, int *sign)
{
	char *tmp_out;
	tmp_out = ecvt(x,ndigit, decpt,sign); /* call ecvt    */
	__toascii_a(tmp_out,tmp_out);        /* convert output to ascii */
	return ((char *)tmp_out);
}

/**
 * @brief Convert double to string
 */
char *__fcvt_a(double x, int ndigit, int *decpt, int *sign)
{
	char *tmp_out;
	tmp_out =  fcvt(x, ndigit, decpt, sign);
	__toascii_a(tmp_out,tmp_out);        /* convert output to ascii */
	return tmp_out;
}

/**
 * @brief Convert double to string
 */
char *
__gcvt_a(double x, int ndigit, char * buf)
{
	char *tmp_out;
	tmp_out =  gcvt(x, ndigit, buf);
	__toascii_a(tmp_out,tmp_out);        /* convert output to ascii */
	return tmp_out;
}

/**
 * @brief Get an environment variable
 */
char *
__getenv_a(const char *varname)
{
	ATHD_t *atp;
	char *eName;
	char *eValue;
	char *aValue;
	char *result;

	atp = athdp();  /* get pointer to athd thread structure */

    if ((aValue = htFetchValue(atp->envtbl, varname)) != NULL)
            return aValue;

    eName = __alloca(strlen(varname) + 1);

	__toebcdic_a(eName, varname); /* convert ascii to ebcdic */
	eValue = getenv(eName);

	if (eValue == NULL)
		return(NULL);

    aValue = __alloca(strlen(eValue) + 1);
	__toascii_a(aValue, eValue);
    result = htAddValue(atp->envtbl, varname, aValue, 1);

	return(result); /* Return address of return buffer  */
}

/**
 * @brief Get an environment variable (enhanced ASCII)
 */
char *
__getenv_ea(const char *varname)
{
    return __getenv_a(varname);
}

/**
 * @brief Translates an ISO-8859-1 multibyte string to a wide character array.
 *
 * @param sourceString          Source extended ASCII byte string.
 * @param destinationWideBuffer Output wide character buffer.
 * @param maxWcharsToWrite      Maximum number of wchar_t elements to write.
 * @return                      Number of wide characters written, or -1 on overflow.
 */
static size_t
convertAsciiToWideString(const char *sourceString, wchar_t *destinationWideBuffer, size_t maxWcharsToWrite)
{
    if (sourceString == NULL) {
        return 0;
    }

    // Query mode: calculate required wide character length
    if (destinationWideBuffer == NULL) {
        size_t stringLength = 0;
        const unsigned char *sourcePtr = (const unsigned char *)sourceString;
        while (*sourcePtr++ != '\0') {
            stringLength++;
        }
        return stringLength;
    }

    const unsigned char *sourcePtr = (const unsigned char *)sourceString;
    size_t wcharsWritten = 0;

    while (*sourcePtr != '\0' && wcharsWritten < maxWcharsToWrite) {
        // Zero-extend the unsigned byte value directly to wchar_t
        destinationWideBuffer[wcharsWritten] = (wchar_t)(*sourcePtr);
        sourcePtr++;
        wcharsWritten++;
    }

    if (wcharsWritten < maxWcharsToWrite) {
        destinationWideBuffer[wcharsWritten] = L'\0';
    }

    return wcharsWritten;
}

/**
 * @brief Translates a wide character array to an ISO-8859-1 byte string.
 *
 * @param destinationBuffer Output byte buffer.
 * @param sourceWideString  Source wide character string.
 * @param maxBytesToWrite   Capacity of destinationBuffer.
 * @return                  Number of bytes written, or (size_t)-1 on error.
 */
static size_t
convertWideToAsciiString(char *destinationBuffer, const wchar_t *sourceWideString, size_t maxBytesToWrite)
{
    if (sourceWideString == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }

    // Query mode: calculate required byte buffer length
    if (destinationBuffer == NULL) {
        size_t totalBytesNeeded = 0;
        const wchar_t *widePtr = sourceWideString;
        while (*widePtr != L'\0') {
            if (*widePtr > 0x00FF) {
                errno = EILSEQ; // Character cannot fit in ISO-8859-1
                return (size_t)-1;
            }
            totalBytesNeeded++;
            widePtr++;
        }
        return totalBytesNeeded;
    }

    const wchar_t *widePtr = sourceWideString;
    size_t bytesWritten = 0;

    while (*widePtr != L'\0' && bytesWritten < maxBytesToWrite) {
        if (*widePtr > 0x00FF) {
            errno = EILSEQ; // Value exceeds extended ASCII range
            return (size_t)-1;
        }

        destinationBuffer[bytesWritten] = (char)(*widePtr & 0xFF);
        widePtr++;
        bytesWritten++;
    }

    if (bytesWritten < maxBytesToWrite) {
        destinationBuffer[bytesWritten] = '\0';
    }

    return bytesWritten;
}

/**
 * @brief Convert multibyte character to wide character using iconv()
 *
 * @param pwc  Pointer to the destination wide character
 * @param s    Pointer to the multibyte character string input
 * @param n    Maximum number of bytes to inspect from 's'
 * @return     Number of bytes consumed, 0 for null character, or -1 on error
 */
int 
__mbtowc_a(wchar_t *pwc, const char *s, size_t n)
{
    if (s == NULL) {
        return 0; 
    }

    if (n == 0) {
        errno = EILSEQ;
        return -1;
    }

    if (*s == '\0') {
        if (pwc) {
            *pwc = L'\0';
        }
        return 0;
    }

    if (pwc) {
        // Use our subroutine to convert a single character window
        char singleCharString[2] = { *s, '\0' };

        convertAsciiToWideString(singleCharString, pwc, 1);
    }

    return 1; // 1 byte consumed
}


/**
 * @brief Convert multibyte characters to wide characters
*
 * @param pwcs Pointer to the destination wide-character array (or NULL)
 * @param s    Pointer to the source multibyte string
 * @param n    Maximum number of wchar_t elements to write to pwcs
 * @return     Number of wide characters written (excluding L'\0'), or (size_t)-1 on error
 */
size_t 
__mbstowcs_a(wchar_t *pwcs, const char *s, size_t n)
{
    if (s == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }

    // Delegate behavior entirely to our subroutine mapping logic
    return convertAsciiToWideString(s, pwcs, n);
}

/**
 * @brief Convert wide character to multibyte character
 *
 * @param pmb   Pointer to the Output Byte Buffer
 * @param c     The Wide Character to Convert
 * @return      Number of Bytes Written, 0- ForNull, or -1 On Error
 */
int
__wctomb_a(char *pmb, wchar_t c)
{
    if (pmb == NULL) {
        return 0; // Stateless encoding
    }

    if (c == L'\0') {
        *pmb = '\0';
        return 0;
    }

    wchar_t singleWideString[2] = { c, L'\0' };
    char resultByte = '\0';

    // Attempt the conversion using our sub-macro utility
    size_t result = convertWideToAsciiString(&resultByte, singleWideString, 1);
    
    if (result == (size_t)-1) {
        // errno is already set to EILSEQ inside convertWideToAsciiString if wideChar > 0xFF
        return -1;
    }

    *pmb = resultByte;
    return 1; // 1 byte generated
}

/**
 * @brief Convert wide characters to multibyte characters
 *
 * @param pmb    Destination Buffer (or NULL For Length Query)
 * @param string Source Wide Character String
 * @param n      Maximum Number of Bytes to Write to Destnation Buffer
 * @return       Number of bytes written (excluding NULL), or (size_t)-1 On Error
 */
size_t
__wcstombs_a(char *pmb, wchar_t *string, size_t n)       
{
    if (string == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }

    // Delegate directly to the custom character boundary mapping subroutine
    return convertWideToAsciiString(pmb, string, n);
}

/**
 * @brief Determines the number of bytes in a multibyte character using iconv.
 *
 * @param inputString  Pointer to the multibyte character sequence
 * @param maxBytesRead Maximum number of bytes to inspect from inputString
 * @return             Number of bytes consumed, 0 for null character, or -1 on error
 */
int 
__mblen_a(const char *inputString, size_t maxBytesRead)
{
    if (inputString == NULL) {
        return 0; // Latin-1/Extended ASCII is stateless
    }

    if (maxBytesRead == 0) {
        errno = EILSEQ;
        return -1;
    }

    if (*inputString == '\0') {
        return 0;
    }
    
    // Every valid character in extended ASCII is exactly 1 byte long
    return 1;
}
        
/**
 * @brief Make a unique file name
 */
int
__mkstemp_a(char *template)
{
    int res;

	__toebcdic_a(template,template);  /* convert template to ebcdic */
	res = mkstemp(template);          /* call mkstemp               */
	__toascii_a(template,template);   /* convert template back to ascii */
	return (res);
}

/**
 * @brief Make a unique file name
 */
char *
__mktemp_a(char *template)
{
    char *tmp_out;

	__toebcdic_a(template,template);  /* convert template to ebcdic */
	tmp_out = mktemp(template);       /* call mktemp                */
	__toascii_a(tmp_out,tmp_out);     /* convert output to ascii    */
	__toascii_a(template,template);   /* convert template back to ascii */
	return ((char *)tmp_out);
}

/**
 * @brief Put an environment variable
 */
int 
__putenv_a(const char *envvar)
{
	return putenv(__getEstring1_a(envvar));
}

/**
 * @brief Resolve a path name
 */
char *
__realpath_a(const char * file_name, char *resolved_name)
{
    char *res = realpath((const char *) __getEstring1_a(file_name), resolved_name);
    if (res) 
        __toascii_a(resolved_name, (const char *) resolved_name);
    return (res);
}

/**
 * @brief Set an environment variable
 */
int 
__setenv_a(char *var_name, char *new_value, int change_flag)
{
	ATHD_t *atp = athdp();

    (void) htAddValue(atp->envtbl, var_name, new_value, change_flag);
	return setenv((const char *) __getEstring1_a(var_name),
				  (const char *) __getEstring2_a(new_value),
                  change_flag);
}

/**
 * @brief Convert string to double
 */
double 
__strtod_a( const char *nptr, char **endptr)
{
	char		*tmp;
	char		*e;
	double		tmpdbl;
	tmp = __getEstring1_a(nptr);
	tmpdbl = strtod(tmp, &e);
	if (endptr != NULL) {
		if (e)
			*endptr = (char*) nptr + (e - tmp);
		else
			*endptr = (char*) nptr;
	}
	return tmpdbl;
}

/**
 * @brief Convert string to intmax
 */
intmax_t
__strtoimax_a( const char *nptr, char **endptr, int base )
{
	char	*tmp;
	long	l;
	char	*e;

	tmp = __getEstring1_a(nptr);
	l = strtol(tmp, &e, base);
	if ( endptr )
		*endptr = (char*) nptr + (e - tmp);
	return (intmax_t) l;
}

/**
 * @brief Convert string to long
 */
long int 
__strtol_a( const char *nptr, char **endptr, int base )
{
	char	*tmp;
	long	l;
	char	*e;

	tmp = __getEstring1_a(nptr);
	l = strtol(tmp, &e, base);
	if ( endptr )
		*endptr = (char*) nptr + (e - tmp);
	return l;
}

/**
 * @brief Convert string to unsigned long
 */
unsigned long int 
__strtoul_a(const char *s, char **endptr, int base)
{
	char		*tmp;
	unsigned	long ul;
	char		*e;

	tmp = __getEstring1_a(s);
	ul = strtoul(tmp, &e, base);
	if ( endptr )
		*endptr = (char*) s + (e - tmp);
	return ul;
}

/**
 * @brief Convert string to uintmax_t
 */
uintmax_t
__strtoumax_a(const char *s, char **endptr, int base)
{
	char		*tmp;
	unsigned	long ul;
	char		*e;

	tmp = __getEstring1_a(s);
	ul = strtoul(tmp, &e, base);
	if ( endptr )
		*endptr = (char*) s + (e - tmp);
	return (uintmax_t) ul;
}

/**
 * @brief Issue a system command
 */
int 
__system_a(const char *s)
{
	if (s)
		return system(__getEstring1_a(s));
	else
		return system(NULL);
}

/**
 * @brief Convert Base-64 String Representation to Long Integer
 */
long 
__a64l_a(const char *s)
{
	return a64l(__getEstring1_a(s));
}

/**
 * @brief Convert Long Integer to Base-64 String Representation
 */
char *
__l64a_a( long int l )
{
    char *s;
	s = l64a(l);
	return __getAstring1_a(s);
}

/**
 * @brief Unset an an environment variable
 */
int
__unsetenv_a(const char *name)
{
	ATHD_t *atp = athdp();

    htDeleteValue(atp->envtbl, name);
    return setenv((const char *) __getEstring1_a(name), NULL, 1);
}

/**
 * @brief Return the environment in ASCII
 */
char ***
__Envna_a(void)
{
    extern char **environ;

    char **e = environ;
    static char **a = NULL;
    size_t lEnv = 0;
    int iEnv;

    /*
     * Free any previous ASCII environ. 
     */
    if (a != NULL) {
        for (iEnv = 0; a[iEnv] != NULL; iEnv++) {
            free(a[iEnv]);
        }
        free(a);
    }

    for (iEnv = 0; e[iEnv] != NULL; iEnv++)
        lEnv += sizeof(e);

    lEnv += sizeof(e);

    a = malloc(lEnv);
    memset(a, 0, lEnv);
    
    for (iEnv = 0; e[iEnv] != NULL; iEnv++) {
        char *p;
        p = strdup(e[iEnv]);
        __toascii_a(p, (const char *)p);
        a[iEnv] = p;
    }
    a[iEnv] = NULL;

    return &a;
}
