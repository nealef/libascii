#
#  Makefile to create archive "libascii.a"
#
 
 
# To build with debug option, uncomment the following
DEBUG		= 
 
 
# Define compiler options
#		CC			= Compiler name
#		CFLAGS		= Compiler flags
#
CC		= xlc
CPPFLAGS = -D_XOPEN_SOURCE=500 -D_XOPEN_SOURCE_EXTENDED=1 -D_OPEN_THREADS -D__VM__ -D_UNIX03_SOURCE
CFLAGS	= -O2 -c $(DEBUG) $(CPPFLAGS) -I./ -D_ALL_SOURCE -qxplink -qlanglvl=extended:extc89:extc99 \
		  -qfloat=ieee -qlongname -q32 -qseverity=e=CCN3296 -qdll -qexportall

all		: 	liboverride.so

# Define archive dependencies and archive command
liboverride.so : override.o
	${CC} -qxplink -Wl,dll -q32 $< -o $@

# Define compile dependencies and command for each part
.c.o		: 

clean:
	rm -f *.so *.o *.x *.lst *.dbg

##### END OF FILE #####
