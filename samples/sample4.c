/*
 * sample4 - This program shows an example of code performs localeconv()
 *           setlocale()
 *
*/

#define _OPEN_THREADS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pthread.h>
#include "_Ascii_a.h" 

main()
{
	char * string1;
    char *string, *stopstring;
    long l;
	unsigned long ul;
    int bs;
	struct lconv * mylocale;
	mylocale = localeconv();

	/* Display default decimal point */
	printf("Default decimal point is a %s\n",mylocale->decimal_point);
	if (NULL != (string1 = setlocale(LC_ALL, "Fr_CA.IBM-1047")))
	{
		mylocale = localeconv();
		/* A comma is set to be the decimal point when the 
		   locale if Fr_CA.IBM-1047 */
		printf("Frenc-speaking Canadian decimal point is a %s\n", 
		          mylocale->decimal_point);
	}
	else {
		printf("setlocale(LC_ALL, Fr_CA.IBM-1047) returned <NULL>\n");
	}
 
    string = "10110134932";
    printf("\nstring = %s\n",string);
    for (bs=2; bs<= 8; bs *=2)
    {        
        l = strtol(string, &stopstring, bs);
        printf(" strtol = %ld (base %d)\n", l, bs);
        printf(" Stopped scan at %s\n\n", stopstring);
    }

	l = strtol(string, &stopstring, 10);
	printf(" strtol = %ld (base %d)\n", l, 10);
	printf(" Stopped scan at %s\n\n", stopstring);
    l = strtol(string, NULL, 10);
    printf(" strtol = %ld (base %d) with Null Pointer \n\n", l, 10);
    ul = strtoul(string, NULL, 4);
    printf(" strtoul = %ld (base %d) with Null Pointer \n\n", ul, 4);

	string = "$%#&";
    printf("\nstring = %si which should fail.\n",string);
	l = strtol(string, &stopstring, 10);
	printf(" strtol = %ld (base %d)\n", l, 10);
	printf(" Stopped scan at %s\n\n", stopstring);

	pthread_exit("");
	return ;
}

