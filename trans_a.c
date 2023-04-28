/**
 * @file trans_a.c
 * @brief Various translation utility functions
 * 
 * Provides the following services:
 *	- __argvtoascii_a
 *	- __argvtoebcdic_a
 *	- __getAstring1_a
 *	- __getAstring2_a
 *	- __getEstring1_a
 *	- __getEstring2_a
 *	- __getEstring3_a
 *	- __getEstring4_a
 *	- __toascii_a
 *	- __toebcdic_a
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

#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__argvtoascii_a)
#pragma export(__argvtoebcdic_a)
#pragma export(__getAstring1_a)
#pragma export(__getAstring2_a)
#pragma export(__getEstring1_a)
#pragma export(__getEstring2_a)
#pragma export(__getEstring3_a)
#pragma export(__getEstring4_a)
#pragma export(__toascii_a)
#pragma export(__toasciilen_a)
#pragma export(__toebcdic_a)
#endif

/**
 * @brief area to hold buffers pointed to by AHD
 */
struct ebuffers {
    /* Buffers pointed to by the estringx_a pointers in ATHD          */
    char   estring1_buffer[MAXSTRING_a+1]; /* Warning this field must
                                              be first                */
    char   estring2_buffer[MAXSTRING_a+1];
    char   estring3_buffer[MAXSTRING_a+1];                          
    char   estring4_buffer[MAXSTRING_a+1];
};
typedef struct ebuffers ebuffers_t;

/**
 * @brief Converts EBCDIC argument strings to ASCII
 */
int 
__argvtoascii_a(int argc, char *argv[])
{
	int i;
	for (i=0; i < argc ; i++)   
		__toascii_a(argv[i], (const char *) argv[i]);
	return 0;
}

/**
 * @brief Converts ASCII argument strings to EBCDIC
 */
int 
__argvtoebcdic_a(int argc, char *argv[])
{
	int i;
	for (i=0; i < argc ; i++)   
		__toebcdic_a(argv[i], (const char *) argv[i]);
	return 0;
}

/**
 * @brief Returns address of an ASCII string copy of the EBCDIC input string.
 *
 * Notes:	
 *   1. May only be called once per ASCII library routine because the string is not malloc()ed.
 *   2. Multiple copies allows for limited multiple calls
 */
char *
__getAstring1_a(const char *einstr1)
{
	ATHD_t *myathdp;
	myathdp = athdp();
	strncpy(myathdp->estring1_a,einstr1,MAXSTRING_a);
	__toascii_a(myathdp->estring1_a,myathdp->estring1_a);
	return myathdp->estring1_a;
}

/**
 * @brief Returns address of an ASCII string copy of the EBCDIC input string.
 *
 * Notes:	
 *   1. May only be called once per ASCII library routine because the string is not malloc()ed.
 *   2. Multiple copies allows for limited multiple calls
 */
char *
__getAstring2_a(const char *einstr2)
{
	ATHD_t *myathdp;
	myathdp = athdp();
	strncpy(myathdp->estring2_a,einstr2,MAXSTRING_a);
	__toascii_a(myathdp->estring2_a,myathdp->estring2_a);
	return myathdp->estring2_a;
}

/**
 * @brief Returns address of an EBCDIC string copy of the ASCII input string.
 *
 * Notes:	
 *   1. May only be called once per ASCII library routine because the string is not malloc()ed.
 *   2. Multiple copies allows for limited multiple calls
 */
char *
__getEstring1_a(const char *ainstr1)
{
	ATHD_t *myathdp;
	myathdp = athdp();
	strncpy(myathdp->estring1_a,ainstr1,MAXSTRING_a);
	__toebcdic_a(myathdp->estring1_a,myathdp->estring1_a);
	return myathdp->estring1_a;
}

/**
 * @brief Returns address of an EBCDIC string copy of the ASCII input string.
 *
 * Notes:	
 *   1. May only be called once per ASCII library routine because the string is not malloc()ed.
 *   2. Multiple copies allows for limited multiple calls
 */
char *
__getEstring2_a(const char *ainstr2)
{
	ATHD_t *myathdp;
	myathdp = athdp();
	strncpy(myathdp->estring2_a,ainstr2,MAXSTRING_a);
	__toebcdic_a(myathdp->estring2_a,myathdp->estring2_a);
	return myathdp->estring2_a;
}

/**
 * @brief Returns address of an EBCDIC string copy of the ASCII input string.
 *
 * Notes:	
 *   1. May only be called once per ASCII library routine because the string is not malloc()ed.
 *   2. Multiple copies allows for limited multiple calls
 */
char *
__getEstring3_a(const char *ainstr3)
{
	ATHD_t *myathdp;
	myathdp = athdp();
	strncpy(myathdp->estring3_a,ainstr3,MAXSTRING_a);
	__toebcdic_a(myathdp->estring3_a,myathdp->estring3_a);
	return myathdp->estring3_a;
}

/**
 * @brief Returns address of an EBCDIC string copy of the ASCII input string.
 *
 * Notes:	
 *   1. May only be called once per ASCII library routine because the string is not malloc()ed.
 *   2. Multiple copies allows for limited multiple calls
 */
char *
__getEstring4_a(const char *ainstr4)
{
	ATHD_t *myathdp;
	myathdp = athdp();
	strncpy(myathdp->estring4_a,ainstr4,MAXSTRING_a);
	__toebcdic_a(myathdp->estring4_a,myathdp->estring4_a);
	return myathdp->estring4_a;
}

