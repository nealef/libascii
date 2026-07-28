/**
 * @file unistd_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the unistd functions.
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
#include <stdint.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include "global_a.h"
 
#pragma export(__access_a)
#pragma export(__chdir_a)
#pragma export(__chown_a)
#pragma export(__close_a)
#pragma export(__confstr_a)
#pragma export(__ctermid_a)
#pragma export(__execl_a)
#pragma export(__execv_a)
#pragma export(__execve_a)
#pragma export(__execvp_a)
#pragma export(__getcwd_a)
#pragma export(__gethostname_a)
#pragma export(__getlogin_a)
#pragma export(__getpass_a)
#pragma export(__getwd_a)
#pragma export(__getwd_o)
#pragma export(__link_a)
#pragma export(__pathconf_a)
#pragma export(__read_a)
#pragma export(__readlink_a)
#pragma export(__rmdir_a)
#pragma export(__symlink_a)
#pragma export(__truncate_a)
#pragma export(__truncate_o)
#pragma export(__ttyname_a)
#pragma export(__ttyname_r_a)
#pragma export(__unlink_a)
#pragma export(__write_a)
 
#pragma map(__access_a, "\174\174A00192")
#pragma map(__chdir_a, "\174\174A00193")
#pragma map(__chown_a, "\174\174A00194")
#pragma map(__close_a, "CLOSOVRA")
#pragma map(__confstr_a, "\174\174A00238")
#pragma map(__ctermid_a, "\174\174A00274")
#pragma map(__execl_a, "\174\174A00039")
#pragma map(__execv_a, "\174\174A00068")
#pragma map(__execve_a, "\174\174A00084")
#pragma map(__execvp_a, "\174\174A00085")
#pragma map(__getcwd_a, "\174\174A00196")
#pragma map(__gethostname_a, "\174\174A00260")
#pragma map(__getlogin_a, "\174\174A00261")
#pragma map(__getpass_a, "\174\174A00263")
#pragma map(__getwd_a, "\174\174A00197")
#pragma map(__getwd_o, "GETWDOVRA")
#pragma map(__link_a, "\174\174A00199")
#pragma map(__pathconf_a, "\174\174A00200")
#pragma map(__read_a, "READOVRA")
#pragma map(__readlink_a, "\174\174A00202")
#pragma map(__rmdir_a, "\174\174A00203")
#pragma map(__symlink_a, "\174\174A00205")
#pragma map(__truncate_a, "\174\174A00206")
#pragma map(__truncate_o, "TRUNCOVRA")
#pragma map(__ttyname_a, "\174\174A00294")
#pragma map(__ttyname_r_a, "\174\174A00034")
#pragma map(__unlink_a, "\174\174A00207")
#pragma map(__write_a, "WRITOVRA")

/*%PAGE																*/
/**
 * ASCII front-end routines for UNISTD functions
 */
 
/**
 * @brief Access a file
 */
int 
__access_a(const char *path, int how)
{
	return access((const char *) __getEstring1_a(path), how);
}
 
/**
 * @brief Change directory
 */
int 
__chdir_a(const char *path)
{
	return chdir((const char *) __getEstring1_a(path));
}
 
/**
 * @brief Change owner
 */
int 
__chown_a(const char *path, uid_t owner, gid_t group)
{
	return chown((const char *) __getEstring1_a(path), owner, group);
}

/**
 * @brief Get Configuration Variables
 */
size_t
__confstr_a(int name, char *buf, size_t len)
{
    size_t res = confstr(name, buf, len);
    if (res > 0)
        __toasciilen_a(buf, buf, res);
    return(res);
}

/**
 * @brief Generate Path Name for Controlling Terminal
 */
char *
__ctermid_a(char *s)
{
    char *tmp_out;
    tmp_out = ctermid(__getEstring1_a(s));
    return(__getAstring1_a(tmp_out));
}

/**
 * @brief Execute a process - arguments no environment
 */
