#
#  Makefile to create archive "libascii.a"
#
 
 
# To build with debug option, uncomment the following
# DEBUG		= -g
 
 
# Define compiler options
#		CC			= Compiler name
#		CFLAGS		= Compiler flags
#
CC		= xlc
CPPFLAGS = -D_XOPEN_SOURCE=500 -D_XOPEN_SOURCE_EXTENDED=1 -D__VM__ -D_UNIX03_SOURCE
CFLAGS	= -O2 -c $(DEBUG) $(CPPFLAGS) -I./ -D_ALL_SOURCE -qxplink -qlanglvl=extended:extc89:extc99 \
		  -qfloat=ieee -qlongname -q32 -qseverity=e=CCN3296 

# Define list of programs to be processed
OBJS	= ctype_a.o dirent_a.o dlfcn_a.o dll_a.o dynit_a.o \
		  fcntl_a.o fnmatch_a.o grp_a.o iconv_a.o \
		  inet_a.o ipc_a.o langin_a.o locale_a.o init_a.o \
		  netdb2_a.o nl_typ_a.o \
		  print_a.o pwd_a.o regex_a.o rexec_a.o scanf_a.o stat_a.o \
		  stdio_a.o stdlib_a.o string_a.o time_a.o trans_a.o \
		  unistd_a.o utime_a.o utsnam_a.o wchar_a.o


all: 	libascii.a

# Define archive dependencies and archive command
libascii.a	: $(OBJS) _Ascii_a.h global_a.h
	ar -rc libascii.a $(OBJS) 

# Define compile dependencies and command for each part
.c.o		: 

$(OBJS)		: _Ascii_a.h global_a.h
 
clean:
	rm -f *.a *.o

dist:	libascii.tar.gz
		tar -czf $@ LICENSE Makefile *.c *.h samples/*.c

##### END OF FILE #####
