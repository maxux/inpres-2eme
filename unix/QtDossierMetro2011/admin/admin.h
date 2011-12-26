#ifndef __ADMIN_H
	#define __ADMIN_H
	
	/* SYSTEM GLOBAL */
	typedef struct global_t {
		FILE *log;
		int *mkey_id;
		int *skey_id;
		
	} global_t;
	
	int check_login(char *passwd);
	int request_login();
#endif
