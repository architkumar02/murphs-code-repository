#include <stdio.h>		/* I/O and Strings 			*/
#include <stdlib.h>

/* Varidiac Debug Macro 
 * Set by default to 2, change with cmd line argument to preprocessor
 * -DDEBUG 1 or -DDEBUG 3, etc.
 */
#ifdef DEBUG
	#if DEBUG>=3		/* Very Verbose Debug Information */
		#define DPRINTF(format, ...) {\
			fprintf(stderr,"Debug: Line %d in %s in %s. " format,\
				__LINE__,__func__,__FILE__, ##__VA_ARGS__);\
		}	
	#elif DEBUG==2	/* Semi verbose debug information */
		#define DPRINTF(format, ...) {\
			fprintf(stderr,"Debug: Line %d in %s. " format,\
				__LINE__,__func__, ##__VA_ARGS__);\
		}
	#elif DEBUG==1	/* little debug information */
		#define DPRINTF(format, ...) {\
			fprintf(stderr,"Debug: In %s " format,\
				__func__, ##__VA_ARGS__);\
		}
	#else
		#define DPRINTF(format,..)
	#endif
#else
	#define DPRINTF(format, ...)
#endif

/* Tokenize variable names macro */
#define TOKENIZE(x) (#x)

/* Malloc Check Macor */
#define CHECK_MALLOC(ptr) {\
   ptr = (void *) ptr;           \
   if( !ptr ){                   \
         fprintf(stderr,"ERROR: Failed to malloc space for %s on line %d in     %s\n",TOKENIZE(ptr),__LINE__,__func__ ); \
         exit(EXIT_FAILURE);     \
   }                             \
}

