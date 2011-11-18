#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "control.h"
#include "stack_client.h"

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
		debug("[ ] No client associated\n");
		return;
	}
	
	debug("[ ] -- Clients associated:\n");
	
	/* Loop Writing */
	while(head != NULL) {
		debug("     + Name: %-20s (PID: %d)\n", head->name, head->pid);
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
		if(kill(client->pid, 0) != -1)
			kill(client->pid, signal);
			
		else debugc("[-] Client %d seems to be dead...\n", client->pid);
		
		client = client->next;
	}
}
