/**
 * @file ndbm_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the ndbm functions.
 * 
 * Notes	:	All the procedures are name "__xxxxxxxx_a" where
 *				xxxxxxxx is the name of the standard C run-time
 *				function name. Unless otherwise noted, all functions
 * 				take the same argument,produce the same output and
 *				return the same values as the standard functions.
 */

#pragma langlvl( extended )
 
#include <ndbm.h>
#include "global_a.h"

#pragma export(__dbm_open_a)

#pragma map(__dbm_open_a,   "\174\174A00381")

/**
 * @brief Open NDBM file
 */
DBM *
__dbm_open_a(const char *file, int open_flags, mode_t file_mode)
{
    return dbm_open(__getEstring1_a(file), open_flags, file_mode);
}
