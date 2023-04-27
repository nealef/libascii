#
#  Makefile for ctype test cases
#


# Define debug option
#			 comment out define to test as FUNCTIONs
# DEBUG	= -g

# Define compiler options                    
#		CC			= compiler name
#		CASCII		= to compile with ASCII char strings
#		CINC		= include libraries
#		CFLAGS		= compiler flags
CC		= unset _CXX_OSUFFIX; cxx
CINC	= -I../
CASCII	= -D__STRING_CODE_SET__="ISO8859-1" 
CFLAGS	= -c $(DEBUG) -D_ALL_SOURCE $(CINC) 


sample1	sample2	sample3 sample4 : sample1.o sample2.o sample3.o sample4.o
	$(CC) -o sample1  sample1.o ../libascii.a
	$(CC) -o sample2  sample2.o ../libascii.a
	$(CC) -o sample3  sample3.o ../libascii.a
	$(CC) -o sample4  sample4.o ../libascii.a

sample1.o	: sample1.c 
	$(CC) $(CFLAGS) $(CASCII) sample1.c

sample2.o	: sample2.c 
	$(CC) $(CFLAGS) $(CASCII) sample2.c

sample3.o	: sample3.c 
	$(CC) $(CFLAGS) $(CASCII) sample3.c

sample4.o	: sample4.c 
	$(CC) $(CFLAGS) $(CASCII) sample4.c

##### END OF FILE #####
