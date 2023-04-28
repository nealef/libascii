/**
 * @file dlfcn_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the dlfcn.h functions.
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

#include <dlfcn.h>
#include "global_a.h"

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__dlerror_a)
#pragma export(__dlopen_a)
#pragma export(__dlsym_a)
#endif

#pragma map (__dlerror_a, "\174\174A00438")
#pragma map (__dlopen_a, "\174\174A00436")
#pragma map (__dlsym_a, "\174\174A00437")

/*%PAGE																*/
/**
 * @brief Retrieve dll error message
 */
char *
__dlerror_a()
{
    char *err = dlerror();
    if (err)
        __toascii_a(err, err);
    return err;
}

/**
 * @brief Open a DLL
 */
void *
__dlopen_a(const char *file, int mode)
{
    return dlopen((const char *) __getEstring1_a(file), mode);
}

/**
 * @brief Locate a symbol within DLL
 */
void *
__dlsym_a(void *__restrict__ handle, const char *__restrict__ name)
{
    return dlsym(handle, (const char *) __getEstring2_a(name));
}
