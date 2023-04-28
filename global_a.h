/**
 * @file global_a.h
 * @brief Define global structures for the ASCII library.
 *
 */
 #include <pthread.h>
 #include <unistd.h>
 #include <iconv.h>
 #include <locale.h>
 #include "_Ascii_a.h"

 /** 
  * Define the macro used to obtain the pointer to the
  * ASCII library thread structure athd.   
  */
 #define athdp() ((struct ATHD *) getathdp())


 #define MAXSTRING_a 2048      /* Increase for large printf, sprintf, etc. */

/**
 * @brief Structure ATHD defines the thread specific data used by ASCII library.
 */
struct ATHD {
	char         cthdeye[4];   /* athd eye catcher                    */
	pid_t        pid;          /* process id                          */
	pthread_t    threadid;     /* thread id                           */
	iconv_t      cd_EtoA;      /* EBCDIC to ASCII iconv descriptor    */
	iconv_t      cd_AtoE;      /* ASCII to EBCDIC iconv descriptor    */
	void         *getenvp;     /* getenv return buffer                */
	int          getenvlen;    /* length of getenv return buffer      */

	char         *epathname;   /* ebcdic path name                    */
	int          initdone;     /* athd data area  initialization complete   */
	char         *estring1_a;  /* ebcdic string returned by __getEstring1_a */
	char         *estring2_a;  /* ebcdic string returned by __getEstring2_a */
	char         *estring3_a;  /* ebcdic string returned by __getEstring3_a */
	char         *estring4_a;  /* ebcdic string returned by __getEstring4_a */
	struct lconv *elconv_a;    /* Ptr to local copy of lconv         */
	} ;

typedef struct ATHD ATHD_t;
