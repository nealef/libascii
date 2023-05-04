/**
 * @file time_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the time functions.
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

#include <time.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__asctime_a)
#pragma export(__ctime_a)
#pragma export(__gettimeofday_a)
#pragma export(__gmtime_a)
#pragma export(__gmtime_r_a)
#pragma export(__localtime_a)
#pragma export(__localtime_r_a)
#pragma export(__mktime_a)
#pragma export(__strftime_a)
#pragma export(__tzset_a)
#endif

#pragma map(__asctime_a, "\174\174A00324")
#pragma map(__ctime_a, "\174\174A00325")
#pragma map(__gettimeofday_a, "\174\174A00333")
#pragma map(__gmtime_a, "\174\174A00334")
#pragma map(__gmtime_r_a, "\174\174A00335")
#pragma map(__localtime_a, "\174\174A00336")
#pragma map(__localtime_r_a, "\174\174A00337")
#pragma map(__mktime_a, "\174\174A00338")
#pragma map(__strftime_a, "\174\174A00095")
#pragma map(__tzset_a, "\174\174A00327")

/*%PAGE																*/
/**
 * @brief Return character representation of time
 */
char *
__asctime_a(const struct tm *timeptr)
{
	char	*tmp_out;

	tmp_out = asctime(timeptr);         /* call asctime           	*/ 
	__toascii_a(tmp_out,tmp_out);       /* convert output to ascii  */
	return ((char *)tmp_out);
}

/*%PAGE																*/
/**
 * @brief Return time as a character string
 */
char *
__ctime_a(const time_t *timer)
{
	char	*tmp_out;

	tmp_out = ctime(timer);             /* call ctime				*/
	__toascii_a(tmp_out,tmp_out);       /* convert output to ascii	*/
	return ((char *)tmp_out);
}

/**
 * @brief Convert Time to Broken-Down UTC Time
 */
struct tm *
__gmtime_a(const time_t *timer)
{
	return gmtime(timer);
}

/**
 * @brief Convert Time to Broken-Down UTC Time
 */
struct tm *
__gmtime_r_a(const time_t *timer, struct tm *res)
{
    struct tm *t = gmtime(timer);
    memcpy(res, t, sizeof(res));
	return res;
}

/**
 * @brief Convert Time to Broken-Down Local Time
 */
struct tm *
__localtime_a(const time_t *timer)
{
	return localtime(timer);
}

/**
 * @brief Convert Time to Broken-Down Local Time
 */
struct tm *
__localtime_r_a(const time_t *timer, struct tm *res)
{
    struct tm *t = localtime(timer);
    memcpy(res, t, sizeof(res));
	return res;
}

/**
 *	@brief Convert local time
 *
 *	No conversion required just needed for the pragma mapping
 */
time_t 
__mktime_a(struct tm *tmptr)
{
    return mktime(tmptr);
}

/**
 *	@brief Set the timezone
 *
 *	No conversion required just needed for the pragma mapping
 */
void 
__tzset_a(void)
{
    tzset();
	return;
}

/**
 * @brief Format time into a string
 */
size_t 
__strftime_a(char *dest, size_t maxsize, const char *format, 
         const struct tm *timeptr)
{
    size_t len = strftime(dest, maxsize, (const char *) __getEstring3_a(format),
                          timeptr);
    if (len > 0)
        __toasciilen_a(dest, dest, len);
    return(len);
}
/**
 * @brief Get time of day
 */
int 
__gettimeofday_a(struct timeval *tp, struct timezone *tzp)
{
    return gettimeofday(tp, tzp);
}
