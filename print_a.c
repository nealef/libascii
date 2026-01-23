/**
 * @file print_a.c
 * @brief [xx]printf front-ends
 *
 * Contains ACSII-to-EBCDIC front end to the following functions:
 *	- fprintf
 *	- printf
 *	- snprintf
 *	- sprintf
 *	- vfprintf
 *	- vprintf
 *	- vsprintf
 * 
 * Notes	:	1) All the procedures are name "__xxxxxxxx_a" where
 *				xxxxxxxx is the name of the standard C run-time
 *				function name. Unless otherwise noted, all functions
 * 				take the same argument,produce the same output and
 *				return the same values as the standard functions.
 *
 *				2) The %n$ form of the conversion specifiers are
 *				not supported by any of the printf variations.
 */

/********************************************************************
 * Copyright:   Licensed Materials - Property of IBM.               *
 *              (C) Copyright IBM Corp. 1997.                       *
 *              All rights reserved.                                *
 ********************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#ifdef GEN_IEEE_FP
#include <ieee_md.h>
#endif
#include "global_a.h"

#pragma export(__fprintf_a)
#pragma export(__printf_a)
#pragma export(__snprintf_a)
#pragma export(__sprintf_a)
#pragma export(__vfprintf_a)
#pragma export(__vprintf_a)
#pragma export(__vsnprintf_a)
#pragma export(__vsprintf_a)

#pragma map(__fprintf_a, "\174\174A00152")
#pragma map(__printf_a, "\174\174A00118")
#pragma map(__snprintf_a, "\174\174A00433")
#pragma map(__sprintf_a, "\174\174A00153")
#pragma map(__vfprintf_a, "\174\174A00154")
#pragma map(__vprintf_a, "\174\174A00155")
#pragma map(__vsnprintf_a, "\174\174A00434")
#pragma map(__vsprintf_a, "\174\174A00156")

typedef	_Bool BOOL;

#define	FALSE	0
#define	TRUE	!FALSE

/**
 *	Proto-type statements   
 */
static int CvrtToEbcdic(char *, size_t, char *, va_list);

/*%PAGE																*/
/**
 * @brief ASCII front-end to fprintf
 *
 * Note: Assumes EBCDIC output for stdout and stderr and ASCII for others.   
 *
 */
int 
__fprintf_a(FILE *stream, const char *format, ...)
{
	char	buffer[MAXSTRING_a];	/* use MAXSTRING_a for size     */
	va_list	parg;			/* ptr to variable arg					*/
	int		result;			/* number of chars printed				*/

	va_start(parg, format);	
	result = CvrtToEbcdic(buffer,MAXSTRING_a,(char *)format,parg);
	va_end(parg);
	if (result > 0) { 
		/* Translate to ASCII if not stdxxx							*/
		if (stream != stderr && stream != stdout)
			__toasciilen_a(buffer,buffer,result);
		result = fwrite(buffer,1,result,stream);
    }
	return(result);
}

/**
 * @brief ASCII front-end to printf
 *
 * Note: Assumes EBCDIC output
 *
 */
int 
__printf_a(const char *format, ...)
{
	char	buffer[MAXSTRING_a];	/* use MAXSTRING_a for size     */
	va_list	parg;			/* ptr to variable arg					*/
	int		result;			/* number of chars printed				*/
	int		i;				/*										*/

	va_start(parg, format);	
	result = CvrtToEbcdic(buffer,MAXSTRING_a,(char *)format,parg);
	va_end(parg);
	if (result > 0)
		result = fwrite(buffer,1,result,stdout);
	return(result);
}

/**
 * @brief ASCII front-end to sprintf
 *
 * Note: Assumes ASCII output
 *
 */
int 
__sprintf_a(char *buffer, const char *format, ...)
{
	va_list	parg;			/* ptr to variable arg					*/
	int		result;			/* number of chars printed				*/

	va_start(parg, format);	
	result = CvrtToEbcdic(buffer,MAXSTRING_a,(char *)format,parg);
	va_end(parg);
	if (result >= 0)
		__toasciilen_a(buffer,buffer,result);
	return(result);
}

