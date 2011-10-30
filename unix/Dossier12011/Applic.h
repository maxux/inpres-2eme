#ifndef __APPLIC_H
	#define __APPLIC_H
	
	#ifdef DEBUG
		#define debug(...) fprintf(stderr, COLOR_GREY); fprintf(stderr, __VA_ARGS__); fprintf(stderr, COLOR_NONE);
	#else
		#define debug(...) //
	#endif
#endif
