#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "control.h"
#include "stack_client.h"
#include "metro_protocol.h"
#include "debug.h"

extern global_t sys;

void keep_alive() {
	client_table_t *client, *backup;
	
	while(1) {
		debug("PNG: Pinging...\n");
		
		client = *(sys.clients_head);
		backup = client;
		
		/* Setting everyone offline */
		while(client != NULL) {
			client->alive = 0;
			client = client->next;
		}
		
		/* Sending "Ping" */
		stack_sending_signal(backup, SIGPWR);
		
		/* Waiting 2 seconds */
		usleep(200000);
		
		/* Checking who is not alive */
		client = backup;
		
		/* Checking who if offline */
		while(client != NULL) {
			if(!client->alive) {
				debugc("ERR: Client %d ping timeout\n", client->pid);
				unstack_client(sys.clients_head, client->pid);
			}
				
			client = client->next;
		}
		
		/* Next ping in 10 seconds */
		usleep(PROTO_PING_INTERVAL * 1000000);
	}
}

void stack_client(client_table_t **head, client_table_t *new) {
	/* Stack Empty ? It becode the first */
	if(*head == NULL) {
		*head = new;
		return;
	}
	
	/* Adding Client */
	new->next = *head;
	*head = new;
}

void stack_client_print(client_table_t *head) {
	/* Stack Empty */
	if(head == NULL) {
		debug("STK: No client associated\n");
		return;
	}
	
	debug("STK: -- Clients associated:\n");
	
	/* Loop Writing */
	while(head != NULL) {
		debug("STK:  + Name: %-20s (PID: %d)\n", head->name, head->pid);
		head = head->next;
	}
}

int unstack_client(client_table_t **head, pid_t pid) {
	client_table_t *previous, *follow;
	
	/* Stack empty */
	if(*head == NULL)
		return 0;
	
	/* First Element */
	if((*head)->pid == pid) {
		previous = *head;
		*head = (*head)->next;
		
		free(previous);
		return 1;
	}
	
	/* Searching Element */
	follow = *head;
	while(follow != NULL && follow->pid != pid) {
		previous = follow;
		follow = follow->next;
	}
	
	/* Comparing if not last */
	if(follow->pid == pid) {
		previous->next = follow->next;
		free(follow);
		return 1;
	}
	
	return 0;
}

void stack_sending_signal(client_table_t *client, int signal) {
	while(client != NULL) {
		debug("STK: Sending signal %d to %d\n", signal, client->pid);
		
		if(kill(client->pid, 0) != -1)
			kill(client->pid, signal);
			
		else debugc("ERR: Client %d seems to be dead...\n", client->pid);
		
		client = client->next;
	}
}

client_table_t * stack_return_client(client_table_t *client, pid_t pid) {
	while(client != NULL) {
		if(client->pid == pid)
			return client;
		
		client = client->next;
	}
	
	return NULL;
}

client_table_t * stack_search_station(client_table_t *client, char *station_name) {
	while(client != NULL) {
		if(strcmp(client->name, station_name) == 0)
			return client;
		
		client = client->next;
	}
	
	return NULL;
}
