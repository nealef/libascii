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
    uint64_t dataClass = 0;
    static uint64_t tstFinite = 0xfc0,
             tstInfinite = 0x030, 
             tstNormal = 0x300,
             tstSubNormal = 0x0c0,
             tstNan = 0x00f,
             tstZero = 0xc00,
             tstSign = 0x555;
    static int resInfinite = FP_INFINITE,
               resNaN = FP_NAN,
               resNormal = FP_NORMAL,
               resSubNormal = FP_SUBNORMAL,
               resZero = FP_ZERO;
    int res = 0;

    switch(size) {
    case 4 : 
        switch(test) {
        case FPCLASSIFY :
            __asm__ ("         LXEBR 0,0\n"
                     "         TCEB  0,%1\n"
                     "         JNZ   NOTINF\n"
                     "         L     %0,%2\n"
                     "         J     FPCLASSF\n"
                     "NOTINF   TCEB  0,%3\n"
                     "         JNZ   NOTNANF\n"
                     "         L     %0,%4\n"
                     "         J     FPCLASSF\n"
                     "NOTNANF  TCEB  0,%5\n"
                     "         JNZ   NOTNRMF\n"
                     "         L     %0,%6\n"
                     "         J     FPCLASSF\n"
                     "NOTNRMF  TCEB  0,%7\n"
                     "         JNZ   NOTSUBF\n"
                     "         L     %0,%8\n"
                     "         J     FPCLASSF\n"
                     "NOTSUBF  TCEB  0,%9\n"
                     "         JNZ   FPCLASSF\n"
                     "         L     %0,%10\n"
                     "FPCLASSF EQU   *\n"
                     : "=r" (res)
                     : "m" (tstInfinite), "m" (resInfinite),
                       "m" (tstNan), "m" (resNaN),
                       "m" (tstNormal), "m" (resNormal),
                       "m" (tstSubNormal), "m" (resSubNormal),
                       "m" (tstZero), "m" (resZero)
                     : "cc");
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
            asm ("  LEXBR 0,0\n"
                 "  TCEB  0,%1\n"
                 "  JZ    CLASSAF\n"
                 "  LHI   %0,1\n"
                 "CLASSAF EQU *\n"
                 : "=r" (res) : "m" (dataClass) : "cc");
        }
        break;
    case 8 : 
        switch(test) {
        case FPCLASSIFY :
            __asm__ ("         LDXBR 0,0\n"
                     "         TCDB  0,%1\n"
                     "         JNZ   NOTIND\n"
                     "         L     %0,%2\n"
                     "         J     FPCLASSD\n"
                     "NOTIND   TCDB  0,%3\n"
                     "         JNZ   NOTNAND\n"
                     "         L     %0,%4\n"
                     "         J     FPCLASSD\n"
                     "NOTNAND  TCDB  0,%5\n"
                     "         JNZ   NOTNRMD\n"
                     "         L     %0,%6\n"
                     "         J     FPCLASSD\n"
                     "NOTNRMD  TCDB  0,%7\n"
                     "         JNZ   NOTSUBD\n"
                     "         L     %0,%9\n"
                     "         J     FPCLASSD\n"
                     "NOTSUBD  TCDB  0,%9\n"
                     "         JNZ   FPCLASSD\n"
                     "         L     %0,%10\n"
                     "FPCLASSD EQU   *\n"
                     : "=r" (res)
                     : "m" (tstInfinite), "m" (resInfinite),
                       "m" (tstNan), "m" (resNaN),
                       "m" (tstNormal), "m" (resNormal),
                       "m" (tstSubNormal), "m" (resSubNormal),
                       "m" (tstZero), "m" (resZero)
                     : "cc");
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
            asm ("  LDXBR 0,0\n"
                 "  TCDB  0,%1\n"
                 "  JZ    CLASSAD\n"
                 "  LHI   %0,1\n"
                 "CLASSAD EQU *\n"
                 : "=r" (res) : "m" (dataClass) : "cc");
        }
        break;
    case 16 : 
        switch(test) {
        case FPCLASSIFY :
            __asm__ ("         TCXB  0,%1\n"
                     "         JNZ   NOTINX\n"
                     "         L     %0,%2\n"
                     "         J     FPCLASSX\n"
                     "NOTINX   TCXB  0,%3\n"
                     "         JNZ   NOTNANX\n"
                     "         L     %0,%4\n"
                     "         J     FPCLASSX\n"
                     "NOTNANX  TCXB  0,%5\n"
                     "         JNZ   NOTNRMX\n"
                     "         L     %0,%6\n"
                     "         J     FPCLASSX\n"
                     "NOTNRMX  TCXB  0,%7\n"
                     "         JNZ   NOTSUBX\n"
                     "         L     %0,%8\n"
                     "         J     FPCLASSX\n"
                     "NOTSUBX  TCXB  0,%9\n"
                     "         JNZ   FPCLASSX\n"
                     "         L     %0,%10\n"
                     "FPCLASSX EQU   *\n"
                     : "=r" (res)
                     : "m" (tstInfinite), "m" (resInfinite),
                       "m" (tstNan), "m" (resNaN),
                       "m" (tstNormal), "m" (resNormal),
                       "m" (tstSubNormal), "m" (resSubNormal),
                       "m" (tstZero), "m" (resZero)
                     : "cc");
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
            asm ("  TCXB  0,%1\n"
                 "  JZ    CLASSAX\n"
                 "  LHI   %0,1\n"
                 "CLASSAX EQU *\n"
                 : "=r" (res) : "m" (dataClass) : "cc");
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