/**
 * @brief Convert null-terminated EBCDIC (IBM-1047) string to ASCII (ISO8859-1).
 *
 * @param outasciistr Output ASCII string
 * @param inebcdicstr Input EBCDIC string
 */
void 
__toascii_a(char *outasciistr, const char *inebcdicstr)
{
	size_t rc;
	size_t in_size, out_size;
	ATHD_t *myathdp;
	myathdp = athdp();  /* get pointer to ATHD thread structure */
	out_size = in_size = strlen(inebcdicstr) + 1; /* plus null byte */
	rc = iconv(myathdp->cd_EtoA,(char **) &inebcdicstr, &in_size, &outasciistr, &out_size);
	if (rc == (size_t) -1)
		__panic_a("Error from iconv() in __toascii_a()");
	return;
}

/**
 * @brief Convert null-terminated ASCII (ISO8859-1) string to EBCDIC (IBM-1047)
 *
 * @param outebcdicstr Output EBCDIC string
 * @param inasciistr Input ASCII string
 */
void 
__toebcdic_a(char *outebcdicstr, const char *inasciistr)
{
	size_t rc;
	size_t in_size, out_size;
	ATHD_t *myathdp;
	myathdp = athdp();  /* get pointer to ATHD thread structure */
	out_size = in_size = strlen(inasciistr) + 1; /* plus null byte */
	rc = iconv(myathdp->cd_AtoE,(char **) &inasciistr, &in_size, &outebcdicstr, &out_size);
	if (rc == (size_t) -1)
		__panic_a("Error from iconv() in __toebcdic_a()");
	return;
}

/**
 * @brief Convert null-terminated EBCDIC (IBM-1047) string to ASCII (ISO8859-1) with length.
 *
 * @param outasciistr Output ASCII string
 * @param inebcdicstr Input EBCDIC string
 * @param insize length of input string
 */
void __toasciilen_a(char *outasciistr, const char *inebcdicstr,int insize)
{
	size_t rc;
	size_t in_size, out_size;
	ATHD_t *myathdp;
	myathdp = athdp();  /* get pointer to ATHD thread structure */
	out_size = in_size = insize;
	rc = iconv(myathdp->cd_EtoA,(char **) &inebcdicstr, &in_size, &outasciistr, &out_size);
	if (rc == (size_t) -1)
		__panic_a("Error from iconv() in __toascii_a()");
	return;
}

/*********************************************************************
*
*	Name   	 :	init_trans_a
*	Function :	Initialization routine for the translation ASCII
*				library routines.
*	Notes	 :	Sets static iconv descriptor cd_EtoA and cd_AtoE for
*				later use.
*
*********************************************************************/
void init_trans_a()
{
	ATHD_t 		*myathdp;
	ebuffers_t  *myebuffers;
	int			ebuffers_sz;
	myathdp = athdp();  /* get pointer to ATHD thread structure */
	myathdp->cd_EtoA = iconv_open("ISO8859-1", "IBM-1047"); 
	if (myathdp->cd_EtoA == (iconv_t) -1)
		__panic_a("Iconv open failed for 1047\n");
    myathdp->cd_AtoE = iconv_open("IBM-1047", "ISO8859-1"); 
	if (myathdp->cd_AtoE == (iconv_t)-1)
		__panic_a("Iconv open failed for 8859\n");

	/* -- Get space for local buffers                             */
	/* -- Set estringx_a ptrs in ATHD                             */
	ebuffers_sz = sizeof(ebuffers_t);
	myebuffers  = (ebuffers_t *) calloc(1,ebuffers_sz);

	myathdp->estring1_a = (char *)&(myebuffers->estring1_buffer); 
	myathdp->estring2_a = (char *)&(myebuffers->estring2_buffer); 
	myathdp->estring3_a = (char *)&(myebuffers->estring3_buffer); 
	myathdp->estring4_a = (char *)&(myebuffers->estring4_buffer); 

	return;
}

/**
 * @brief Termination routine for the translation ASCII library routines. 
 * 
 * This routine runs when the thread is terminated to clean up resources obtained by the
 * terminating thread.
 */
void 
term_trans(ATHD_t *athdptr)
{
	iconv_t 	lociconv;
	char 		*locstring;
	ebuffers_t *loc_ebuffers;

	/* If ..., clear so that we don't do it again					*/
	if ((lociconv = athdptr->cd_EtoA) != (iconv_t) 0) {
		athdptr->cd_EtoA = (iconv_t) 0;
		iconv_close(lociconv); /* close iconv */
	}

	/* If ..., clear so that we don't do it again					*/
	if ((lociconv = athdptr->cd_AtoE) != (iconv_t) 0) {
		athdptr->cd_AtoE = (iconv_t) 0;
		iconv_close(lociconv); /* close iconv */
	}

	/* Free ebuffers area                                          */
	if ((loc_ebuffers = (ebuffers_t *)athdptr->estring1_a) != NULL) {
		athdptr->estring1_a = NULL;
		free(loc_ebuffers);
	}
	return;
}

/**
 * @brief Determine if a file is associated with a terminal or printer device
 *
 * @param stream File stream
 * @returns 0 if not a terminal or printer device, otherwise 1
 */
int
__isTerminal(FILE *stream)
{
    fldata_t fl;
    char fName[1024];

    if (fldata(stream, (char *) &fName, &fl) != 0) 
        return ((fl.__device == __TERMINAL) || 
                (fl.__device == __PRINTER));
    else
        return (0);
}
