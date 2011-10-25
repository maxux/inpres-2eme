#ifndef __APPLIC_H
	#define __APPLIC_H
	
	#ifdef DEBUG
		#define debug(...) fprintf(stderr, __VA_ARGS__)
	#else
		#define debug(...) //
	#endif
#endif
