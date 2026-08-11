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
           
/**
 * Rounding modes
 */
#ifndef FE_TONEAREST
#  define FE_TOWARDZERO 0
#  define FE_TONEAREST  1
#  define FE_UPWARD     2
#  define FE_DOWNWARD   3
#endif

#pragma export(__class_a)
#pragma export(__round_a)
#pragma export(__roundf_a)
#pragma export(__roundl_a)
#pragma export(__trunc_a)
#pragma export(__truncf_a)
#pragma export(__truncl_a)
#pragma export(__lround_a)
#pragma export(__lroundf_a)
#pragma export(__lroundl_a)
#pragma export(__fesetround_a)
#pragma export(__fegetround_a)
#pragma export(__fabsf_a)
#pragma export(__fabsl_a)
#pragma export(__erff_a)
#pragma export(__erfcf_a)
#pragma export(__erfl_a)
#pragma export(__erfcl_a)
#pragma export(__lgamma_a)
#pragma export(__lgammaf_a)
#pragma export(__lgammal_a)
#pragma export(__nexttoward_a)
#pragma export(__exp2_a)
#pragma export(__exp2f_a)
#pragma export(__exp2l_a)
#if 0
#pragma export(__llround_a)
#pragma export(__llroundf_a)
#pragma export(__llroundl_a)
#endif

#pragma map(__class_a, "\174\174CLASSB")
#pragma map(__round_a, "\174\174ROUN\174B")
#pragma map(__roundf_a, "\174\174ROUNFB")   
#pragma map(__roundl_a, "\174\174ROUNLB")   
#pragma map(__trunc_a, "\174\174TRNC\174B")
#pragma map(__truncf_a, "\174\174FTRC\174B")   
#pragma map(__truncl_a, "\174\174LTRC\174B")   
#pragma map (__lround_a, "LROUND")
#pragma map (__lroundf_a, "LROUNDF")
#pragma map (__lroundl_a, "LROUNDL")
#pragma map (__fesetround_a, "fesetround")
#pragma map (__fegetround_a, "fegetround")
#pragma map (__fabsf_a, "FABSF")
#pragma map (__fabsl_a, "FABSL")
#pragma map (__erff_a, "\174\174FERF\174B")
#pragma map (__erfcf_a, "\174\174FEFC\174B")
#pragma map (__erfl_a, "\174\174LERF\174B")
#pragma map (__erfcl_a, "\174\174LEFC\174B")
#pragma map (__lgamma_a, "\174\174LGMAB9")
#pragma map (__lgammaf_a, "\174\174FLGA\174B")
#pragma map (__lgammal_a, "\174\174LLGA\174B")
#pragma map (__nexttoward_a, "\174\174NXTW\174B")
#pragma map (__exp2_a, "\174\174EXP2\174B")
#pragma map (__exp2f_a, "\174\174FXP2\174B")
#pragma map (__exp2l_a, "\174\174LXP2\174B")

static int roundingMode = 0;

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
 * @brief Round a float
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

/**
 * @brief Truncate a double
 */
double
__trunc_a(double g)
{
    double res;

    asm ("  FIDBR   0,3,0\n"
         "  STDY    0,%0\n"
         : "=m" (res) : : "cc");
    
    return res;
}

/**
 * @brief Truncate a float
 */
float
__truncf_a(float f)
{
    float res;

    asm ("  FIEBR   0,3,0\n"
         "  STEY    0,%0\n"
         : "=m" (res) : : "cc");
    
    return res;
}

/**
 * @brief Truncate a long double
 */
long double
__truncl_a(double g)
{
    union {
        long double res;
        double d[2];
    } r;

    asm ("  FIXBR   0,3,0\n"
         "  STDY    0,%0\n"
         "  STDY    2,%1\n"
         : "=m" (r.d[0]), "=m" (r.d[1]) : : "cc");
    
    return r.res;
}

/**
 * @brief Round a double to long int
 */
long int
__lround_a(double g)
{
    long int r;

    asm ("  CGDBR   0,1,0\n"
         "  STG     0,%0\n"
         : "=m" (r) : : "cc");
    
    return r;
}

/**
 * @brief Round a float to long int
 */
long int
__lroundf_a(float g)
{
    long int r;

    asm ("  CGEBR   0,1,0\n"
         "  STG     0,%0\n"
         : "=m" (r) : : "cc");
    
    return r;
}

/**
 * @brief Round a long double to long int
 */
long int
__lroundl_a(long double g)
{
    long int r;

    asm ("  CGXBR   0,1,0\n"
         "  STG     0,%0\n"
         : "=m" (r) : : "cc");
    
    return r;
}

/**
 * @brief Set the rounding mode
 */
int
__fesetround_a(int mode)
{
    switch (mode) {
        case FE_TONEAREST:
        case FE_UPWARD: 
        case FE_DOWNWARD:
        case FE_TOWARDZERO:
            roundingMode = mode;
            return 0;
            break;
        default :
            return -1;
    }
}

/**
 * @brief Round a long double to long int
 */
int
__fegetround_a()
{
    return roundingMode;
}

/**
 * @brief Return absolute value of float
 */
float
__fabsf_a(float a)
{
    return (float)(fabs((double)a));
}

/**
 * @brief Return absolute value of long double
 */
