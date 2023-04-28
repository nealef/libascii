/**
 * @file unistd_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the unistd functions.
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
#include <pwd.h>
#include "global_a.h"
 
#ifdef GEN_PRAGMA_EXPORT
#pragma export(__access_a)
#pragma export(__chdir_a)
#pragma export(__chown_a)
#pragma export(__confstr_a)
#pragma export(__ctermid_a)
#pragma export(__execv_a)
#pragma export(__execve_a)
#pragma export(__execvp_a)
#pragma export(__getcwd_a)
#pragma export(__gethostname_a)
#pragma export(__getlogin_a)
#pragma export(__getpass_a)
#pragma export(__getwd_a)
#pragma export(__link_a)
#pragma export(__pathconf_a)
#pragma export(__readlink_a)
#pragma export(__rmdir_a)
#pragma export(__ttyname_a)
#pragma export(__symlink_a)
#pragma export(__unlink_a)
#endif
 
#pragma map(__access_a, "\174\174A00192")
#pragma map(__chdir_a, "\174\174A00193")
#pragma map(__chown_a, "\174\174A00194")
#pragma map(__confstr_a, "\174\174A00238")
#pragma map(__ctermid_a, "\174\174A00274")
#pragma map(__execv_a, "\174\174A00068")
#pragma map(__execve_a, "\174\174A00084")
#pragma map(__execvp_a, "\174\174A00085")
#pragma map(__getcwd_a, "\174\174A00196")
#pragma map(__gethostname_a, "\174\174A00260")
#pragma map(__getlogin_a, "\174\174A00261")
#pragma map(__getpass_a, "\174\174A00263")
#pragma map(__getwd_a, "\174\174A00197")
#pragma map(__pathconf_a, "\174\174A00200")
#pragma map(__readlink_a, "\174\174A00202")
#pragma map(__rmdir_a, "\174\174A00203")
#pragma map(__symlink_a, "\174\174A00205")
#pragma map(__ttyname_a, "\174\174A00034")
#pragma map(__unlink_a, "\174\174A00207")

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
__execv_a(const char *path, char *const argv[])
{
	return execv((const char *) __getEstring1_a(path), argv);
}

/**
 * @brief Execute a process - arguments and environment
 */
int 
__execve_a(const char *path, char *const argv[], char *const envp[])
{
	return execve((const char *) __getEstring1_a(path), argv, envp);
}

/**
 * @brief Execute a process using search path - arguments no environment
 */
int 
__execvp_a(const char *file, char *const argv[])
{
	return execvp((const char *) __getEstring1_a(file), argv);
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
	return(p);
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
	return  __getAstring1_a(p);
}
 
/**
 * @brief Remove a directory entry
 */
int 
__unlink_a(const char *path)
{
	return unlink((const char *) __getEstring1_a(path));
}
