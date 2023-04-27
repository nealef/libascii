#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include "_Ascii_a.h"  

main(int argc,char * argv[],char * env[])
{
	long int l2;
	char *myenv;
	char world[14];
	char mybuf[200];
	FILE *stream;
	double dd;
	DIR *dir;
	int i;
	double f;
	struct dirent *entry;
	char myd_name[80];

	__argvtoascii_a(argc, argv);
	printf("The value of argc is: %d \n\n",argc);
	printf("Comand line arguments:\n");
	for (i=0; i < argc ; i++)
	printf("\n    argv[%d]: '%s'\n", i, argv[i]);


	myenv = getenv("PATH");
	printf("\nEnvironement Variable PATH=%s\n",myenv);

	strcpy(world,"hello, world");
	printf(":%s:\n",world);
	printf(":%10s:\n",world);
	printf(":%.10s:\n",world);
	printf(":%-10s:\n",world);
	printf(":%.15s:\n",world);
	printf(":%-15s:\n",world);
	printf(":%15.10s:\n",world);
	printf(":%-15.10s:   :%-15.10s:\n",world,world);

	sprintf(mybuf,":%-15.10s:   :%-15.10s:\n",world,world);
	printf(mybuf);

	i='z';
	printf("Print the letter z after :%c\n",i);
	printf("Print the letter z after :%10.10c\n",i);

	i=45;
	printf(" 45 =%-10d:\n",i);

	f=atof("45.678");
	printf(" 45.678 =%-10g:\n\n",f);

	dir = opendir("."); /* opendir on current directory */
	i = 0;
	printf("List the files in the current directory using opendir/readdir\n");
	while ((entry = readdir(dir)) != NULL) {
		if (((i++)%4) == 0)
			printf("\n");
		printf("%-19.19s",entry->d_name);
	}
	printf("\n");

	exit(0);
}

