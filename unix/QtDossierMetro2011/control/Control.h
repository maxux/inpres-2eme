#ifndef __CONTROL_H
	#define __CONTROL_H
	
	#define MESSAGE_MAX_SIZE	512
	
	typedef struct message_t {
		long lType;
		pid_t pid;
		int requete;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;
	
	typedef struct client_table_t {
		pid_t pid;
		char name[32];
		
		struct client_table_t *next;
		
	} client_table_t;

	#define PROTO_LOGIN	0x01
	#define PROTO_LOGOUT	0x02
	#define PROTO_SEARCH	0x03
	#define PROTO_SHUTDOWN	0x04
	
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
	#define __debug(...)	fprintf(stderr, __VA_ARGS__); \
				fprintf(stderr, COLOR_NONE);
	
	/* Setting Colors */
	/* Classic Debug */
	#define debug(...)	fprintf(stderr, COLOR_GREY); \
				__debug(__VA_ARGS__);
	
	/* Critical Debug */	
	#define debugc(...)	fprintf(stderr, COLOR_RED); \
				__debug(__VA_ARGS__);
#endif
