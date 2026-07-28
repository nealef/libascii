/**
 * @file syslog_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the syslog functions.
 * 
 * Notes	:	All the procedures are name "__xxxxxxxx_a" where
 *				xxxxxxxx is the name of the standard C run-time
 *				function name. Unless otherwise noted, all functions
 * 				take the same argument,produce the same output and
 *				return the same values as the standard functions.
 */

#pragma langlvl( extended )
 
#include <syslog.h>
#include "global_a.h"

#pragma export(__syslog_a)
#pragma export(__openlog_a)

#pragma map(__syslog_a,  "\174\174A00366")
#pragma map(__openlog_a, "OPENLOGA")

/**
 * @brief openlog stub
 *
 */
void 
__openlog_a(const char *ident, int option, int facility)
{
    return;
}

/**
 * @brief syslog stub
 *
 */
void 
__syslog_a(int priority, const char *format, ...)
{
    return;
}

