#ifndef __CONTROL_H
	#define __CONTROL_H
	
	#define ADMIN_PASSWORD		"helloworld"
	
	/* Code Prototypes */
	void sighandler(int signal);
	int signal_intercept(int signal, void (*function)(int));
	void stopping_server();
	
	/* Global Variables */
	typedef struct global_t {
		char running;
		int *mkey_id;
		int *skey_id;
		struct client_table_t **clients_head;
		char *shm;
		
	} global_t;
#endif
