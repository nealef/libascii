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
#pragma map(select_ovr, "SLCTOVRA")
#pragma map(__clock_ovr, "CLCKOVRA")
#pragma map(__uname_ovr, "UNAMEOVR")
#pragma map(__uname_ovr, "UNAMEOVR")
#pragma map(__wcsstr_a    "WCSSTROV")
#define select(n, r, w, x, t) select_ovr(n, r, w, x, t)
#define clock() __clock_ovr()
#ifdef __UNAME_OVERRIDE
# define uname(a) __uname_ovr(a)
#endif
#define wcsstr(a, b) __wcsstr_a(a, b)

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

#undef toupper
#undef tolower
#undef towupper
#undef towlower

#undef MB_CUR_MAX
#define MB_CUR_MAX __mb_cur_max_a()
int __mb_cur_max_a(void);
#pragma map(__mb_cur_max_a, "MBCURMXA");

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
