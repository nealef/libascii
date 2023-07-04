/**
 * @file stdlib_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the stdlib functions.
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

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <grp.h>
#include <pwd.h>
#ifdef GEN_IEEE_FP
#include <ieee_md.h>
#endif
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
 #pragma export(__a64l_a)
 #pragma export(__atof_a)
 #pragma export(__atoi_a)
 #pragma export(__atol_a)
 #pragma export(__ecvt_a)
 #pragma export(__Envna_a)
 #pragma export(__fcvt_a)
 #pragma export(__gcvt_a)
 #pragma export(__getenv_a)
 #pragma export(__l64a_a)
 #pragma export(__mbstowcs_a)
 #pragma export(__mbtowc_a)
 #pragma export(__mkstemp_a)
 #pragma export(__mktemp_a)
 #pragma export(__putenv_a)
 #pragma export(__realpath_a)
 #pragma export(__setenv_a)
 #pragma export(__strtod_a)
 #pragma export(__strtol_a)
 #pragma export(__strtoul_a)
 #pragma export(__system_a)
 #pragma export(__unsetenv_a)
 #pragma export(__wcstombs_a)
 #pragma export(__wctomb_a)
#endif

#pragma map(__a64l_a, "\174\174A00172")
#pragma map(__atof_a, "\174\174A00164")
#pragma map(__atoi_a, "\174\174A00165")
#pragma map(__atol_a, "\174\174A00166")
#pragma map(__Envna_a, "\174\174ENVNA") 
#pragma map(__ecvt_a, "\174\174A00173")
#pragma map(__fcvt_a, "\174\174A00174")
#pragma map(__gcvt_a, "\174\174A00175")
#pragma map(__getenv_a, "\174\174A00181")
#pragma map(__l64a_a, "\174\174A00176")
#pragma map(__mbstowcs_a, "\174\174A00006")
#pragma map(__mbtowc_a, "\174\174A00008")
#pragma map(__mkstemp_a, "\174\174A00184")
#pragma map(__mktemp_a, "\174\174A00240")
#pragma map(__putenv_a, "\174\174A00186")
#pragma map(__realpath_a, "\174\174A00187")
#pragma map(__setenv_a, "\174\174A00188")
#pragma map(__strtod_a, "\174\174A00167")
#pragma map(__strtol_a, "\174\174A00168")
#pragma map(__strtoul_a, "\174\174A00169")
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
	ATHD_t *myathdp;
	char tmpvarname[80];
	char *tmpvarnamep;
	int varlen=80;
	char *tmpenvp;
	char *getreturnp;
	if ((varlen = 1+strlen(varname)) > 80)
		tmpvarnamep = __alloca(varlen);
	else
		tmpvarnamep = &tmpvarname[0];
	__toebcdic_a(tmpvarnamep,varname); /* convert ascii to ebcdic */
	tmpenvp = getenv(tmpvarnamep);
	if (tmpenvp == NULL)
		return(tmpenvp);
	myathdp = athdp();  /* get pointer to athd thread structure */
	if (myathdp->getenvlen < (varlen = 1 + strlen(tmpenvp))) {
		if (myathdp->getenvp != NULL)
			free(myathdp->getenvp);
		myathdp->getenvp = malloc(varlen);
		myathdp->getenvlen = varlen;
	}
	__toascii_a(myathdp->getenvp,tmpenvp);
	return(myathdp->getenvp); /* Return address of return buffer  */
}

/**
 * @brief Convert multibyte character to wide character
 */
int 
__mbtowc_a(wchar_t *pwc, const char *string, size_t n)
{
	return(mbtowc(pwc,(const char *) __getEstring1_a(string),n));
}                                                       

/**
 * @brief Convert multibyte characters to wide characters
 */
int 
__mbstowcs_a(wchar_t *pwc, const char *string, size_t n)       
{                                                                
	return (mbstowcs(pwc,(const char *) __getEstring1_a(string),n));
}

/**
 * @brief Convert wide character to multibyte character
 */
int 
__wctomb_a(char *pmb, wchar_t c)
{
    int len;

	len = wctomb(pmb, c);
//    if (len > 0)
//        __toasciilen_a(pmb, pmb, len);
    return(len);
}                                                       

/**
 * @brief Convert wide characters to multibyte characters
 */
size_t
__wcstombs_a(char *pmb, wchar_t *string, size_t n)       
{                                                                
    size_t len;
	len = wcstombs(pmb, string, n);
    return(len);
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
	return putenv((const char *) __getEstring1_a(envvar));
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
	return setenv((const char *) __getEstring1_a(var_name),
				 (const char *) __getEstring2_a(new_value),change_flag);
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
__strtoul_a( const char *s, char **endptr, int base )
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
 * @brief Issue a system command
 */
int 
__system_a( const char *s )
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
__a64l_a( const char *s )
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
    return setenv((const char *) __getEstring1_a(name), "", 1);
}

/**
 * @brief Return the environment in ASCII
 */
char ***
__EnvnA(void)
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

/*%PAGE																*/
/**
 * Start of routines that are not exported
 */

/**
 * @brief Thread termination routine for getenv() ASCII.
 */
void 
term_getenv(ATHD_t *athdptr)
{
	if (athdptr->getenvp != NULL)
		free(athdptr->getenvp);
	return;
}
