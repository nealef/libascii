/**
 * @file override.c
 * @brief Contains stubs to call write/read
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
#include <stdarg.h>
#include <stdio.h>
 
#pragma map(__getc_a, "GETCOVRA")
#pragma map(__write_a, "WRITOVRA")
#pragma map(__read_a, "READOVRA")
#pragma map(__close_a, "CLOSOVRA")
#pragma map(__ungetc_a, "UGTCOVRA")

/*%PAGE																*/
/**
 * @brief Override write() API 
 *
 */
ssize_t 
__write_a(int fd, const void *buf, size_t len)
{
    return write(fd, buf, len);
}

/**
 * @brief Override read() API 
 *
 */
ssize_t 
__read_a(int fd, void *buf, size_t len)
{
    return read(fd, buf, len);
}

/**
 * @brief Override close() API 
 *
 */
int
__close_a(int fd)
{
    return close(fd);
}

/**
 * @brief Override getchar() API
 *
 */
int
__getchar_a()
{
    return getchar();
}

/**
 * @brief Override getc() API
 *
 */
int
__getc_a(FILE *s)
{
    return getc(s);
}

/**
 * @brief Override ungetc() API
 *
 */
int
__ungetc_a(int c, FILE *s)
{
    return ungetc(c, s);
}
