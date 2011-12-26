#ifndef __CONTROL_H
	#define __CONTROL_H
	
	#include <pthread.h>
	#include <sqlite3.h>
	#include <unistd.h>
	#include <sys/types.h>
	
	#include "stack_client.h"
	#include "metro_protocol.h"
	
	#define ADMIN_PASSWORD		"helloworld"
	
	/* Code Prototypes */
	void sighandler(int signal);
	int signal_intercept(int signal, void (*function)(int));
	void stopping_server();
	
	int request_admin(metro_protocol request, pid_t pid, struct client_table_t *clients);
	
	/* Global Variables */
	typedef struct global_t {
		int running;
		FILE *log;
		
		int *mkey_id;
		int *skey_id;
		char *shm;
		struct client_table_t **clients_head;
		pthread_t *ads;
		pthread_t *ping;
		
		sqlite3 *ads_db;		
		int *ads_index;
		int ads_count;
		
		int admin_msg;
		
	} global_t;
#endif
