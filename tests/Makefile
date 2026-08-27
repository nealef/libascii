#
#  Makefile to build and run libascii tests
#
 
 
# To build with debug option, uncomment the following
DEBUG		= 
 
 
# Define compiler options
#		CC			= Compiler name
#		CFLAGS		= Compiler flags
#
CC		= xlc
CPPFLAGS = -D_XOPEN_SOURCE=600 -D_XOPEN_SOURCE_EXTENDED=1 -D__VM__ -D_UNIX03_SOURCE -D_OPEN_THREADS 
CFLAGS	= -g -O2 -c $(DEBUG) $(CPPFLAGS) -I./ -D_ALL_SOURCE -qxplink -qlanglvl=extended:extc89:extc99 \
		  -qfloat=ieee -qlongname -q32 -qseverity=e=CCN3296 -qascii
LDFLAGS = -O -g -qxplink -qdll -Wl,dll -q32 -qascii
LDAFLAGS = -O -g -qxplink -qdll -Wl,dll -q32 -qascii ../libascii.x

# Define list of programs to be processed
TESTS = mbwctests mbwctests_a


all		: 	$(TESTS)

# Define archive dependencies and archive command
mbwctests.o : mbwctests.c
			  $(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

mbwctests	: mbwctests.o
			  $(CC) $(LDFLAGS) -o $@ $^

mbwctests_a	: mbwctests.o
			  $(CC) $(LDAFLAGS) -o $@ $^

clean		:
	rm -f *.o *.dbg *.lst

run			: $(TESTS)
			  @for test in $(TESTS); do \
				  echo "Running $$test"; \
				  LIBPATH=../:$(LIBPATH) ./$$test; \
			  done


##### END OF FILE #####
