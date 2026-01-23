/********************************************************************/
/*                                                                  */
/* Name		: 	_Ascii_a.h	                                        */
/*                                                                  */
/* Copyright:   Licensed Materials - Property of IBM.               */
/*              (C) Copyright IBM Corp. 1997.                       */
/*              All rights reserved.                                */
/*                   	                                        	*/
/* Function	: 	Provides prototypes and #define for ascii library  	*/
/*           	routines which support ascii input characters.     	*/
/*                   	                                        	*/
/********************************************************************/
#ifndef _Ascii_a
 #define _Ascii_a

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
   extern "C" {
#endif

int 		__argvtoascii_a(int, char *[]);
int 		__argvtoebcdic_a(int, char *[]);
char *		__getAstring1_a(const char *);
char *		__getAstring2_a(const char *);
char *		__getEstring1_a(const char *);
char *		__getEstring2_a(const char *);
char *		__getEstring3_a(const char *);
char *		__getEstring4_a(const char *);
void 		__toascii_a(char *, const char *);
void 		__toasciilen_a(char *, const char *, int);
void 		__toebcdic_a(char *, const char *);
void 		__panic_a(char *);
int         __isTerminal(FILE *);
struct ATHD * getathdp();

#ifdef __cplusplus
}
#endif

#endif