float
__fabsl_a(float a)
{
    union {
        long double res;
        double d[2];
    } r;

    asm ("  LPXBR   0,0\n"
         "  STDY    0,%0\n"
         "  STDY    2,%1\n"
         : "=m" (r.d[0]), "=m" (r.d[1]) : : "cc");

    return r.res;
}

#undef lgamma

/**
 * @brief Return log gamma of double
 */
double
__lgamma_a(double a)
{
    return (lgamma((double)a));
}

/**
 * @brief Return log gamma of float
 */
float
__lgammaf_a(float a)
{
    return (float)(lgamma((double)a));
}

/**
 * @brief Return log gamma of long double
 * * For large positive values of x, we use Stirling's approximation 
 * to maintain high precision:
 * ln Gamma(x) ^42^29^28 (x - 0.5)ln(x) - x + 0.5ln(2^77^20) + 1/(12x) - 1/(360x^3)
 */
long double
__lgammal_a(long double x)
{
    // Stirling's approximation is very accurate for larger x
    if (x > 8.0L) {
        const long double LOG_SQRT_TWO_PI = 0.9189385332046727417803297364056176L;
        long double x_inv = 1.0L / x;
        long double x_inv2 = x_inv * x_inv;
        
        long double res = (x - 0.5L) * logl(x) - x + LOG_SQRT_TWO_PI;
        res += (x_inv / 12.0L) - (x_inv2 * x_inv / 360.0L);
        return res;
    }

    // Fallback to double-precision lgamma for smaller or negative values.
    // Note: lgamma(x) handles the sign of the gamma function via the global 'signgam'.
    return (long double)lgamma((double)x);
}

/**
 * @brief Return the error function for float
 */
float
__erff_a(float a)
{
    return (float)(erf((double)a));
}

/**
 * @brief Return the complementary error function for float
 */
float
__erfcf_a(float a)
{
    return (float)(erfc((double)a));
}

/**
 * @brief Return the error function for long double
 */
long double
__erfl_a(long double x)
{
    // For very small x, the Taylor series is highly accurate and efficient
    if (fabsl(x) < 1.0L) {
        long double sum = x;
        long double term = x;
        long double x2 = x * x;
        
        // 2 / sqrt(pi) in long double precision
        const long double TWO_OVER_SQRT_PI = 1.1283791670955125738961589031215452L;

        for (int n = 1; n < 50; n++) {
            term *= -x2 / n;
            long double next_term = term / (2 * n + 1);
            sum += next_term;
            if (fabsl(next_term) < 1e-20L) break; // Precision threshold
        }
        return TWO_OVER_SQRT_PI * sum;
    }

    // Fallback: For larger x, if we don't have a long-double continued fraction
    // implementation, we use the double-precision erf().
    // Note: This loses precision beyond the 15-17 decimal digits of a double.
    return (long double)erf((double)x);
}

/**
 * @brief Return the complementary error function for float
 */
long double
__erfcl_a(long double a)
{
    static long double one = 1.0;

    return (one - erfl(a));
}

/**
 * @brief Next toward
 */
double
__nexttoward_a(double x, long double y)
{
    union double_bits {
        double d;
        uint64_t u;
    } hx;

    /*
     * Handle NAN case
     */
    if (isnan(x) || isnan((double)y)) {
        return x + (double)y; 
    }

    /*	
	 * 2. Handle when x equals y
     */
    if (x == (double)y) {
        return (double)y; // Returns y, preserving correct sign if x == 0
    }

    /*
     * 3. Handle x == 0.0 (Special case: finding the smallest subnormal)
     */
    if (x == 0.0) {
        hx.u = 1ULL; // Smallest positive subnormal bit pattern
        return (y < 0.0L) ? -hx.d : hx.d;
    }

    /*
     * Extract the sign bit (MSB)
     */
    uint64_t sign = hx.u & 0x8000000000000000ULL;

    /*
     * 4. Determine direction: do we move away from or toward zero?
     *    If x and (y - x) have the same sign, we move away from zero (magnitude increases)
     */
    if ((x > 0.0 && y > (long double)x) || (x < 0.0 && y < (long double)x)) {
        // Moving AWAY from zero: increment magnitude
        if (sign) {
            hx.u--; // Negative numbers get closer to -Infinity by decreasing integer value
        } else {
            hx.u++; // Positive numbers get closer to +Infinity by increasing integer value
        }
    } else {
        // Moving TOWARD zero: decrement magnitude
        if (sign) {
            hx.u++; // Negative numbers get closer to zero by increasing integer value
        } else {
            hx.u--; // Positive numbers get closer to zero by decreasing integer value
        }
    }

    /*
     * 5. Handle overflow checking
     *    If the exponent bits become all 1s (0x7FF...) and mantissa is 0, it became Infinity
     */
    if ((hx.u & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL) {
        // Trigger overflow warning flag dynamically via standard math operations
        volatile double overflow = 1e300;
        overflow *= overflow; 
    }

    return hx.d;
}

/*
 * High-precision constant representation of ln(2)
 */
#define LN2 0.693147180559945309417232121458176568L

/*
 * @brief Return exp2()
 */
double 
__exp2_a(double x) {
    return exp(x * (double)LN2);
}

/*
 * @brief Return exp2f()
 */
float 
__exp2f_a(float x) {
    return expf(x * (float)LN2);
}

/*
 * @brief Return exp2l()
 */
long double
__exp2l_a(long double x) {
    return expl(x * (long double)LN2);
}
