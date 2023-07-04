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
#pragma export(__gai_strerror_a)
#pragma export(__getaddrinfo_a)
#pragma export(__gethostbyaddr_a)
#pragma export(__gethostbyname_a)
#pragma export(__getnameinfo_a)
#pragma export(__getprotobyname_a)
#pragma export(__getservbyname_a)
#pragma export(__getservbyport_a)
#endif

#pragma map(__gai_strerror_a, "\174\174A00208")
#pragma map(__getaddrinfo_a, "\174\174A00082")
#pragma map(__gethostbyaddr_a, "\174\174A00257")
#pragma map(__gethostbyname_a, "\174\174A00258")
#pragma map(__getnameinfo_a, "\174\174A00087")
#pragma map(__getprotobyname_a, "\174\174A00318")
#pragma map(__getservbyname_a, "\174\174A00321")
#pragma map(__getservbyport_a, "\174\174A00322")

void convertHostentToAscii(struct hostent *);
void convertServentToAscii(struct servent *);

/*%PAGE																*/
/**
 * @brief Translate address info error codes into strings
 */
const char *
__gai_strerror_a(int errcode)
{
    char *res = (char *) gai_strerror(errcode);
    if (res != NULL)
        __toascii_a(res, res);
    return res;
}

/**
 * @brief Get address information
 */
int
__getaddrinfo_a(const char *hostname, const char *servname, 
                const struct addrinfo *hints, struct addrinfo **res)
{
    struct addrinfo *eHints = NULL;
    int rc;

    if (hints != NULL) {
        struct addrinfo *h,
                        *n = __alloca(sizeof (struct addrinfo));
        eHints = n;
        for (h = (struct addrinfo *) hints; h != NULL; h = h->ai_next) {
            *n = *h;
            if (h->ai_flags & AI_CANONNAME == AI_CANONNAME)
                __toascii_a(n->ai_canonname, n->ai_canonname);
            if (n->ai_next != NULL) {
                n->ai_next = __alloca(sizeof (struct addrinfo));
                n = n->ai_next;
            }
        }
    }

    rc = getaddrinfo(__getEstring1_a(hostname), __getEstring2_a(servname),
                     eHints, res);
    if (rc == 0) {
        struct addrinfo *h;
        for (h = *res; h != NULL; h = h->ai_next) 
            if (h->ai_flags & AI_CANONNAME == AI_CANONNAME)
                __toascii_a(h->ai_canonname, h->ai_canonname);
    }
    return rc;
}

/**
 * @brief Get host by address
 */
struct hostent *
__gethostbyaddr_a(char *address, int address_len, int domain)
{
	struct hostent*	my_hostent;
	my_hostent = gethostbyaddr(address, address_len, domain);
	if (my_hostent)
		convertHostentToAscii(my_hostent);
	return my_hostent;
}

/**
 * @brief Get host by name
 */
struct hostent *
__gethostbyname_a(char *name)
{
	struct hostent *my_hostent;

	my_hostent = gethostbyname(__getEstring1_a((const char *)name));
	if (my_hostent)
		convertHostentToAscii(my_hostent);
	return my_hostent;
}

/**
 * @brief Get server by name
 */
struct servent *
__getservbyname_a(char *name, char *proto)
{
	struct servent *my_servent;

	my_servent = getservbyname(__getEstring1_a((const char *)name),
		    				   __getEstring2_a((const char *)proto));
	if (my_servent)
		convertServentToAscii(my_servent);
	return my_servent;
}

/**
 * @brief Get server by port
 */
struct servent *
__getservbyport_a(int port, char *proto)
{
	struct servent *my_servent;

	my_servent = getservbyport(port, __getEstring2_a((const char *)proto));
	if (my_servent)
		convertServentToAscii(my_servent);
	return my_servent;
}

/**
 * @brief Get proto by name
 */
struct protoent *
__getprotobyname_a(char *name)
{
	struct protoent *my_protoent;
    int alias;

	my_protoent = getprotobyname(__getEstring1_a((const char *)name));
	if (my_protoent) {
		__toascii_a(my_protoent->p_name, my_protoent->p_name);
        for (alias = 0; my_protoent->p_aliases[alias] != NULL; alias++)
            __toascii_a(my_protoent->p_aliases[alias], my_protoent->p_aliases[alias]);
    }
	return my_protoent;
}

/**
 * @brief Get name info
 */
int
__getnameinfo_a(const struct sockaddr *addr, socklen_t addrlen,
                char *host, socklen_t hostlen,
                char *serv, socklen_t servlen, int flags)
{
    int rc;

    rc = getnameinfo(addr, addrlen, host, hostlen, serv, servlen, flags);
    if (rc == 0) {
        if (hostlen > 0)
            __toasciilen_a(host, host, hostlen);
        if (servlen > 0)
            __toasciilen_a(host, host, hostlen);
    }
    return rc;
}

/*%PAGE																*/
/**
 * Internal routines
 */

/**
 * @brief Convert string fields in hostent to ascii
 */
void 
convertHostentToAscii(struct hostent * phostent)
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
convertServentToAscii(struct servent * pservent)
{
	int	i;

	__toascii_a(pservent->s_name,pservent->s_name);
	if (pservent->s_aliases)
		for (i=0;pservent->s_aliases[i];i++)
			__toascii_a(pservent->s_aliases[i],pservent->s_aliases[i]);
	__toascii_a(pservent->s_proto,pservent->s_proto);
}
