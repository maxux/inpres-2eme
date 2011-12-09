#ifndef __STACK_CLIENT_H
	#define __STACK_CLIENT_H
	
	#include <unistd.h>
	
	typedef struct client_table_t {
		pid_t pid;
		char name[20];
		int name_id;
		char alive;
		
		struct client_table_t *next;
		
	} client_table_t;
	
	void stack_client(client_table_t **head, client_table_t *new);
	void stack_client_print(client_table_t *head);
	int unstack_client(client_table_t **head, pid_t pid);
	void stack_sending_signal(client_table_t *client, int signal);
	
	void keep_alive();
	client_table_t * stack_return_client(client_table_t *client, pid_t pid);
	client_table_t * stack_search_station(client_table_t *client, char *station_name);
#endif
