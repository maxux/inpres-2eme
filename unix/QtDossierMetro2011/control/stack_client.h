#ifndef __STACK_CLIENT_H
	#define __STACK_CLIENT_H
	
	#include <unistd.h>
	
	typedef struct client_table_t {
		pid_t pid;
		char name[32];
		
		struct client_table_t *next;
		
	} client_table_t;
	
	void stack_client(client_table_t **head, client_table_t *new);
	void stack_client_print(client_table_t *head);
	int unstack_client(client_table_t **head, pid_t pid);
	void stack_sending_signal(client_table_t *client, int signal);
#endif
