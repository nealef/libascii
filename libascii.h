#ifndef __LIBASCII_H
#define __LIBASCII_H

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>

#pragma map(close, "CLOSOVRA")
#pragma map(read, "READOVRA")
#pragma map(write, "WRITOVRA")
#pragma map(fgetc, "FGETOVRA")
#pragma map(getc, "GETCOVRA")
#pragma map(getchar, "GTCHOVRA")
#pragma map(ungetc, "UGETCOVRA")
// #pragma map(getwd, "GETWDOVRA")
// #pragma map(truncate, "TRUNCOVRA")
#pragma map(select_ovr, "SLCTOVRA")
#pragma map(__clock_ovr, "CLCKOVRA")
#pragma map(__uname_ovr, "UNAMEOVR")
#define select(n, r, w, x, t) select_ovr(n, r, w, x, t)
#define clock() __clock_ovr()
#ifdef __UNAME_OVERRIDE
# define uname(a) __uname_ovr(a)
#endif

void __initASCIIlib_a(void);
int __isVM(void);

#ifdef getchar
# undef getchar
# undef getc
#endif

#undef isalnum
#undef isalpha
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit

/*
 * Debug MACROs
 */
#ifdef __LIBASCII_DEBUG
void __dump__data(char *, void *, size_t);
void __debug_print(const char *, int, char *, ...);
# define DUMP_DATA(t, d, l) __dump_data(t, d, l)

# define DEBUG_PRINT(fmt, ...) __debug_print(__func__, __LINE__, fmt, __VA_ARGS__)

#else
# define DUMP_DATA(t, d, l)
# define DEBUG_PRINT(fmt, ...) 
#endif

#endif
