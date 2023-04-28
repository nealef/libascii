/**
 * @file ctype_a.c
 * @brief Contains ASCII isxxxx functions for use by OE/MVS	
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
 ********************************************************************/

/********************************************************************
 * Copyright:   Licensed Materials - Property of IBM.               *
 *              (C) Copyright IBM Corp. 1997.                       *
 *              All rights reserved.                                *
 ********************************************************************/

#ifdef GEN_PRAGMA_EXPORT
#pragma export(__isalnum_a)
#pragma export(__isalpha_a)
#pragma export(__iscntrl_a)
#pragma export(__isdigit_a)
#pragma export(__isgraph_a)
#pragma export(__islower_a)
#pragma export(__isprint_a)
#pragma export(__ispunct_a)
#pragma export(__isspace_a)
#pragma export(__isupper_a)
#pragma export(__isxdigit_a)
#pragma export(__tolower_a)
#pragma export(__toupper_a)
#endif

#pragma map(__isalnum_a, "\174\174A00210")
#pragma map(__isalpha_a, "\174\174A00211")
#pragma map(__iscntrl_a, "\174\174A00213")
#pragma map(__isdigit_a, "\174\174A00214")
#pragma map(__isgraph_a, "\174\174A00215")
#pragma map(__islower_a, "\174\174A00216")
#pragma map(__isprint_a, "\174\174A00217")
#pragma map(__ispunct_a, "\174\174A00218")
#pragma map(__isspace_a, "\174\174A00219")
#pragma map(__isupper_a, "\174\174A00220")
#pragma map(__isxdigit_a, "\174\174A00221")
#pragma map(__tolower_a, "\174\174A00222")
#pragma map(__toupper_a, "\174\174A00223")

#define ASC_A   0x41
#define ASC_a   0x61
#define ASC_F   0x46
#define ASC_f   0x66
#define ASC_Z   0x5a
#define ASC_z   0x7a
#define ASC_0   0x30
#define ASC_9   0x39

#define ASC_SP  0x20
#define ASC_CR  0x0d
#define ASC_LF  0x0a
#define ASC_VT  0x0b
#define ASC_HT  0x09
#define ASC_FF  0x0c

/*%PAGE																*/
/********************************************************************/
/*																	*/
/* ASCII front-end routines for CTYPE functions						*/
/*																	*/
/********************************************************************/
 
int
__isalnum_a( int c )
{
	return( ( c >= ASC_A && c <= ASC_Z ) || 
			( c >= ASC_a && c <= ASC_z ) || 
			( c >= ASC_0 && c <= ASC_9 ) );
}

int
__isalpha_a( int c )
{
	return( ( c >= ASC_A && c <= ASC_Z ) || ( c >= ASC_a && c <= ASC_z ) );
}

int
__iscntrl_a( int c ) 
{
	return( (c >= '\x00' && c <= '\x1F') || (c == '\x7F') );   
} 

int
__isdigit_a( int c )
{
	return( c >= ASC_0 && c <= ASC_9 );
}

int
__isgraph_a( int c)
{
	return( (c > '\x20' && c < '\x7F'));  
}

int
__islower_a( int c )
{
	return( c >= ASC_a && c <= ASC_z );
}

int
__isprint_a( int c)
{
	return( (c >= '\x20' && c < '\x7F'));  
}

int
__ispunct_a( int c)
{
	return(( (c >= '\x21' && c <= '\x2f') || \
			 (c >= '\x3a' && c <= '\x40') || \
			 (c >= '\x5b' && c <= '\x60') || \
			 (c >= '\x7b' && c <= '\x7e') )   ); 
}

int
__isspace_a( int c )
{
	return( c == ASC_SP || c == ASC_LF || c == ASC_HT || 
			c == ASC_VT || c == ASC_CR || c == ASC_FF );
}

int
__isupper_a( int c )
{
	return( c >= ASC_A && c <= ASC_Z );
}

int
__isxdigit_a( int c )
{
	return( ( c >= ASC_0 && c <= ASC_9 ) || 
			( c >= ASC_a && c <= ASC_f ) || 
			( c >= ASC_A && c <= ASC_F ) );
}

int
__tolower_a( int c )
{
	return( (c >= ASC_A && c <= ASC_Z) ? c - ASC_A + ASC_a : c );
}

int
__toupper_a( int c )
{
	return( (c >= ASC_a && c <= ASC_z) ? c - ASC_a + ASC_A : c );
}
