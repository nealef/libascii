/**
 * @file stdio_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the stdio functions.
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
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <netdb.h>
#include <regex.h>
#include <sys/time.h>
#include "global_a.h"
 
#ifdef GEN_PRAGMA_EXPORT
 #pragma export(__cuserid_a)
 #pragma export(__fdopen_a)
 #pragma export(__fgets_a)
 #pragma export(__fopen_a)
 #pragma export(__fputc_a)
 #pragma export(__fprintf_a)
 #pragma export(__fputs_a)
 #pragma export(__fread_a)
 #pragma export(__freopen_a)
 #pragma export(__fwrite_a)
 #pragma export(__fwrite_allascii_a)
 #pragma export(__getc_a)
 #pragma export(__getc_ascii_a)
 #pragma export(__getopt_a)
 #pragma export(__gets_a)
 #pragma export(__perror_a)
 #pragma export(__popen_a)
 #pragma export(__putc_a)
 #pragma export(__putchar_a)
 #pragma export(__puts_a)
 #pragma export(__remove_a)
 #pragma export(__rename_a)
 #pragma export(__setvbuf_a)
 #pragma export(__tempnam_a)
 #pragma export(__tmpnam_a)
#endif
#pragma map(__cuserid_a, "\174\174A00248")
#pragma map(__fdopen_a, "\174\174A00241")
#pragma map(__fgets_a, "\174\174A00305")
#pragma map(__fopen_a, "\174\174A00246")
#pragma map(__fprintf_a, "\174\174A00152")
#pragma map(__fputc_a, "\174\174A00302")
#pragma map(__fputs_a, "\174\174A00307")
#pragma map(__fread_a, "\174\174A00308")
#pragma map(__freopen_a, "\174\174A00247")
#pragma map(__fwrite_a, "\174\174A00309")
#pragma map(__getc_a, "\174\174A00306")
#pragma map(__getopt_a, "\174\174A00190")
#pragma map(__gets_a, "\174\174A00306")
#pragma map(__perror_a, "\174\174A00178")
#pragma map(__putc_a, "\174\174A00146")
#pragma map(__putchar_a, "\174\174A00303")
#pragma map(__puts_a, "\174\174A00304")
#pragma map(__popen_a, "\174\174A00249")
#pragma map(__remove_a, "\174\174A00243")
#pragma map(__rename_a, "\174\174A00244")
#pragma map(__tempnam_a, "\174\174A00250")
#pragma map(__tmpnam_a, "\174\174A00245")
 
#define WRKBUFSIZ _POSIX_PATH_MAX
 
/*%PAGE																*/
/**
 * ASCII front-end routines for STDIO functions
 */

/**
 * @brief Return user id string
 */
char *
__cuserid_a(char *s)
{
    char *tmp_out;

    if (s) {
        tmp_out = cuserid(__getEstring1_a(s));
    } else {
        tmp_out = cuserid(NULL);
    }
    return(__getAstring1_a(tmp_out));
}

/**
 * @brief Return stream for an open file descriptor
 */
FILE *
__fdopen_a(int fildes, const char *options)
{
	return fdopen(fildes, (const char *) __getEstring2_a(options));
}
 
/**
 * @brief Return stream for an open file descriptor
 *
 * Assume the input is in ASCII format if file, in EBCDIC if stdin 
 */
char *
__fgets_a(char *string, int n, FILE *stream)
{
	if (stream==stdin) {  /* assume the input is ebcdic */
		if (fgets(string, n, stream) != NULL) {
			__toascii_a(string, string);
			return(string);
		} else
			return(NULL);
	} else {
		int i, len;
		fpos_t pos;
		fgetpos(stream, &pos);
		if (fgets(string, n, stream) != NULL) {
			len=strlen(string);
			for (i=0; string[i]!=0x0A && i<len; i++);
			if (i==len || string[i+1]=='\0')
				return(string);
			else {
				fsetpos(stream, &pos); /* reset the stream position */
				fgets(string, i+2, stream); /* gets again but only for i+1 bytes */
				return(string);
			}
		}
		return(NULL);
	}
}
 
/**
 * @brief Open a file
 */
