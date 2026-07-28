/**
 * @file time_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the time functions.
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
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "global_a.h"

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
#pragma export(__tzznA_a)
#pragma export(__futimes)

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
#pragma map(__tzznA_a, "\174\174TZZNA")
#pragma map(__futimes, "futimes")

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
    memcpy(res, t, sizeof(*res));
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
    memcpy(res, t, sizeof(*res));
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
 *	@brief Return timezone name
 */
char **
__tzznA_a(void)
{
    extern char *tzname[2];
    static char *tzn[2] = { NULL, NULL };
    char *p;

    if (tzn[0] != NULL)
        free(tzn[0]);
    if (tzn[1] != NULL)
        free(tzn[1]);
    
    if (tzname[0] != NULL) {
        p = strdup(tzname[0]);
        __toascii_a(p, (const char *)p);
        tzn[0] = p;
    } else
        tzn[0] = NULL;

    if (tzname[1] != NULL) {
        p = strdup(tzname[1]);
        __toascii_a(p, (const char *)p);
        tzn[1] = p;
    } else 
        tzn[1] = NULL;
    
	return tzn;
}

/**
 * @brief Format time into a string
 *
 * The API doesn't support the epoch '%s' format so we have to do it
 * ourselves.
 */
size_t 
__strftime_a(char *dest, size_t maxsize, const char *format, 
         const struct tm *timeptr)
{
    size_t len;
    char *epoch;

    len = strftime(dest, maxsize, __getEstring3_a(format), timeptr);
    if ((epoch = strstr(dest, "%s"))) {
        char *res = __alloca(2 * maxsize),
             epsec[11],
             *cursor = dest,
             *eos = &dest[len];
        time_t secs = mktime((struct tm *)timeptr);
        int l = 0;

        sprintf(epsec, "%u", secs);
        memset(res, 0, (2 * maxsize));

        do {
            /*
             * Check for any leading characters
             */
            l = ((uintptr_t) epoch - (uintptr_t) cursor);
            if (l > 0)
                strncat(res, cursor, l);

            /*
             * Replace %s with epoch seconds
             */
            epoch += 2;
            cursor = epoch;
            strcat(res, epsec);

            /*
             * Check for any more %s instancs
             */
            epoch = strstr(cursor, "%s");
        } while (epoch != NULL);

        /*
         * Handle any remaining characters
         */
        if (cursor < eos)
            strcat(res, cursor);

        /*
         * Copy to result area and set length
         */
        strncpy(dest, res, maxsize - 1);
        len = strlen(dest);
    }

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

/**
 * @brief Implement futimes() API
 */
int 
__futimes(int fd, const struct timeval tv[2])
{
    FILE *stream;
    fldata_t fileInfo;
    char fileName[FILENAME_MAX];
    int rc = -1, f;

    memset(fileName, 0, sizeof(fileName));
    if ((f = dup(fd)) != -1) {
        stream = fdopen(f, "r");
        if (stream != NULL) {
            if (fldata(stream, fileName, &fileInfo) == 0) {
                if (fileName[0] != 0) {
                    rc = utimes(fileName, tv);
                } else {
                    errno = EBADF;
                }
            }
            fclose(stream);
        }
        close(f);
    }
    return rc;
}
