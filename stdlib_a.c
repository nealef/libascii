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
#include <locale.h>
#include <langinfo.h>
#ifdef GEN_IEEE_FP
# include <ieee_md.h>
#endif
#include <wchar.h>
#include <wctype.h>
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
#pragma export(__mbrlen_a)
#pragma export(__mbsinit_a)
#pragma export(__mbrtowc_a)
#pragma export(__mbsrtowcs_a)
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
#pragma export(__wcrtomb_a)
#pragma export(__wcsrtombs_a)
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
#pragma map(__mbrlen_a, "MBRLEN")
#pragma map(__mbrtowc_a, "MBRTOWC")
#pragma map(__mbsinit_a, "MBSINIT")
#pragma map(__mbsrtowcs_a, "MBSRTOWCS")
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
#pragma map(__wcrtomb_a, "WCRTOMB")
#pragma map(__wcsrtombs_a, "WCSRTOMBS")
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
 *
 * We use setenv as the inmplementation of putenv is funny about automatic storage
 */
int 
__putenv_a(const char *envvar)
{
    char *var_name, *new_value;
	ATHD_t *atp = athdp();

    var_name = __alloca(strlen(envvar)+1);
    __toebcdic_a(var_name, envvar);
    if (strchr(var_name, '=')) {
        var_name  = strtok(var_name, "=");
        new_value = strtok(NULL, "=");
    } else {
        new_value = NULL;
    }
    (void) htAddValue(atp->envtbl, var_name, new_value, 1);
    return setenv(var_name, new_value, 1);
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


/**
 * Start of Wide Character and Multibyte Character Functions
 */
static unicodeEncoding_t
deduceEncodingFromCurrentLocale(void)
{
    const char *codesetName = nl_langinfo(CODESET);
    if (codesetName == NULL) return ENCODING_UTF8;
    if (strcasecmp(codesetName, "UTF-8") == 0 || strcasecmp(codesetName, "UTF8") == 0) return ENCODING_UTF8;
    if (strcasecmp(codesetName, "UTF-16LE") == 0) return ENCODING_UTF16_LE;
    if (strcasecmp(codesetName, "UTF-16BE") == 0) return ENCODING_UTF16_BE;
    if (strcasecmp(codesetName, "UTF-32LE") == 0) return ENCODING_UTF32_LE;
    if (strcasecmp(codesetName, "UTF-32BE") == 0) return ENCODING_UTF32_BE;
        return ENCODING_ASCII_EXTENDED;
}

/**
 * @brief Test State Object for Initial State
 *
 * @param[in] ps Shift state
 * @returns 1 if ps is initial state 0 otherwise
 */
int 
__mbsinit_a(const mbstate_t *ps)
{
    if (ps == NULL) return 1;
    const mbstate_a_t *state = (const mbstate_a_t *)ps;
    return (state->pendingLowSurrogate == 0 && state->pendingHighSurrogate == 0);
}

/**
 * @brief Implement mbrtowc for ASCII 
 *
 * @param[out] pwc Wide character buffer
 * @param[in]  s Input string
 * @param[in]  n Size of string
 * @param[in|out] ps Internal state
 * @returns Number of bytes converted or error (< 0)
 */
size_t
__mbrtowc_a(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
    ATHD_t *atp = athdp();
    mbstate_a_t *state = (ps == NULL) ? &atp->mb : (mbstate_a_t *)ps;

    if (s == NULL) {
        state->pendingLowSurrogate = 0;
        state->pendingHighSurrogate = 0;
        return 0; // 0 indicates stateless encoding mode
    }

    if (state->pendingLowSurrogate != 0) {
        if (pwc != NULL) *pwc = state->pendingLowSurrogate;
        state->pendingLowSurrogate = 0;
        return 0;
    }

    if (n == 0) return (size_t)-2;

    unicodeEncoding_t encoding = deduceEncodingFromCurrentLocale();
    uint32_t scalarCodePoint = 0;
    size_t bytesConsumed = 0;

    switch (encoding) {
        case ENCODING_ASCII_EXTENDED: {
            unsigned char byte = (unsigned char)s[0];
            if (byte == '\0') {
                if (pwc) *pwc = L'\0';
                return 0;
            }
            scalarCodePoint = byte;
            bytesConsumed = 1;
            break;
        }
        case ENCODING_UTF8: {
            unsigned char byte0 = (unsigned char)s[0];
            if (byte0 == 0x00) { if (pwc) *pwc = L'\0'; return 0; }

            if (byte0 <= 0x7F) {
                scalarCodePoint = byte0;
                bytesConsumed = 1;
            } else if ((byte0 & 0xE0) == 0xC0) {
                if (n < 2) return (size_t)-2;
                scalarCodePoint = ((byte0 & 0x1F) << 6) | (s[1] & 0x3F);
                bytesConsumed = 2;
            } else if ((byte0 & 0xF0) == 0xE0) {
                if (n < 3) return (size_t)-2;
                scalarCodePoint = ((byte0 & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
                bytesConsumed = 3;
            } else if ((byte0 & 0xF4) == 0xF0) {
                if (n < 4) return (size_t)-2;
                scalarCodePoint = ((byte0 & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
                bytesConsumed = 4;
            } else {
                errno = EILSEQ; return (size_t)-1;
            }
            break;
        }
        case ENCODING_UTF16_BE:
        case ENCODING_UTF16_LE: {
            if (n < 2) return (size_t)-2;
            unsigned short unit1 = (encoding == ENCODING_UTF16_BE) ?
                ((unsigned char)s[0] << 8) | (unsigned char)s[1] :
                ((unsigned char)s[1] << 8) | (unsigned char)s[0];

            if (unit1 == 0) { if (pwc) *pwc = L'\0'; return 0; }

            if (unit1 >= 0xD800 && unit1 <= 0xDBFF) {
                if (n < 4) return (size_t)-2;
                unsigned short unit2 = (encoding == ENCODING_UTF16_BE) ?
                    ((unsigned char)s[2] << 8) | (unsigned char)s[3] :
                    ((unsigned char)s[3] << 8) | (unsigned char)s[2];

                if (unit2 >= 0xDC00 && unit2 <= 0xDFFF) {
                    scalarCodePoint = 0x10000 + (((unit1 & 0x03FF) << 10) | (unit2 & 0x03FF));
                    bytesConsumed = 4;
                } else {
                    errno = EILSEQ; return (size_t)-1;
                }
            } else {
                scalarCodePoint = unit1;
                bytesConsumed = 2;
            }
            break;
        }
        case ENCODING_UTF32_BE:
        case ENCODING_UTF32_LE: {
            if (n < 4) return (size_t)-2;
            uint32_t codePoint = (encoding == ENCODING_UTF32_BE) ?
                ((unsigned char)s[0] << 24) | ((unsigned char)s[1] << 16) | ((unsigned char)s[2] << 8) | (unsigned char)s[3] :
                ((unsigned char)s[3] << 24) | ((unsigned char)s[2] << 16) | ((unsigned char)s[1] << 8) | (unsigned char)s[0];

            if (codePoint == 0) { if (pwc) *pwc = L'\0'; return 0; }
            scalarCodePoint = codePoint;
            bytesConsumed = 4;
            break;
        }
    }

    if (scalarCodePoint >= 0x10000 && sizeof(wchar_t) == 2) {
        uint32_t adjusted = scalarCodePoint - 0x10000;
        wchar_t highSurrogate = (wchar_t)((adjusted >> 10) + 0xD800);
        wchar_t lowSurrogate  = (wchar_t)((adjusted & 0x03FF) + 0xDC00);

        if (pwc) *pwc = highSurrogate;
        state->pendingLowSurrogate = lowSurrogate;
        return bytesConsumed;
    }

    if (pwc) *pwc = (wchar_t)scalarCodePoint;
    return bytesConsumed;
}

/**
 * @brief Wide character to multibyte character for ASCII
 *
 * @param[out] s Multibyte buffer
 * @param[in] wc Wide character
 * @param[in|out] Internal state
 * @returns Number converted (or < 0 for error and errno set)
 */
size_t
__wcrtomb_a(char *s, wchar_t wc, mbstate_t *ps)
{
    ATHD_t *atp = athdp();
    mbstate_a_t *state = (ps == NULL) ? &atp->mb : (mbstate_a_t *)ps;
    if (s == NULL) return 1;

    unicodeEncoding_t encoding = deduceEncodingFromCurrentLocale();
    uint32_t fullCodePoint = wc;

    if (sizeof(wchar_t) == 2) {
        if (wc >= 0xD800 && wc <= 0xDBFF) {
            state->pendingHighSurrogate = wc;
            return 0;
        }
        if (wc >= 0xDC00 && wc <= 0xDFFF) {
            if (state->pendingHighSurrogate == 0) {
                errno = EILSEQ; return (size_t)-1;
            }
            fullCodePoint = 0x10000 + (((state->pendingHighSurrogate & 0x03FF) << 10) | (wc & 0x03FF));
            state->pendingHighSurrogate = 0;
        }
    }

    if (encoding == ENCODING_ASCII_EXTENDED) {
        if (fullCodePoint == 0) { s[0] = '\0'; return 1; }
        if (fullCodePoint > 0xFF) { errno = EILSEQ; return (size_t)-1; }
        s[0] = (char)(fullCodePoint & 0xFF);
        return 1;
    }

    if (encoding == ENCODING_UTF8) {
        if (fullCodePoint == 0) { s[0] = '\0'; return 1; }
        if (fullCodePoint <= 0x7F) {
            s[0] = (char)fullCodePoint;
            return 1;
        } else if (fullCodePoint <= 0x7FF) {
            s[0] = (char)(0xC0 | ((fullCodePoint >> 6) & 0x1F));
            s[1] = (char)(0x80 | (fullCodePoint & 0x3F));
            return 2;
        } else if (fullCodePoint <= 0xFFFF) {
            s[0] = (char)(0xE0 | ((fullCodePoint >> 12) & 0x0F));
            s[1] = (char)(0x80 | ((fullCodePoint >> 6) & 0x3F));
            s[2] = (char)(0x80 | (fullCodePoint & 0x3F));
            return 3;
        } else if (fullCodePoint <= 0x10FFFF) {
            s[0] = (char)(0xF0 | ((fullCodePoint >> 18) & 0x07));
            s[1] = (char)(0x80 | ((fullCodePoint >> 12) & 0x3F));
            s[2] = (char)(0x80 | ((fullCodePoint >> 6) & 0x3F));
            s[3] = (char)(0x80 | (fullCodePoint & 0x3F));
            return 4;
        }
    }

    errno = EILSEQ;
    return (size_t)-1;
}

/**
 * @brief Multibyte to wide character (ASCII)
 *
 * @param[out] pwc Wide character buffer
 * @param[in] s Multibytes to convert
 * @param[in] n Size of multibytes to be read
 * @returns Number of bytes that complete the multibyte character
 */
int
__mbtowc_a(wchar_t *pwc, const char *s, size_t n)
{
    ATHD_t *atp = athdp();

    if (s == NULL) {
        __mbrtowc_a(NULL, NULL, 0, (mbstate_t *)&atp->mb);
        return 0; // Non-zero if stateful, 0 if stateless
    }

    size_t res = __mbrtowc_a(pwc, s, n, (mbstate_t *)&atp->mb);
    if (res == (size_t)-1 || res == (size_t)-2) return -1;
    return (int)res;
}

/**
 * @brief Wide character to multibyte (ASCII)
 *
 * @param[out] s Multibyte buffer
 * @param[out] wc Wide character buffer
 * @param[in] n Size of wide characters to be read
 * @returns Length of multibyte character
 */
int
__wctomb_a(char *s, wchar_t wc)
{
    ATHD_t *atp = athdp();
    if (s == NULL) return 0;
    size_t res = __wcrtomb_a(s, wc, (mbstate_t *)&atp->mb);
    if (res == (size_t)-1) return -1;
    return (int)res;
}

/**
 * @brief Return length of multibyte character (ASCII)
 *
 * @param[in] s Multibyte buffer
 * @param[in] n Maximum number of bytes to examine
 * @returns Length of multibyte (or < 0 if error)
 */
int
__mblen_a(const char *s, size_t n)
{
    return __mbtowc_a(NULL, s, n);
}

/**
 * @brief Explicit state version of mblen()
 *
 *
 * @param[in] s Multibyte buffer
 * @param[in] n Maximum number of bytes to examine
 * @param[in|out] ps Shift state
 * @returns Length of multibyte (or < 0 if error)
 */
size_t
__mbrlen_a(const char *s, size_t n, mbstate_t *ps)
{
    return __mbrtowc_a(NULL, s, n, ps);
}

/**
 * @brief Explicit state version of mbstowcs()
 *
 * @param[out] dst Wide character buffer
 * @param[in] src Multibyte character buffer
 * @param[in] len Maximum number of codes to store
 * @param[in] ps Shift state
 * @returns Number of modified pwc array elements, not counting the terminating 0 code
 * which is 0 if pwc is a null pointer or -1 if error.
 */
size_t
__mbsrtowcs_a(wchar_t *dst, const char **src, size_t len, mbstate_t *ps)
{
    if (src == NULL || *src == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }
    const char *s = *src;
    size_t wcharsWritten = 0;

    while (1) {
        wchar_t wc = 0;
        size_t bytesConsumed = __mbrtowc_a(&wc, s, 4, ps);

        if (bytesConsumed == (size_t)-1) return (size_t)-1;
        if (bytesConsumed == (size_t)-2) { errno = EILSEQ; return (size_t)-1; }
        if (bytesConsumed == 0 && wc == L'\0') {
            if (dst && wcharsWritten < len) dst[wcharsWritten] = L'\0';
            *src = NULL;
            return wcharsWritten;
        }

        if (dst) {
            if (wcharsWritten >= len) {
                *src = s;
                return wcharsWritten;
            }
            dst[wcharsWritten] = wc;
        }

        wcharsWritten++;
        s += bytesConsumed;
    }
}

/**
 * @brief Multibyte string to wide character string (ASCII)
 *
 * @param[out] dst Wide character buffer
 * @param[in] src Multibyte character buffer
 * @param[in] len Maximum number of codes to store
 * @returns Number of modified pwc array elements, not counting the terminating 0 code
 * which is 0 if pwc is a null pointer or -1 if error.
 */
size_t
__mbstowcs_a(wchar_t *dst, const char *src, size_t len)
{
    if (src == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }
    const char *srcPtr = src;
    mbstate_a_t localState = {0, 0};
    return __mbsrtowcs_a(dst, &srcPtr, len, (mbstate_t *)&localState);
}

/** 
 * @brief Convert wide character string to multibyte string (explicit state)
 *
 * @param[out] dst Multibyte character buffer
 * @param[in] src Wide character buffer
 * @param[in] len Size of multibyte buffer
 * @param[in] ps Shift state
 * @returns the number of bytes in the resulting multibyte character sequence -1 if error.
 */
size_t 
__wcsrtombs_a(char *dst, const wchar_t **src, size_t len, mbstate_t *ps)
{
    if (src == NULL || *src == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }
    const wchar_t *wcs = *src;
    size_t bytesWritten = 0;
    char tempBuffer[8];

    while (*wcs != L'\0') {
        size_t bytesProduced = __wcrtomb_a(tempBuffer, *wcs, ps);
        if (bytesProduced == (size_t)-1) return (size_t)-1;

        if (dst) {
            if (bytesWritten + bytesProduced > len) {
                *src = wcs;
                return bytesWritten;
            }
            for (size_t i = 0; i < bytesProduced; i++) {
                dst[bytesWritten + i] = tempBuffer[i];
            }
        }

        bytesWritten += bytesProduced;
        wcs++;
    }

    if (dst && bytesWritten < len) {
        dst[bytesWritten] = '\0';
    }

    *src = NULL;
    return bytesWritten;
}

/** 
 * @brief Convert wide character string to multibyte string (explicit state)
 *
 * @param[out] dst Multibyte character buffer
 * @param[in] src Wide character buffer
 * @param[in] len Size of multibyte buffer
 * @param[in] ps Shift state
 * @returns the number of bytes in the resulting multibyte character sequence -1 if error.
 */
size_t
__wcstombs_a(char *dst, const wchar_t *src, size_t len)
{
    if (src == NULL) {
        errno = EINVAL;
        return (size_t)-1;
    }
    const wchar_t *srcPtr = src;
    mbstate_a_t localState = {0, 0};
    return __wcsrtombs_a(dst, &srcPtr, len, (mbstate_t *)&localState);
}