FILE *
__fopen_a(const char *path, const char *mode)
{
	return fopen((const char *) __getEstring1_a(path),
				 (const char *) __getEstring2_a(mode));
}
 
/**
 * @brief Put a character to a file
 *
 * Assume EBCDIC output if stdout or stderr
 */
int 
__fputc_a(int c, FILE *stream)
{
	char input_char[]=" ";      /* 2 bytes work area : ' '+'\0' */
	input_char[0]=c;
	if ((stream==stdout) || (stream == stderr))
		__toebcdic_a(input_char, input_char);
	return (fputc(input_char[0], stream));
}
 
/**
 * @brief Put a string to a file
 *
 * Assume EBCDIC output if stdout or stderr
 */
int 
__fputs_a(char *fstring, FILE *stream)
{
	int tmpint;
	/* if stdout or stderr, then convert string to ebcdic, else
       preserve in ascii 											*/
	if ((stream == stderr) || (stream == stdout)) {
        char *estring = __alloca(strlen(fstring) + 1);
		__toebcdic_a(estring,fstring);
		tmpint = fputs(estring,stream);
		return(tmpint);
	} else
	 	return(fputs(fstring,stream));
}
 
/**
 * @brief Read from a stream
 *
 * Assume ASCII input if stdin
 */
int 
__fread_a(void *buffer,size_t size,size_t count, FILE *stream)
{
	size_t tmpint;
	/* if stdin , then convert string to ascii for return          */
	if (stream == stdin) {
		tmpint = fread(buffer,size,count,stream);
		__toascii_a(buffer,buffer);       /* convert back to ascii */
		return(tmpint);
	}
	else
		return(fread(buffer,size,count,stream));
}
 
/**
 * @brief Reopen a stream
 */
FILE *
__freopen_a( const char *path, const char *mode, FILE *stream)
{
	return freopen((char const *) __getEstring1_a(path),
				   (char const *) __getEstring2_a(mode),
				   stream);
}
 
/**
 * @brief Write to a stream
 */
size_t 
__fwrite_a(const void *buffer, size_t size, size_t count, FILE *stream)
{
	size_t	bytes;
	if (stream==stderr || stream==stdout) {
        char *out = __alloca(buffer);
		__toebcdic_a((char *) out, (char *) buffer);
        bytes = fwrite(out, size, count, stream);
    } else 
        bytes = fwrite(buffer, size, count, stream);
	return bytes;
}
 
/**
 * @brief Write ASCII data to a stream
 */
size_t 
__fwrite_allascii_a(const void *buffer, size_t size, size_t count, FILE *stream)
{
	size_t bytes;
	bytes = fwrite(buffer, size, count, stream);
	return bytes;
}
 
/**
 * @brief Read a character from a stream
 *
 * Assume EBCDIC input if stdin
 */
int 
__getc_a(FILE *stream)
{
	char input_char[]=" ";      /* 2 bytes work area : ' '+'\0' */
 
	input_char[0]=getc(stream);
	if (input_char[0] == 0xff)
		return(-1);
	if (stream==stdin)
		__toascii_a(input_char, input_char);
	return (input_char[0]);
}
 
/**
 * @brief Read a character from a stream
 */
int 
__getc_ascii_a(FILE *stream)
{
	char input_char[]=" ";      /* 2 bytes work area : ' '+'\0' */
	input_char[0]=getc(stream);
	if (input_char[0] == 0xff)
		return(-1);
	return (input_char[0]);
}

/**
 * @brief Get options from an array of arguments
 *
 * The definiton of this function has been changed to remove the 	
 * const from the 2nd argument. This was done to eliminate a warning
 * message on the calls to __argvtoebcdic_a and __argvtoascii_a.
 * This code actually violates the design of the real getopt in that
 * the 2nd argument valuesare actually changed for a short period of
 * time between the two __argvtoxxx routines.
 * int __getopt_a(int argc, char *const argv[], const char *varname)
 */
