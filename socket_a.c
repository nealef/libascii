/**
 * @file socket_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the socket functions for OE Sockets format calls
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include "global_a.h"
 
struct sockaddr_iucv {                                              
    unsigned char   siucv_len;       /* Length of sockaddr struct */
    unsigned char   siucv_family;    /* addressing family */        
    unsigned short  siucv_port;      /* port number */              
    unsigned long   siucv_addr;      /* address */                  
    unsigned char   siucv_nodeid[8]; /* nodeid to connect to */     
    unsigned char   siucv_userid[8]; /* userid to connect to */     
    unsigned char   siucv_name[8];   /* iucvname for connect */     
};                                                                  

#pragma export(__getsockname_a)
#pragma export(__connect_a)
#pragma export(__bind_a)
#pragma export(__accept_a)
#pragma export(__recvfrom_a)
#pragma export(__sendto_a)

#pragma map(__getsockname_a,  "\174\174A00409")
#pragma map(__connect_a,      "\174\174A00407")
#pragma map(__bind_a,         "\174\174A00406")
#pragma map(__accept_a,       "\174\174A00404")
#pragma map(__recvfrom_a,     "\174\174A00410")
#pragma map(__sendto_a,       "\174\174A00411")

/*%PAGE                                                             */
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

/**
 * @brief Get socket name
 */
int 
__getsockname_a(int sd, struct sockaddr *address, size_t *address_len)
{
    int rc;
    struct sockaddr_in *in;
    struct sockaddr_in6 *i6;
    struct sockaddr_un *un;
    struct sockaddr_iucv *iu;

    rc = getsockname(sd, address, address_len);
    if (rc == 0) {
        switch(address->sa_family) {
            case AF_INET : 
                in = (struct sockaddr_in *) address;
                break;
            case AF_INET6 :
                i6 = (struct sockaddr_in6 *) address;
                break;
            case AF_UNIX :
                un = (struct sockaddr_un *) address;
                __toascii_a(un->sun_path, un->sun_path);
                break;
            case AF_IUCV :
                iu = (struct sockaddr_iucv *) address;
                __toasciilen_a(iu->siucv_nodeid, iu->siucv_nodeid, sizeof(iu->siucv_nodeid));
                __toasciilen_a(iu->siucv_userid, iu->siucv_userid, sizeof(iu->siucv_userid));
                __toasciilen_a(iu->siucv_name, iu->siucv_name, sizeof(iu->siucv_name));
                break;
        }
    }

    return rc;
}

/**
 * @brief connect socket
 */
int 
__connect_a(int sd, const struct sockaddr *address, size_t address_len)
{
    int rc;
    struct sockaddr_in *in;
    struct sockaddr_in6 *i6;
    struct sockaddr_un *un;
    struct sockaddr_iucv *iu;
    struct sockaddr *sa = (address != NULL ? __alloca(address_len) : NULL);

    if (address != NULL) {
        memcpy(sa, address, address_len);

        switch(address->sa_family) {
            case AF_INET : 
                in = (struct sockaddr_in *) sa;
                break;
            case AF_INET6 :
                i6 = (struct sockaddr_in6 *) sa;
                break;
            case AF_UNIX :
                un = (struct sockaddr_un *) sa;
                __toascii_a(un->sun_path, un->sun_path);
                break;
            case AF_IUCV :
                iu = (struct sockaddr_iucv *) sa;
                __toasciilen_a(iu->siucv_nodeid, iu->siucv_nodeid, sizeof(iu->siucv_nodeid));
                __toasciilen_a(iu->siucv_userid, iu->siucv_userid, sizeof(iu->siucv_userid));
                __toasciilen_a(iu->siucv_name, iu->siucv_name, sizeof(iu->siucv_name));
                break;
        }
    } 

    return connect(sd, sa, address_len);
}

/**
 * @brief bind to socket
 */
int 
__bind_a(int sd, const struct sockaddr *address, size_t address_len)
{
    int rc;
    struct sockaddr_in *in;
    struct sockaddr_in6 *i6;
    struct sockaddr_un *un;
    struct sockaddr_iucv *iu;
    struct sockaddr *sa = (address != NULL ? __alloca(address_len) : NULL);

    if (address != NULL) {
        memcpy(sa, address, address_len);

        switch(address->sa_family) {
            case AF_INET : 
                in = (struct sockaddr_in *) sa;
                break;
            case AF_INET6 :
                i6 = (struct sockaddr_in6 *) sa;
                break;
            case AF_UNIX :
                un = (struct sockaddr_un *) sa;
                __toascii_a(un->sun_path, un->sun_path);
                break;
            case AF_IUCV :
                iu = (struct sockaddr_iucv *) sa;
                __toasciilen_a(iu->siucv_nodeid, iu->siucv_nodeid, sizeof(iu->siucv_nodeid));
                __toasciilen_a(iu->siucv_userid, iu->siucv_userid, sizeof(iu->siucv_userid));
                __toasciilen_a(iu->siucv_name, iu->siucv_name, sizeof(iu->siucv_name));
                break;
        }
    }

    return bind(sd, sa, address_len);
}

