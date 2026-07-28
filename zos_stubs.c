/**
 * @file zos_stubs.c
 * @brief Contains dummy entry points for z/OS which already has its own ASCII 
 * layer
 * 
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#pragma export(select_ovr)
#pragma export(__fgetc_ovr)
#pragma export(__getc_ovr)
#pragma export(__getchar_ovr)
#pragma export(__ungetc_ovr)
#pragma export(__close_ovr)
#pragma export(__getwd_ovr)
#pragma export(__read_ovr)
#pragma export(__truncate_ovr)
#pragma export(__write_ovr)
#pragma export(nanosleep)
#pragma export(__isVM)

#pragma map(select_ovr, "SLCTOVRA")
#pragma map(__fgetc_ovr, "FGETOVRA")
#pragma map(__getc_ovr, "GETCOVRA")
#pragma map(__getchar_ovr, "GTCHOVRA")
#pragma map(__ungetc_ovr, "UGETCOVRA")
#pragma map(__close_ovr, "CLOSOVRA")
#pragma map(__getwd_ovr, "GETWDOVRA")
#pragma map(__read_ovr, "READOVRA")
#pragma map(__truncate_ovr, "TRUNCOVRA")
#pragma map(__write_ovr, "WRITOVRA")

int 
__fgetc_ovr(FILE *stream)
{
    return fgetc(stream);
}

int 
__getc_ovr(FILE *stream)
{
    return getc(stream);
}

int 
__getchar_ovr()
{
    return getchar();
}
int 
__ungetc_ovr(int c, FILE *stream)
{
    return ungetc(c, stream);
}

int
__close_ovr(int fd)
{
    return close(fd);
}

char *
__getwd_ovr(char *buf)
{
    return getcwd(buf, 1024);

}

ssize_t 
__read_ovr(int fildes, void *buf, size_t nbyte)
{
    return read(fildes, buf, nbyte);
}

int 
__truncate_ovr(const char *path, off_t length)
{
    return truncate(path, length);
}

ssize_t 
__write_ovr(int fildes, const void *buf, size_t nbyte)
{
    return write(fildes, buf, nbyte);
}

int
select_ovr(int nfds, fd_set *readlist, fd_set *writelist, fd_set *exceptlist, struct timeval *timeout)
{
    return select(nfds, readlist, writelist, exceptlist, timeout);
}

/**
 * @brief Implement nanosleep via select
 */
int
nanosleep(const struct timespec *req, struct timespec *rem)
{
    struct timeval end, now, timeout;
    int64_t endUsecs, remUsecs;
    int32_t usec, sec;
    int rc;

    endUsecs = req->tv_nsec * 1000 + req->tv_sec * 1000000;
    gettimeofday(&end, NULL);
    endUsecs += end.tv_sec * 1000000 + end.tv_usec;
    timeout.tv_sec = endUsecs / 1000000;
    timeout.tv_usec = endUsecs % 1000000;
    rc = select(0, NULL, NULL, NULL, &timeout);
    while ((rc == -1) && (errno = EINTR)) {
        gettimeofday(&now, NULL);
        remUsecs = endUsecs - (now.tv_sec * 1000000 + now.tv_usec);
        timeout.tv_sec = remUsecs / 1000000;
        timeout.tv_usec = remUsecs % 1000000;
        rc = select(0, NULL, NULL, NULL, &timeout);
    }

    return rc;
}

int
__isVM()
{
    return 0;
}
