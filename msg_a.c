/**
 * @file msg_a.c
 * @brief Contains ASCII-to-EBCDIC front end to the msg functions.
 * 
 * Notes	:	All the procedures are name "__xxxxxxxx_a" where
 *				xxxxxxxx is the name of the standard C run-time
 *				function name. Unless otherwise noted, all functions
 * 				take the same argument,produce the same output and
 *				return the same values as the standard functions.
 */

#pragma langlvl( extended )
 
#include <sys/msg.h>
#include "global_a.h"

#pragma export(__msgrcv_a)
#pragma export(__msgsnd_a)

#pragma map(__msgrcv_a,   "\174\174A00280")
#pragma map(__msgsnd_a,   "\174\174A00281")

typedef struct {
    long msgtype;
    char msgtext[0];
} message_t;

/**
 * @brief Receive a message
 */
ssize_t
__msgrcv_a(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{
    ssize_t msgLen;
    message_t *msg = msgp;

    msgLen = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    if (msgLen > 0)
        __toasciilen_a(msg->msgtext, msg->msgtext, msgLen);

    return msgLen;
}

/**
 * @brief Send a message
 */
ssize_t
__msgsnd_a(int msqid, void *msgp, size_t msgsz, int msgflg)
{
    int rc;
    message_t *msg = msgp,
              *snd;

    snd = malloc(msgsz + sizeof(long));
    memcpy(snd, msg, msgsz + sizeof(long));
    __toebcdiclen_a(snd->msgtext, snd->msgtext, msgsz);
    rc = msgsnd(msqid, snd, msgsz, msgflg);
    free(snd);

    return rc;
}
