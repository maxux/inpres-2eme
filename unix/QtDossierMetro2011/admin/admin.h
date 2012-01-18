#ifndef __ADMIN_H
	#define __ADMIN_H
	
	#include <setjmp.h>
	
	/* SYSTEM GLOBAL */
	typedef struct global_t {
		FILE *log;
		int *mkey_id;
		int *skey_id;
		
		/* Signal Fallback */
		jmp_buf jump_buffer;
		
	} global_t;
	
	int check_login(char *passwd);
	int request_login();
#endif
