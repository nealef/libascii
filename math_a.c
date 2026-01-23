/**
 * @file math_a.c
 * @brief Contains BFP/DFP routines for math.h
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "global_a.h"

/**
 * Data class types
 */
#define FPCLASSIFY  1
#define ISFINITE    2
#define ISINF       3
#define ISNORMAL    4
#define SIGNBIT     5
#define ISNAN       6
           
#pragma export(__class_a)
#pragma export(__round_a)
#pragma export(__roundf_a)
#pragma export(__roundl_a)
#if 0
#pragma export(__lround_a)
#pragma export(__lroundf_a)
#pragma export(__lroundl_a)
#pragma export(__llround_a)
#pragma export(__llroundf_a)
#pragma export(__llroundl_a)
#endif

#pragma map(__class_a, "\174\174CLASSB")
#pragma map(__round_a, "\174\174ROUN\174B")
#pragma map(__roundf_a, "\174\174ROUNFB")   
#pragma map(__roundl_a, "\174\174ROUNLB")   

/*%PAGE																*/
/**
 * @brief Return class of data
 */
int
__class_a(const long double h, int size, int test)
{
    uint32_t dataClass = 0;
    static uint32_t tstFinite = 0xfc0,
             tstInfinite = 0x030, 
             tstNormal = 0x300,
             tstSubNormal = 0x0c0,
             tstNan = 0x00f,
             tstZero = 0xc00,
             tstSign = 0x555;

    __asm__ ("TSTFIN   EQU    X'FC0'\n"     /* Same as the tst... above but for assembler */
             "TSTINF   EQU    X'030'\n"
             "TSTNRM   EQU    X'300'\n"
             "TSTSUB   EQU    X'0C0'\n"
             "TSTNAN   EQU    X'00F'\n"
             "TSTZER   EQU    X'C00'\n"
             "TSTSGN   EQU    X'555'\n");

    __asm__ ("RESINF   EQU    7\n"          /* Assembler version of FP_INFINITE */
             "RESNAN   EQU    10\n"         /* ... FP_NAN */
             "RESNRM   EQU    1\n"          /* ... FP_NORMAL */
             "RESSUB   EQU    5\n"          /* ... FP_SUBNORMAL */
             "RESZRO   EQU    3\n");        /* ... FP_ZERO */

    int res = 0;

    switch(size) {
    case 4 : 
        switch(test) {
        case FPCLASSIFY :
            __asm__ ("         LXEBR 0,0\n"
                     "         LHI   9,TSTINF\n"
                     "         TCEB  0,0(,9)\n"
                     "         JZ    NOTINF\n"
                     "         LHI   %0,RESINF\n"
                     "         J     FPCLASSF\n"
                     "NOTINF   LHI   9,TSTNAN\n"
                     "         TCEB  0,0(,9)\n"
                     "         JZ    NOTNANF\n"
                     "         LHI   %0,RESNAN\n"
                     "         J     FPCLASSF\n"
                     "NOTNANF  LHI   9,TSTNRM\n"
                     "         TCEB  0,0(,9)\n"
                     "         JZ    NOTNRMF\n"
                     "         LHI   %0,RESNRM\n"
                     "         J     FPCLASSF\n"
                     "NOTNRMF  LHI   9,TSTSUB\n"
                     "         TCEB  0,0(,9)\n"
                     "         JZ    NOTSUBF\n"
                     "         LHI   %0,RESSUB\n"
                     "         J     FPCLASSF\n"
                     "NOTSUBF  LHI   %0,RESZRO\n"
                     "FPCLASSF EQU   *\n"
                     : "=r" (res) : : "cc", "9");
            break;
        case ISFINITE :
            dataClass = tstFinite;
            break;
        case ISINF :
            dataClass = tstInfinite;
            break;
        case ISNORMAL :
            dataClass = tstNormal;
            break;
        case SIGNBIT :
            dataClass = tstSign;
            break;
        case ISNAN :
            dataClass = tstNan;
            break;
        }
        if (dataClass != 0) {
            __asm__ ("  LHI   %0,0\n"
                     "  LEXBR 0,0\n"
                     "  L     9,%1\n"
                     "  TCEB  0,0(,9)\n"
                     "  JZ    CLASSAF\n"
                     "  LHI   %0,1\n"
                     "CLASSAF EQU *\n"
                     : "=r" (res) : "m" (dataClass) : "cc", "9");
        }
        break;
    case 8 : 
        switch(test) {
        case FPCLASSIFY :
            __asm__ ("         LDXBR 0,0\n"
                     "         LHI   9,TSTINF\n"
                     "         TCDB  0,0(,9)\n"
                     "         JZ    NOTIND\n"
                     "         LHI   %0,RESINF\n"
                     "         J     FPCLASSD\n"
                     "NOTIND   LHI   9,TSTNAN\n"
                     "         TCDB  0,0(,9)\n"
                     "         JZ    NOTNAND\n"
                     "         LHI   %0,RESNAN\n"
                     "         J     FPCLASSD\n"
                     "NOTNAND  LHI   9,TSTNRM\n"
                     "         TCDB  0,0(,9)\n"
                     "         JZ    NOTNRMD\n"
                     "         LHI   %0,RESNRM\n"
                     "         J     FPCLASSD\n"
                     "NOTNRMD  LHI   9,TSTSUB\n"
                     "         TCDB  0,0(,9)\n"
                     "         JZ    NOTSUBD\n"
                     "         LHI   %0,RESSUB\n"
                     "         J     FPCLASSD\n"
                     "NOTSUBD  LHI   %0,RESZRO\n"
                     "FPCLASSD EQU   *\n"
                     : "=r" (res) : : "cc", "9");
            break;
        case ISFINITE :
            dataClass = tstFinite;
            break;
        case ISINF :
            dataClass = tstInfinite;
            break;
        case ISNORMAL :
            dataClass = tstNormal;
            break;
        case SIGNBIT :
            dataClass = tstSign;
            break;
        case ISNAN :
            dataClass = tstNan;
            break;
        }
        if (dataClass != 0) {
            __asm__ ("  LHI   %0,0\n"
                     "  LDXBR 0,0\n"
                     "  L     9,%1\n"   
                     "  TCDB  0,0(,9)\n"
                     "  JZ    CLASSAD\n"
                     "  LHI   %0,1\n"
                     "CLASSAD EQU *\n"
                     : "=r" (res) : "m" (dataClass) : "cc", "9");
        }
        break;
    case 16 : 
        switch(test) {
        case FPCLASSIFY :
            __asm__ ("         LHI   9,TSTINF\n"
                     "         TCXB  0,0(,9)\n"
                     "         JZ    NOTINX\n"
                     "         LHI   %0,RESINF\n"
                     "         J     FPCLASSX\n"
                     "NOTINX   LHI   9,TSTNAN\n"
                     "         TCXB  0,0(,9)\n"
                     "         JZ    NOTNANX\n"
                     "         LHI   %0,RESNAN\n"
                     "         J     FPCLASSX\n"
                     "NOTNANX  LHI   9,TSTNRM\n"
                     "         TCXB  0,0(,9)\n"
                     "         JZ    NOTNRMX\n"
                     "         LHI   %0,RESNRM\n"
                     "         J     FPCLASSX\n"
                     "NOTNRMX  LHI   9,TSTSUB\n"
                     "         TCXB  0,0(,9)\n"
                     "         JZ    NOTSUBX\n"
                     "         LHI   %0,RESSUB\n"
                     "         J     FPCLASSX\n"
                     "NOTSUBX  LHI   %0,RESZRO\n"
                     "FPCLASSX EQU   *\n"
                     : "=r" (res) : : "cc", "9");
            break;
        case ISFINITE :
            dataClass = tstFinite;
            break;
        case ISINF :
            dataClass = tstInfinite;
            break;
        case ISNORMAL :
            dataClass = tstNormal;
            break;
        case SIGNBIT :
            dataClass = tstSign;
            break;
        case ISNAN :
            dataClass = tstNan;
            break;
        }
        if (dataClass != 0) {
            __asm__ ("  LHI   %0,0\n"
                     "  L     9,%1\n"
                     "  TCXB  0,0(,9)\n"
                     "  JZ    CLASSAX\n"
                     "  LHI   %0,1\n"
                     "CLASSAX EQU *\n"
                     : "=r" (res) : "m" (dataClass) : "cc", "9");
        }
        break;
    }
    return res;
}

/**
 * @brief Round a double
 */
double
__round_a(double g)
{
    double res;

    asm ("  FIDBR   0,1,0\n"
         "  STDY    0,%0\n"
         : "=m" (res) : : "cc");
    
    return res;
}

/**
 * @brief Round a double
 */
float
__roundf_a(float f)
{
    float res;

    asm ("  FIEBR   0,1,0\n"
         "  STEY    0,%0\n"
         : "=m" (res) : : "cc");
    
    return res;
}

/**
 * @brief Round a long double
 */
long double
__roundl_a(double g)
{
    union {
        long double res;
        double d[2];
    } r;

    asm ("  FIXBR   0,1,0\n"
         "  STDY    0,%0\n"
         "  STDY    2,%1\n"
         : "=m" (r.d[0]), "=m" (r.d[1]) : : "cc");
    
    return r.res;
}
