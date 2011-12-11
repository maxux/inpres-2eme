#ifndef __DEBUG_SYSTEM_H
	#define __DEBUG_SYSTEM_H

	/* DEBUGGING */
	#ifdef COLOR
		#define COLOR_BLUE	"\033[1;34m"
		#define COLOR_GREY	"\033[1;30m"
		#define COLOR_GREEN	"\033[1;32m"
		#define COLOR_RED	"\033[1;31m"
		#define COLOR_NONE	"\033[0m"
	#else
		#define COLOR_BLUE	""
		#define COLOR_GREY	""
		#define COLOR_GREEN	""
		#define COLOR_RED	""
		#define COLOR_NONE	""
	#endif
	
	/* Printing Message */
	#define __debug(...)	{ fprintf(stderr, __VA_ARGS__); \
				fprintf(stderr, COLOR_NONE); }
	
	/* Setting Colors */
	/* Info Message */
	#define debugn(...)	{ fprintf(stderr, COLOR_BLUE); \
				__debug(__VA_ARGS__); }
				
	/* Classic Debug */
	#define debug(...)	{ fprintf(stderr, COLOR_GREY); \
				__debug(__VA_ARGS__); }
	
	/* Critical Debug */	
	#define debugc(...)	{ fprintf(stderr, COLOR_RED); \
				__debug(__VA_ARGS__); }
#endif