int 
__getopt_a(int argc, char *argv[], const char *varname)
{
	ATHD_t *myathdp;
	char tmpvarname[80];
	char *tmpvarnamep;
	int varlen=80;
	int tmpoptp;
	char optarg_ascii[80];
	char tmpoptp_ascii[2];
	char optopt_ascii[2];
	char tmpasciibuff[2];
	char *tmpoptp_ascii_p;
	char *optopt_ascii_p;
	char *getreturnp;

	__argvtoebcdic_a(argc,argv);

	if ((varlen = 1+strlen(varname)) > 80)
		tmpvarnamep = malloc(varlen);
	else
		tmpvarnamep = &tmpvarname[0];
	__toebcdic_a(tmpvarnamep,varname); 

	tmpoptp = getopt(argc,argv,tmpvarnamep);
	__toascii_a(optarg_ascii,optarg); 

	tmpoptp_ascii_p = &tmpoptp_ascii[0];
	optopt_ascii_p = &optopt_ascii[0];

	if (varlen > 80)
		free(tmpvarnamep);
	__argvtoascii_a(argc,argv);
	sprintf(optarg,"%s",optarg_ascii);
	if (tmpoptp != -1) {
		sprintf(tmpasciibuff,"%c",tmpoptp);
		__toascii_a(tmpoptp_ascii_p,tmpasciibuff);
		if (optopt) {
			sprintf(tmpasciibuff,"%c",optopt);
			__toascii_a(optopt_ascii_p,tmpasciibuff);
			optopt = (int)optopt_ascii[0];
		}
		return((int)tmpoptp_ascii[0]);
	} else {
		return(tmpoptp);
	}
}
 
/**
 * @brief Get a string from stdin
 */
char *
__gets_a(char *buffer)
{
	if (gets(buffer) != NULL) {
		__toascii_a(buffer, buffer);
		return(buffer);
	} else
		return(NULL);
	}
 
/**
 * @brief Print the string representation of the error number
 */
void 
__perror_a( const char *s )
{
	perror((const char *) __getEstring1_a(s));
}
 
/**
 * @brief Open a pipe
 */
FILE *
__popen_a(const char *command, const char *mode)
{
	return popen((const char *) __getEstring1_a(command),
				 (const char *) __getEstring2_a(mode));
}
 
/**
 * @brief Put a character to a stream
 *
 * Assume EBCDIC output if stdout or stderr
 */
int 
__putc_a(int c, FILE *stream)
{
	char input_char[]=" ";      /* 2 bytes work area : ' '+'\0' */
 
	input_char[0]=c;
	if (stream==stdout || stream==stderr)
		__toebcdic_a(input_char, input_char);
	return (putc(input_char[0], stream));
}
 
/**
 * @brief Put a character to a file
 *
 * Assume EBCDIC output if stdout or stderr
 */
int 
__putchar_a(int c)
{
	char input_char[]=" ";      /* 2 bytes work area : ' '+'\0' */
	input_char[0]=c;
	__toebcdic_a(input_char, input_char);
	return (fputc(input_char[0], stdout));
}
 
/**
 * @brief Put a string to stdout
 */
int  
__puts_a(char *buffer)
{
	return(puts((const char *) __getEstring1_a(buffer)));
}
 
/**
 * @brief Remove a file
 */
int 
__remove_a(const char *path)
{
	return remove((const char *) __getEstring1_a(path));
}
 
/**
 * @brief Rename a file
 */
int 
__rename_a( const char *old, const char *new )
{
	return rename((const char *)__getEstring1_a(old),
			   	  (const char *)__getEstring2_a(new));
}
 
/** 
 * @brief Control Buffering
 */
int 
__setvbuf_a(FILE *stream,char *buf,int type, size_t size)
{
	return setvbuf(stream,(char *) __getEstring1_a(buf), type, size);
}

/**
 * @brief Generate a temporary file name
 */
char *
__tempnam_a(const char *dir, const char *pfx)
{
	return tempnam((const char *) __getEstring1_a(dir),
				   (const char *) __getEstring2_a(pfx));
}

/**
 * @brief Produce temporary file name
 */
char *
__tmpnam_a(char *string)
{
	char *	tmp_string;
	tmp_string = tmpnam(string);
	if (string == NULL)
		return __getAstring1_a(tmp_string);
	else
		{
			__toascii_a(string,string);
			return string;
		}
}