int 
__execv_a(const char *path, char * const *argv)
{
    char * const *newargv = (char * const *)mkNew((const char **)argv);
    int rc;

	rc = execv((const char *) __getEstring1_a(path), newargv);

    /*
     * We only return if the exec fails so free the argv memory
     */
    freeNew((const char **)newargv);

    return rc;
}

/**
 * @brief Execute a process - arguments and environment
 */
int 
__execve_a(const char *path, char * const *argv, char * const *envp)
{
    char * const *newargv = (char * const *)mkNew((const char **)argv);
    char * const *newenvp = (char * const *)mkNew((const char **)envp);
    int rc;

	rc = execve((const char *) __getEstring1_a(path), newargv, newenvp);

    /*
     * We only return if the exec fails so free the argv & envp memory
     */
    freeNew((const char **)newargv);
    freeNew((const char **)newenvp);

    return rc;
}

/**
 * @brief Execute a process using search path - arguments no environment
 */
int 
__execvp_a(const char *file, char * const *argv)
{
    char * const *newargv = (char * const *)mkNew((const char **)argv);
    int rc;

	rc = execvp((const char *) __getEstring1_a(file), newargv);

    /*
     * We only return if the exec fails so free the argv memory
     */
    freeNew((const char **)newargv);

    return rc;
}
 
/**
 * @brief Execute a process given a list of arguments
 */
int
__execl_a(const char *pn, const char *args, ...)
{
    va_list va;

    /*
     * --- Step 1: Count the arguments ---
     */
    int count = 1; // Start at 1 for the mandatory 'arg'
    va_start(va, args);
    const char *current_arg = va_arg(va, const char *);
    while (current_arg != NULL) {
        count++;
        current_arg = va_arg(va, const char *);
    }
    va_end(va);

    /*
     * --- Step 2: Allocate the array ---
     * We allocate (count + 1) to leave room for a trailing NULL element
     */
    char **argv_array = malloc((count + 1) * sizeof(char *));
    if (argv_array == NULL) {
        perror("Allocation failed");
        return -1;
    }

    /*
     * --- Step 3: Populate the array using strdup ---
     * Restart va_list traversal from the beginning
     */
    va_start(va, args);
    
    // Handle the first mandatory argument (arg0)
    argv_array[0] = (char *) strdup(args);
    __toebcdic_a(argv_array[0], argv_array[0]);
    
    // Handle the remaining variadic arguments
    for (int i = 1; i < count; i++) {
        current_arg = va_arg(va, const char *);
        argv_array[i] = strdup(current_arg);
        __toebcdic_a(argv_array[i], argv_array[i]);
    }
    
    // Explicitly NULL-terminate the array
    argv_array[count] = NULL;
    
    va_end(va);

    execl(__getEstring1_a(pn), (const char *)argv_array);

    /*
     * If we reach here then execl failed so free the array
     */
    for (int i = 0; i < count; i++)
        free(argv_array[i]);
    free(argv_array);

    return -1;
}

/**
 * @brief Get current working directory
 */
char *
__getcwd_a(char *buffer, size_t size)
{
	if (getcwd(buffer, size) != 0) {
		__toascii_a(buffer, buffer);
		return(buffer);
	} else
		return(NULL);
}
 
/**
 * @brief Get host name
 */
int 
__gethostname_a(char *name, size_t namelen)
{
	int g_hostint;
	if ((g_hostint=gethostname(name,namelen)) != -1) {
		__toascii_a(name,name);
	}
	return g_hostint;
}
 
/**
 * @brief Get user login name
 */
char *
__getlogin_a(void)
{
	char *user;
	user = getlogin();
	if ((user)!= NULL) {
		__toascii_a(user,user);
		return(user);
	} else
		return(NULL);
}

/**
 * @brief Read a character string without echo
 */
char *
__getpass_a(const char *prompt)
{ 
    extern char *getpass(const char *);
	char *p;

	p = getpass((const char *) __getEstring1_a(prompt));
	if ((p) != NULL) {
		__toascii_a(p,p);
		return(p);
    } else
		return(NULL);
}

