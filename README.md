### LIBASCII
```
  *********************************************************************
  *                                                                   *
  * libascii - ascii-ebcidic interface layer - README file            *
  *                                                                   *
  * Version 1.1.9                                                     *
  *                                                                   *
  * To report problems or ask questions send e-mail to:               *
  *                                                                   *
  *             libascii@nvet.ibm.com                                 *
  *                                                                   *
  * Copyright:   Licensed Materials - Property of IBM.                *
  *              (C) Copyright IBM Corp. 1997, 1998.                  *
  *              All rights reserved.                                 *
  *                                                                   *
  * License information:                                              *
  *                                                                   *
  *   The libascii source code is provided free of charge and         *
  *   may be distributed freely.  No fee may be charged if you        *
  *   distribute the libascii source code (except for such things     *
  *   as the price of a disk or tape, postage ).  The libascii        *
  *   makefile will compile and produce a libascii.a archive file.    *
  *   The libascii.a archive may be link edited  with any software    *
  *   vendor  product.  Any software vendor product that is link      *
  *   edit with libascii.a archive is free to distribute and charge   *
  *   for that product.                                               *
  *                                                                   *
  *   THIS PROGRAM IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY       *
  *   KIND, EXPRESS  OR IMPLIED, INCLUDING THE IMPLIED  WARRANTIES    *
  *   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.        *
  *   IBM does not warrant uninterrupted or error free operation of   *
  *   the  Program, or that the Program is free from claims by a      *
  *   third party of copyright, patent, trademark, trade secret,      *
  *   or any other intellectual property infringement. IBM  has       *
  *   no obligation to provide service, defect correction, or any     *
  *   maintenance for the Program. IBM  has  no  obligation to        *
  *   supply any Program updates or enhancements to you even if       *
  *   such are or later become available.                             *
  *                                                                   *
  *    Under no circumstances is IBM liable for any of the            *
  *    following:                                                     *
  *                                                                   *
  *      1.  third-party claims against you for losses or damages;    *
  *      2.  loss of, or damage to, your records or data; or          *
  *      3.  direct  damages,  lost  profits, lost savings,           *
  *          incidental, special, or indirect damages or other        *
  *          consequential damages,  even if IBM or its authorized    *
  *          supplier, has been advised of the possibility of         *
  *          such damages.                                            *
  *                                                                   *
  *    Some jurisdictions do not allow these limitations or           *
  *    exclusions, so they may not apply to you.                      *
  *                                                                   *
  *                                                                   *
  *********************************************************************
```
#### OVERVIEW

The libascii package helps you port ASCII-based C applications 
to the EBCDIC-based z/VM OpenExtensions environment. 

If you are porting an application which has a lot of ASCII
dependencies, then you may find there are a large number of
changes required to port the application to EBCDIC-based OS/390.

The C/C++ Run-Time Library functions support EBCDIC characters.
The libascii package provides an ascii interface layer for some of
the more commonly used C/C++ Run-Time Library functions.
libascii supports ascii input and output characters by
performing the necessary iconv() translations before and after
invoking the C/C++ Run-Time Library functions. Note that not all 
C functions are supported (see Limitations for additional 
information). 

The 1.3.x C/C++ compiler ASCII option or the xlc -qascii option to 
generate strings in ASCII.

The libascii package is as thread safe as the Run-Time library except where stated under limitations below.

Note that not all C functions are supported (see LIMITATIONS section for additional information).

In order to use the libascii functions you need to:

o   Install the libascii code.

o   Build the libascii.a archive file and libascii.so shared object.

o   Make minor modifications to your C source code and recompile, using the __STRING_CODE_SET__="ISO8859-1" flag

o   Link edit your application with either the libascii.a archive file, or libascii.so.x file.


#### INSTALLING THE LIBASCII CODE

The libascii TAR file contains all the parts required to create 
the libascii archive. To install: 

1. Create a directory where you want libascii installed. 

2. Copy the TAR file to that directory. 

3. Unwind the file using this command: 
```
     tar -xvfz libascii.tar.Z
```
The libascii source files, makefile, and readme file should 
now be installed. 

####  BUILDING THE LIBASCII ARCHIVE AND SHARED OBJECT FILES

You can use the makefile file provided to build libascii.a and libascii.so from the libascii source files. 

To build these files, cd into the directory with the libascii source files and issue the make command. 

#### USING LIBASCII

After you have installed libascii and built the archive and shared objects files, to use the libascii functions you need to: 

o  Make minor modifications to your C source code and recompile, using the ASCII or -qascii compiler options.

o  Link edit your application with either the libascii.a archive (static linking) libascii.so.x (dynamic linking) files. 

o  Make sure all the required header files are included as specified in the C/C++ Run-Time Library Reference.
   For example strncasecmp() requires <strings.h> to be included.

o   Add the following statement to your source file, after all the existing `#include` statements.
```
      #include "_Ascii_a.h"
```
o  Use either the ASCII or -qascii compiler options to cause the compiler to generate all strings defined in your program in ASCII rather than EBCDIC format.

o   Link-edit your application with the libascii.a archive or libascii.so.x files. 

####  LIMITATIONS

The libascii interface code is code that we found useful and is
offered as is for your use.  It's intended to be used as an
assistance in getting your applications running as quickly as
possible.

The following are some of the known restrictions:

o  Not all the C functions are supported.

o  For some of the supported functions there are known restrictions, as follows:

   -   GETOPT function

       --  The libascii getopt() function is not thread-safe.  The second argument is changed for a short period of 
           time from EBCDIC to ASCII and then back to EBCDIC. 

   -   PRINTF family functions

       --  The "%$n" specification is not supported in the format string.
       --  These functions are limited to 2048 bytes of output.  To increase the size of the strings and output supported
           change #define MAXSTRING_a in global_a.h

   -   SCANF family functions

       --  The "%$n" specification is not supported in the format string.
       --  The maximum number of arguments supported is 20.
       --  These functions are limited to 2048 bytes of input.  To increase the size of the strings and output supported
           change #define MAXSTRING_a in global_a.h

o  The interface layer is not NLS-enabled; it only supports
   ISO8859-1 <-> IBM1040-1 character set conversions.
