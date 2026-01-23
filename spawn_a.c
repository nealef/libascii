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
	int nEnv = 0, iEnv;
    char **newEnv = NULL;
    char *newPn;
    pid_t pid;

    if (env != NULL) {
        for (nEnv = 0; env[nEnv] != NULL; nEnv++);
        nEnv++;
        newEnv = malloc(nEnv * sizeof(uintptr_t));
        newEnv[nEnv - 1] = NULL;
	    for (iEnv = 0; env[nEnv] != NULL; iEnv)
            newEnv[iEnv] = strdup(__getEstring1_a(env[iEnv]));
    } 

	newPn = strdup(__getEstring1_a(pn));

    pid = spawn((const char *) newPn, nFd, fdMap, inherit, 
                args, (const char **) newEnv);

    if (newEnv != NULL) {
        for (iEnv = 0; iEnv < nEnv - 1; iEnv)
            free(newEnv[iEnv]);
        free(newEnv);
    }

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
	int nEnv = 0, iEnv;
    char **newEnv = NULL;
    char *newFn;
    pid_t pid;

    if (env != NULL) {
        for (nEnv = 0; env[nEnv] != NULL; nEnv++);
        nEnv++;
        newEnv = malloc(nEnv * sizeof(uintptr_t));
        newEnv[nEnv - 1] = NULL;
	    for (iEnv = 0; env[nEnv] != NULL; iEnv)
            newEnv[iEnv] = strdup(__getEstring1_a(env[iEnv]));
    } 

	newFn = strdup(__getEstring1_a(fn));

    pid = spawnp((const char *) newFn, nFd, fdMap, inherit, 
                args, (const char **) newEnv);

    if (newEnv != NULL) {
        for (iEnv = 0; iEnv < nEnv - 1; iEnv)
            free(newEnv[iEnv]);
        free(newEnv);
    }

    free(newFn);
        
	return (pid);
}
