/**
 * @file spawn_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the spawn functions.
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

#include <spawn.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include "global_a.h"

#pragma export(__spawn_a)
#pragma export(__spawnp_a)

#pragma map(__spawn_a, "\174\174A00086")
#pragma map(__spawnp_a, "\174\174A00088")

/*%PAGE																*/
/**
 * @brief Spawn a process using pathname
 */
pid_t
__spawn_a(const char *pn, const int nFd, const int fdMap[],
          const struct inheritance *inherit, const char *args[],
          const char *env[])
{
    const char **newArg = NULL,
               **newEnv = NULL;
    char *newPn;
    pid_t pid;

    if (env != NULL)
        newEnv = mkNew(env);

    if (args != NULL)
        newArg = mkNew(args);

	newPn = strdup(__getEstring1_a(pn));

    pid = spawn((const char *) newPn, nFd, fdMap, inherit, 
                newArg, (const char **) newEnv);

    if (newArg != NULL)
        freeNew(newArg);

    if (newEnv != NULL)
        freeNew(newEnv);

    free(newPn);
        
	return (pid);
}

/*%PAGE																*/
/**
 * @brief Spawn a process using filename
 */
pid_t
__spawnp_a(const char *fn, const int nFd, const int fdMap[],
           const struct inheritance *inherit, const char *args[],
           const char *env[])
{
    const char **newArg = NULL,
               **newEnv = NULL;
    char *newFn;
    pid_t pid;

    if (env != NULL)
        newEnv = mkNew(env);

    if (args != NULL)
        newArg = mkNew(args);

	newFn = strdup(__getEstring1_a(fn));

    pid = spawnp((const char *) newFn, nFd, fdMap, inherit, 
                 newArg, (const char **) newEnv);

    if (newArg != NULL)
        freeNew(newArg);

    if (newEnv != NULL)
        freeNew(newEnv);

    free(newFn);
        
	return (pid);
}
