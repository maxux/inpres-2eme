#ifndef __CONTROL_H
	#define __CONTROL_H
	
	#include <pthread.h>
	
	#define ADMIN_PASSWORD		"helloworld"
	
	/* Code Prototypes */
	void sighandler(int signal);
	int signal_intercept(int signal, void (*function)(int));
	void stopping_server();
	
	/* Global Variables */
	typedef struct global_t {
		int running;
		int *mkey_id;
		int *skey_id;
		char *shm;
		struct client_table_t **clients_head;
		pthread_t *ads;
		pthread_t *ping;
		
	} global_t;
#endif