/**
 * @brief ASCII front-end to snprintf
 *
 * Note: Assumes ASCII output
 *
 */
int 
__snprintf_a(char *buffer, size_t maxlen, const char *format, ...)
{
	va_list	parg;			/* ptr to variable arg					*/
	int		result;			/* number of chars printed				*/

	va_start(parg, format);	
	result = CvrtToEbcdic(buffer,maxlen,(char *)format,parg);
	va_end(parg);
	if (result >= 0)
		__toasciilen_a(buffer,buffer,result);
	return(result);
}

/**
 * @brief ASCII front-end to vfprintf
 *
 * Note: Assumes EBCDIC output for stdout and stderr and ASCII for others.   
 *
 */
int 
__vfprintf_a(FILE *stream, const char *format, va_list parg)
{
	char	buffer[MAXSTRING_a];	/* use MAXSTRING_a for size     */
	int		result;			/* number of chars printed				*/

	result = CvrtToEbcdic(buffer,MAXSTRING_a,(char *)format,parg);
	if (result > 0) {
		/* Translate to ASCII if not stdxxx							*/
		if (stream != stderr && stream != stdout)
			__toasciilen_a(buffer,buffer,result);
		result = fwrite(buffer,1,result,stream);
	}
	return(result);
}

/**
 * @brief ASCII front-end to vprintf
 *
 * Note: Assumes EBCDIC output
 *
 */
int 
__vprintf_a(const char *format, va_list parg)
{
	char	buffer[MAXSTRING_a];	/* use MAXSTRING_a for size     */
	int		result;			/* number of chars printed				*/
	int		i;			

	result = CvrtToEbcdic(buffer,MAXSTRING_a,(char *)format,parg);
	if (result > 0) 
		result = fwrite(buffer,1,result,stdout);
	return(result);
}

/**
 * @brief ASCII front-end to vsprintf
 *
 * Note: Assumes ASCII output
 *
 */
int 
__vsprintf_a(char *buffer, const char *format, va_list parg)
{
	int		result;			/* number of chars printed				*/

	result = CvrtToEbcdic(buffer,MAXSTRING_a,(char *)format,parg);
	if (result > 0) 
		__toasciilen_a(buffer,buffer,result);
	return(result);
}

/**
 * @brief ASCII front-end to vsnprintf
 *
 * Note: Assumes ASCII output
 *
 */
int 
__vsnprintf_a(char *buffer, size_t maxlen, const char *format, va_list parg)
{
	int		result;			/* number of chars printed				*/

	result = CvrtToEbcdic(buffer,maxlen,(char *)format,parg);
	if (result > 0) 
		__toasciilen_a(buffer,buffer,result);
	return(result);
}

/*%PAGE																*/
/**
 * @brief Converts format string and all character string
 * arguments to the printf-familiy functions from
 * ASCII to EBCDIC then performs the requested service.
 *
 * @param buffer output buffer containing EBCDIC buffer
 * @param buffersize maximum size of buffer
 * @param eformat ASCII input buffer   
 * @param optional argument porinter
 * @returns Integer value containing the number of bytes  
 *			transmitted excluding the null terminating byte or 
 *			a negative value in the case of an error.
 */