/**
 * @brief Get working directory
 */
char *
__getwd_a(char *path_name)
{
	char *p;

	p = getcwd(path_name, 1024);
	__toascii_a(p,p);
	return(path_name);
}
 
/**
 * @brief Get working directory - override
 */
char *
__getwd_o(char *path_name)
{
	return(__getwd_a(path_name));
}
 
/**
 * @brief Create a link to a file
 */
int 
__link_a(const char *oldfile, const char *newname)
{
	return link((const char *) __getEstring1_a(oldfile),
				(const char *) __getEstring2_a(newname));
}
 
/**
 * @brief Determine Configurable Path Name Variables
 */
long 
__pathconf_a(const char *pathname, int varcode)
{
	return pathconf((const char *) __getEstring1_a(pathname), varcode);
}
 
/**
 * @brief Read the value of a symbolic link
 */
int
__readlink_a(const char *path, char *buf, size_t bufsiz)
{
    int rc = readlink((const char *) __getEstring1_a(path), buf, bufsiz);
    if ((rc > 0) && (bufsiz > 0))
        __toasciilen_a(buf, buf, rc);
    return(rc);
}

/**
 * @brief Remove a directory
 */
int 
__rmdir_a(const char *path)
{
	return rmdir((const char *) __getEstring1_a(path));
}
 
/**
 * @brief Create a symbolic link
 */
int
__symlink_a(const char *pathname, const char *slink)
{
    return symlink((const char *) __getEstring1_a(pathname), 
                   (const char *) __getEstring2_a(slink));
}

/**
 * @brief Get the Name of a Terminal
 */
char *
__ttyname_a(int fd)
{
    char *p = ttyname(fd);

    if (p != NULL) 
        return  __getAstring1_a(p);
    return p;
}

/**
 * @brief Get the Name of a Terminal - reentrant
 */
int
__ttyname_r_a(int fd, char *res, size_t len)
{
    char *p = ttyname(fd);
    size_t l;

    if (p != NULL) {
        l = (strlen(p) < len ? strlen(p) : len - 1);
        __toasciilen_a(res, p, l);
        res[l] = 0;
        return 0;
    }
	return 1;
}

/**
 * @brief Truncate a file
 */
int
__truncate_a(char *path, off_t length)
{
    int fd;
    struct stat st;
    char *ePath = __getEstring1_a(path);
    int rc;

    if ((rc = lstat(ePath, &st)) == 0) {
        if ((fd = open(ePath, O_WRONLY|O_APPEND)) >= 0) {
            rc = ftruncate(fd, length);
            close(fd);
        } else
            rc = fd;
    }

	return rc;
}

/**
 * @brief Truncate a file - override version
 */
int
__truncate_o(char *path, off_t length)
{
    return(__truncate_a(path, length));
}
 
/**
 * @brief Remove a directory entry
 */
int 
__unlink_a(const char *path)
{
	return unlink((const char *) __getEstring1_a(path));
}

/**
 * @brief Override write() API 
 *
 * Convert to EBDCIC if output is directed to terminal/printer
 *
 */
ssize_t 
__write_a(int fd, const void *buf, size_t len)
{
    if (!__isAsciiFD(fd)) {
        char *out = __alloca(len);
        __toebcdiclen_a(out, buf, len);
        return write(fd, out, len);
    } else 
        return write(fd, buf, len);
}

/**
 * @brief Override read() API 
 *
 * Convert to ASCII if input is from terminal
 *
 */
ssize_t 
__read_a(int fd, void *buf, size_t len)
{
    ssize_t res;

    res = read(fd, buf, len);

    if (res > 0) {
        if (!__isAsciiFD(fd)) {
            __toasciilen_a(buf, buf, res);
        } 
    } 
    return res;
}

/**
 * @brief Override close() API 
 *
 * Remove fd from the fdxl_t linked list
 *
 */
int
__close_a(int fd)
{
    int res;

    res = close(fd);
    __deleteFD(fd);
    return res;
}
