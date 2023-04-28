/**
 * @file netdb2_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the netdb functions for OE Sockets format calls
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

#define _OE_SOCKETS
#include <netdb.h>
#include "global_a.h"
 
#ifdef GEN_PRAGMA_EXPORT
#pragma export(__gethostbyaddr_a)
#pragma export(__gethostbyname_a)
#pragma export(__getservbyname_a)
#endif

void Convert_hostent_to_ascii(struct hostent *);
void Convert_servent_to_ascii(struct servent *);

/*%PAGE																*/
/**
 * @brief Get host by address
 */
struct hostent *
__gethostbyaddr_a(char * address, int address_len, int domain)
{
	struct hostent*	my_hostent;
	my_hostent = gethostbyaddr(address, address_len, domain);
	if (my_hostent)
		Convert_hostent_to_ascii(my_hostent);
	return my_hostent;
}

/**
 * @brief Get host by name
 */
struct hostent *
__gethostbyname_a(char *name)
{
	struct hostent*	my_hostent;
	my_hostent = gethostbyname(__getEstring1_a((const char *)name));
	if (my_hostent)
		Convert_hostent_to_ascii(my_hostent);
	return my_hostent;
}

/**
 * @brief Get server by name
 */
struct servent *
__getservbyname_a(char * name, char * proto)
{
	struct servent*	my_servent;
	my_servent = getservbyname(__getEstring1_a((const char *)name),
		    				   __getEstring2_a((const char *)proto));
	if (my_servent)
		Convert_servent_to_ascii(my_servent);
	return my_servent;
}

/*%PAGE																*/
/**
 * Internal routines
 */

/**
 * @brief Convert string fields in hostent to ascii
 */
void 
Convert_hostent_to_ascii(struct hostent * phostent)
{
	int	i;
	__toascii_a(phostent->h_name,phostent->h_name);
	if (phostent->h_aliases)
		for (i=0;phostent->h_aliases[i];i++)
			__toascii_a(phostent->h_aliases[i],phostent->h_aliases[i]);
}

/**
 * @brief Convert string fields in servent to ascii
 */
void 
Convert_servent_to_ascii(struct servent * pservent)
{
	int	i;
	__toascii_a(pservent->s_name,pservent->s_name);
	if (pservent->s_aliases)
		for (i=0;pservent->s_aliases[i];i++)
			__toascii_a(pservent->s_aliases[i],pservent->s_aliases[i]);
	__toascii_a(pservent->s_proto,pservent->s_proto);
}
