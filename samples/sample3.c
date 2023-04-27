/********************************************************************/
/*																	*/
/* Name		: 	tstctype.c											*/	
/* 																	*/
/* Function :	Test case for ctype_a.c/h changes         			*/
/*																	*/
/* Notes	:	None                                            	*/
/*																	*/ 
/********************************************************************/

#include <ctype.h>
#include <dirent.h>
#include <dll.h>
/*
#include <dynit.h>
*/
#include <fcntl.h>
#include <grp.h>
#include <iconv.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <langinfo.h>
#include <locale.h>
#include <netdb.h>
#include <pwd.h>
#include <regex.h>
#include <rexec.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <sys/utsname.h>
#include "_Ascii_a.h"

#define UPPER_LIMIT 0xFF

/*********************************************************************
*
*	Proto-type statements   
*
*********************************************************************/
void	Cmovmem(void *s, void *d, int size); 

/*%PAGE																*/
/*********************************************************************
*
*	Name   	 :	main
*	Function :	Main entry point for test routine
*
*********************************************************************/
 
void main(int argc, char * argv[], char * env[])
{
	int	ch;
	char	Yes[]="  x  ";
	char	No[] ="     ";

	printf("\n");
#ifdef TSTMAC
	printf("\nTSTCTYPE --  Test cases for CTYPE as MACRO\n\n");
#else
	printf("\nTSTCTYPE --  Test cases for CTYPE as FUNCTION\n\n");
#endif

	printf("001-isalnum tests...\n");
	printf("001a-isalnum(a,z,A,Z,0,9) = %d,%d,%d,%d,%d,%d\n",
		isalnum('a'),isalnum('z'),
		isalnum('A'),isalnum('Z'),
		isalnum('0'),isalnum('9'));
	printf("001b-isalnum(@,#) = %d,%d\n",
		isalnum('@'),isalnum('#'));
	printf("\n");

	printf("002-isalpha tests...\n");
	printf("002a-isalpha(a,z,A,Z) = %d,%d,%d,%d\n",
		isalpha('a'),isalpha('z'),
		isalpha('A'),isalpha('Z'));
	printf("002b-isahpha(0,9) = %d,%d\n",
		isalpha('0'),isalpha('9'));
	printf("\n");

	/* iscntrl tests here											*/
	printf("003-iscntrl tests...\n");
	printf("\n");

	printf("004-isdigit tests...\n");
	printf("004a-isdigit(0,9) = %d,%d\n",
		isdigit('0'),isdigit('9'));
	printf("004b-isdigit(a,Z) = %d,%d\n",
		isdigit('a'),isdigit('Z'));
	printf("\n");

	/* isgraph test here											*/
	printf("005-isgraph tests...\n");
	printf("\n");

	printf("006-islower tests...\n");
	printf("006a-islower(a,z) = %d,%d\n",
		islower('a'),islower('z'));
	printf("006b-islower(A,rZz) = %d,%d\n",
		islower('A'),islower('Z'));
	printf("\n");

	/* isprint test here											*/
	printf("007-isprint tests...\n");
	printf("\n");

	/* ispunct test here											*/
	printf("008-ispunct tests...\n");
	printf("\n");

	printf("009-isspace tests...\n");
	printf("009a-isspace( ) = %d\n",
		isspace(' '));
	printf("009b-isspace(a,z,A,Z) = %d,%d,%d,%d\n",
		isspace('a'),isspace('z'),
		isspace('A'),isspace('Z'));
	printf("\n");

	printf("010-isupper tests...\n");
	printf("010a-isupper(A,Z) = %d,%d\n",
		isupper('A'),isupper('Z'));
	printf("010b-isupper(a,z) = %d,%d\n",
		isupper('a'),isupper('z'));
	printf("\n");

	/* isxdigit test here											*/
	printf("011-isdigit tests...\n");
	printf("\n");

	printf("012-tolower tests...\n");
	printf("012a-tolower(A,Z) = %c,%c\n",
		tolower('A'),tolower('Z'));
	printf("012b-tolower(a,z,0,9) = %c,%c,%c,%c\n",
		tolower('a'),tolower('z'),
		tolower('0'),tolower('9'));
	printf("\n");

	printf("013-toupper tests...\n");
	printf("013a-toupper(a,z) = %c,%c\n",
		toupper('a'),toupper('z'));
	printf("013b-toupper(A,Z,0,9) = %c,%c,%c,%c\n",
		toupper('A'),toupper('Z'),
		toupper('0'),toupper('9'));
	printf("\n\n");


	printf("Table of character attributes\n\n");
	printf("VAL XVAL C ALNUM ALPHA CNTRL DIGIT GRAPH LOWER PUNCT SPACE PRINT UPPER HEX\n");
	for (ch=0;ch<=UPPER_LIMIT;++ch) {
		printf("%3d %#04x", ch,ch);
		printf(" %c ",	isprint(ch)  ? ch		: ' ');
		printf("%5s ",	isalnum(ch)  ? "Alnum"	: " "); 
		printf("%5s ",	isalpha(ch)  ? "Alpha" 	: " ");
		printf("%5s ",	iscntrl(ch)  ? "Cntrl"	: " ");
		printf("%5s ",	isdigit(ch)  ? "Digit"	: " ");
		printf("%5s ",	isgraph(ch)  ? "Graph"	: " ");
		printf("%5s ",	islower(ch)  ? "Lower"	: " ");
		printf("%5s ",	ispunct(ch)  ? "Punct"	: " ");
		printf("%5s ",	isspace(ch)  ? "Space"	: " ");
		printf("%5s ",	isprint(ch)  ? "Print"	: " ");
		printf("%5s ",	isupper(ch)  ? "Upper"	: " ");
		printf("%3s ",	isxdigit(ch) ? "Hex"	: " ");
		printf("\n");
	}

    printf("TSTCTYPE.C test case end...\n");
	exit(0);
}
 
/*%PAGE																*/
/*********************************************************************
*
*	Name   	 :	Cmovmem     
*	Function :	Does memory copy
*	Notes    :	Not sure why this is required but if it's not here
*               you get unresolved link error      
*
*********************************************************************/

void Cmovmem(void *s, void *d, int size) { 
	memcpy(d,s,size);
}
