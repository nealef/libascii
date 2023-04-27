/*
 * sample1 - This program shows an example of code that is dependent on
 *           the letters A through Z being contiguous.  In the 
 *           EBCDIC character set this is not true.  For example, the 
 *           character \ falls  between the letters A through Z in the 
 *           EBCDIC 1047 code page. When the ASCII ISO8859-1 code page is
 *           used the letters A through Z are contiguous and the program
 *           produces the correct results.
 *
 * c89 -I.. -o sample1 sample1.c                         
 * sample1
 *   Letter A converted to lower case is:a
 *   Letter \ converted to lower case is: <<<<< Unprintable character
 *   Letter Z converted to lower case is:z
 *
 * c89 -D__STRING_CODE_SET__="ISO8859-1" -I.. -c sample>
 * c89 -o sample1 sample1.o ../libascii.a                
 * sample1
 *   Letter A converted to lower case is:a
 *   Letter \ converted to lower case is:\
 *   Letter Z converted to lower case is:z
 *
*/

#include <stdio.h>
#include "_Ascii_a.h" 

/* Function lower() from K&R */
int lower(int c)
{
	if (c >= 'A' && c <='Z') {
		/* Some programmers may try to be more efficient by coding
		 *    return c | 0x00000040; 
		 * which only works for ASCII code pages.
		 */
		return c + 'a' - 'A';
	}
	else
		return c;
}
main()
{
	int c='\\';

	printf("  Letter A converted to lower case is:%c\n",lower('A'));
	printf("  Letter %c converted to lower case is:%c\n",c,lower(c));
	printf("  Letter Z converted to lower case is:%c\n",lower('Z'));
}