/**
 * @brief Accept socket connection
 */
int 
__accept_a(int sd, struct sockaddr *address, size_t *address_len)
{
    int rc;
    struct sockaddr_in *in;
    struct sockaddr_in6 *i6;
    struct sockaddr_un *un;
    struct sockaddr_iucv *iu;

    rc = accept(sd, address, address_len);
    if ((rc == 0) && (address != NULL)) {
        switch(address->sa_family) {
            case AF_INET : 
                in = (struct sockaddr_in *) address;
                break;
            case AF_INET6 :
                i6 = (struct sockaddr_in6 *) address;
                break;
            case AF_UNIX :
                un = (struct sockaddr_un *) address;
                __toascii_a(un->sun_path, un->sun_path);
                break;
            case AF_IUCV :
                iu = (struct sockaddr_iucv *) address;
                __toasciilen_a(iu->siucv_nodeid, iu->siucv_nodeid, sizeof(iu->siucv_nodeid));
                __toasciilen_a(iu->siucv_userid, iu->siucv_userid, sizeof(iu->siucv_userid));
                __toasciilen_a(iu->siucv_name, iu->siucv_name, sizeof(iu->siucv_name));
                break;
        }
    }

    return rc;
}

/**
 * @brief Receive from a socket
 */
int 
__recvfrom_a(int sd, void *buffer, size_t length, int flags, 
             struct sockaddr *address, size_t *address_len)
{
    int rc;
    struct sockaddr_in *in;
    struct sockaddr_in6 *i6;
    struct sockaddr_un *un;
    struct sockaddr_iucv *iu;

    rc = recvfrom(sd, buffer, length, flags, address, address_len);
    if ((rc == 0) && (address != NULL)) {
        switch(address->sa_family) {
            case AF_INET : 
                in = (struct sockaddr_in *) address;
                break;
            case AF_INET6 :
                i6 = (struct sockaddr_in6 *) address;
                break;
            case AF_UNIX :
                un = (struct sockaddr_un *) address;
                __toascii_a(un->sun_path, un->sun_path);
                break;
            case AF_IUCV :
                iu = (struct sockaddr_iucv *) address;
                __toasciilen_a(iu->siucv_nodeid, iu->siucv_nodeid, sizeof(iu->siucv_nodeid));
                __toasciilen_a(iu->siucv_userid, iu->siucv_userid, sizeof(iu->siucv_userid));
                __toasciilen_a(iu->siucv_name, iu->siucv_name, sizeof(iu->siucv_name));
                break;
        }
    }

    return rc;
}

/**
 * @brief Send to socket
 */
int 
__sendto_a(int sd, const void *buffer, size_t length, int flags,
           const struct sockaddr *address, size_t address_len)
{
    int rc;
    struct sockaddr_in *in;
    struct sockaddr_in6 *i6;
    struct sockaddr_un *un;
    struct sockaddr_iucv *iu;
    struct sockaddr *sa = (address != NULL ? __alloca(address_len) : NULL);

    if (address != NULL) {
        memcpy(sa, address, address_len);

        switch(address->sa_family) {
            case AF_INET : 
                in = (struct sockaddr_in *) sa;
                break;
            case AF_INET6 :
                i6 = (struct sockaddr_in6 *) sa;
                break;
            case AF_UNIX :
                un = (struct sockaddr_un *) sa;
                __toascii_a(un->sun_path, un->sun_path);
                break;
            case AF_IUCV :
                iu = (struct sockaddr_iucv *) sa;
                __toasciilen_a(iu->siucv_nodeid, iu->siucv_nodeid, sizeof(iu->siucv_nodeid));
                __toasciilen_a(iu->siucv_userid, iu->siucv_userid, sizeof(iu->siucv_userid));
                __toasciilen_a(iu->siucv_name, iu->siucv_name, sizeof(iu->siucv_name));
                break;
        }
    }

    return sendto(sd, buffer, length, flags, sa, address_len);
}