int 
CvrtToEbcdic(char *buffer, size_t buffersize, char *format, va_list parg)
 {
	char 		*ip, *op, *argstrg, *eformatp;
	char 		nchar[MAXSTRING_a];
	char		eformat[MAXSTRING_a];
	char		tmpstring[50];
	char 		fmtstring[30];
	char		numstring[10];
	char		typeQual;
	int			astValue[2];     
	int			astCount;
	int			precision;
	int 		i;
	int 		ncp, ccp;
	int			tmp1, tmp2;
	int 		tmpint;
	int 		argint;
	int			ast = '*';
	size_t 		period;
	size_t		lenformat;
	char *		result;
	const char	listofQual[] = "hlL"; 	
	const char 	listofFlags[] = "-+0'# ";
	const char	listofTypes[] = "sScCdioxXufeEgGpn";
	const int	sizeQual  = sizeof(listofQual)-1;
	const int	sizeFlags = sizeof(listofFlags)-1;
	const int	sizeTypes = sizeof(listofTypes)-1;
	double		argdbl;
	long double		argld;
#ifdef GEN_IEEE_FP
	double_t	tmpIEEEdbl;
	float_t		tmpIEEEflt;
	double		tmp390dbl;
#endif
	float		argfp;
	void 		*argptr;
	BOOL     	sw_error;
	BOOL      	sw_noMoreFlags, sw_getWidth, sw_getPrecision;

	/* Copy format string to temp buffer and convert to EBCDIC		*/
	lenformat = strlen(format);
	if (lenformat < sizeof(eformat) )
		eformatp = &eformat[0];
	else
		eformatp = malloc(lenformat+1);
	__toebcdic_a(eformatp,format);

	op = &buffer[0];
	ncp = 0;
	sw_error = FALSE;
	
	/* Scan thru buffer till error, max-output or EOL				*/
	for (ip=&eformat[0] ; !sw_error && ncp<buffersize && *ip != '\0';) {
		sw_noMoreFlags = sw_getWidth = sw_getPrecision = FALSE;

		/* If input character not % then just move it to output   	*/
		if (*ip != '%') {
			*op = *ip;
			ip++;
			op++;
			ncp++;
			continue;
		}

		/* At this point we have % followed by something			*/
		i = astCount = precision = 0;

		/* Scan for one or more "flags"								*/
		sw_noMoreFlags = FALSE;   
		while (!sw_noMoreFlags) {
			if ((result=(char *)memchr(listofFlags,ip[i+1],sizeFlags))!=NULL)
				++i;
			else
				sw_noMoreFlags = TRUE;    
	 	}

		/* Scan for width value 									*/
	 	if (ip[i+1] == '*') { 
			sw_getWidth = TRUE;   
			i++;
		}
		else  
			for (;isdigit(ip[i+1]); ++i);

		/* If next char is . scan for precision value				*/
		period = 0;
	 	if (ip[i+1] == '.') {
			period = ++i;
	 		if (ip[i+1] == '*') {
				sw_getPrecision = TRUE;   
				i++;
			}
			else  
				for (precision = 0; isdigit(ip[i+1]); ++i) 
					precision = precision * 10 + (ip[i+1] - '0');
		}

		/* Scan for h,l or L										*/
		typeQual = ' ';
		if ((result = (char *)memchr(listofQual,ip[i+1],sizeQual)) != NULL) {
			typeQual = ip[i+1];
			i++;
		}

		/* If next char is valid format char get Width & Precision	*/
		/* from args if specified there								*/
		if ((result=(char *)memchr(listofTypes,ip[i+1],sizeTypes))!=NULL) {
			if (sw_getWidth)
				astValue[astCount++] = va_arg(parg, int);
			if (sw_getPrecision) {
				precision = va_arg(parg, int);
				astValue[astCount++] = precision;
			}
		}

		/* Scan for "type" character								*/
		i++;
		if (i > 1 + sizeof(fmtstring)) {
			sw_error = TRUE; 
			continue;
		}

		/* Copy formattting string to temp buffer                 	*/ 
		strncpy(fmtstring,ip,i+1);
		fmtstring[i+1] = '\0';

		/* If string contained any *s, do substitution now			*/
		if (astCount > 0) {
			tmpstring[0] = '\0';
			for (astCount=tmp1=tmp2=0;fmtstring[tmp1]!='\0';tmp1++) {
				if (fmtstring[tmp1] != '*') {
					tmpstring[tmp2++] = fmtstring[tmp1];
					tmpstring[tmp2] = '\0';
				}
				else {
					sprintf(numstring,"%i\0",astValue[astCount++]);
					strcat(tmpstring,numstring);
					tmp2 = strlen(tmpstring);
				}
			}
			strcpy(fmtstring,tmpstring);
		}

		/* Process based of conversion specification character		*/
		switch (ip[i]) {

			/* Input variable is a string							*/
			case 's':
			case 'S':
				/* If period found, only move "precision" chars		*/
				/* of the variable string -- up to buffer max		*/
				argstrg = va_arg(parg, char *);
				if (period == 0 
  					|| precision <= 0
	 				|| precision >= sizeof(nchar))
					precision = sizeof(nchar)-1;
				strncpy(nchar,argstrg,precision);
				nchar[precision] = '\0';
				__toebcdic_a(nchar,nchar);
				ccp = sprintf(op,fmtstring,nchar);
				if (ccp == -1) {
					sw_error = TRUE;
					continue;
				}
				break;

			/* Input variable is single character 					*/
			case 'c':
			case 'C':
				argint = va_arg(parg, int);
				nchar[0] = argint;
				nchar[1] = '\0';
				__toebcdic_a(nchar,nchar);
				tmpint = nchar[0];
				ccp = sprintf(op,fmtstring,tmpint);
				if (ccp == -1) {
					sw_error = TRUE;
					continue;
				}
				break;

			/* Input variable is integer							*/
			case 'd':
			case 'i':
			case 'o':
			case 'x':
			case 'X':
			case 'u':
				argint = va_arg(parg, int);
				ccp = sprintf(op,fmtstring,argint);
				if (ccp == -1) {
					sw_error = TRUE;
					continue;
				}
				break;

			/* Input variable is double        						*/ 
			case 'f':
			case 'e':
			case 'E':
			case 'g':
			case 'G':
				switch (typeQual) {
					case 'h':
						argdbl = va_arg(parg,double);
						ccp = sprintf(op,fmtstring,argdbl);
						if (ccp == -1) {
							sw_error = TRUE;
							continue;
						}
						break;
					case 'L':
						argld = va_arg(parg,long double);
						ccp = sprintf(op,fmtstring,argld);
						if (ccp == -1) {
							sw_error = TRUE;
							continue;
						}
						break;
					default:
						argdbl = va_arg(parg, double);
						ccp = sprintf(op,fmtstring,argdbl);
						if (ccp == -1) {
							sw_error = TRUE;
							continue;
						}
						break;
					}
				break;

			/* Input variable is pointer       						*/ 
			case 'p':
				argptr = va_arg(parg, void *);
				ccp = sprintf(op,fmtstring,argptr);
				if (ccp == -1) {
					sw_error = TRUE;
					continue;
				}
				break;

			/* Input variable is pointer to integer 				*/
			/* Store number of characters printed so far in integer	*/	
			case 'n':
				switch (typeQual) {
					case 'h':
						*(va_arg(parg, short int *)) = ncp; 
						break;
					case 'l':
						*(va_arg(parg, long int *)) = ncp; 
						break;
					default :
  						*(va_arg(parg, int *)) = ncp;   
						break;
				}
				ccp = 0;
				i = 1;
				break;

	 		/* % character was followed by a character that is not 	*/
			/* a formatting code, ignore the % and print following 	*/
			/* char													*/
			default:
				ip++;
				*op = *ip;
				ccp = 1;
				i = 0;
				break;
			}

			/* Incr input & output ptrs and count of chars printed	*/
			ip += i+1;
			op += ccp;
			ncp += ccp;
	}

	/* Add end-of-string char to buffer								*/
	buffer[ncp] = '\0';

	/* Free temp buffer if was was obtained							*/
	if (lenformat >= sizeof(eformat))
		free(eformatp);

	/* Return with rc=-1 is error or num chars printed if all OK	*/
	if (sw_error)  
		return(-1);
	else
		return(ncp);
}
