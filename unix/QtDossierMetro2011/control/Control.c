#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "Control.h"

// struct sigaction Action;
// FenetrePlanVille *F1; 

/* void sighandler(int sigid) {
	printf("-- SIGNAL: %d\n", sigid);
	
	return;
	
	switch(sigid) {
		
	}
	
	// TracePlan(F1->framePlan);
	// TRACER PLAN

	char* T[] = {
		"Dossier Unix Janvier 2011",
		"ne perdez pas de temps",
		"bon travail",
		"dddddddddddddd\ndddddddd"
	};

	static int i = 0;
	printf("%s\n",T[i]);
	// AffichePub(F1->textInformation,T[i]);
	// BALANCER PUB
	i++;
	
	if(i == 4)
		i = 0;
		
	alarm(5);
	
	return;
} */

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

int main(void) {
	message_t message;
	key_t mkey;
	int key_id, message_len;
	pid_t leader_pid;
	
	client_table_t **client_head, *clients, *new_client = NULL;
	
	clients = NULL;
	client_head = &clients;
	
	/* Creating Message Queue */
	mkey = 1342;
	key_id = msgget(mkey, IPC_CREAT | 0666);
	if(key_id == -1) {
		debugc("Cannot create message key\n");
		return 1;
	}
		
	/* Init Process Group */
	debug("Control PID: %d\n", getpid());
	
	while(1) {
		printf("Waiting Messages...\n");
		message_len = msgrcv(key_id, (struct message_t *) &message, sizeof(message_t), 0L, 0);
		if(message_len == -1) {
			debugc("Reading Message Failed\n");
			return 1;
		}
		
		printf("New Message from PID: %d (Type: 0x%02x) :: %s\n", message.pid, message.request, message.text);
		
		switch(message.request) {
			case PROTO_QRY_LOGIN:
				debug("New authentifcation: \n");
				/* TODO: Check Message Validity... */
				
				new_client = (client_table_t *) malloc(sizeof(client_table_t));
				if(!new_client) {
					debugc("Allocation failed. Client skipped\n");
					break;
				}
					
				new_client->pid = message.pid;
				strncpy(new_client->name, message.text, sizeof(new_client->name));
				
				stack_client(client_head, new_client);
				stack_client_print(clients);
				
				if(leader_pid == 0)
					leader_pid = message.pid;
				
				/* printf("-- Trying to add PID %d to PGID %d\n", message.pid, leader_pid);
				
				if(setpgid(message.pid, leader_pid) == -1)
					perror("[-] setpgid"); */
			break;
			
			case PROTO_QRY_LOGOUT:
				debug("User logout: \n");
				/* TODO: Check validity */
				
				if(unstack_client(client_head, message.pid) != 1)
					debugc("Unstacking failed. WTF ?\n");
				
				stack_client_print(clients);
			break;
			
			/* DEBUG OPCODE */
			case 0x42:
				debug("DEBUUUUUG\n");
				stack_sending_signal(clients, SIGUSR1);
			break;
			
			case PROTO_QRY_SEARCH:
				debug("New PathFinding...\n");
				/* Fucking fork(); */
			break;
			
			case PROTO_QRY_SHUTDOWN:
				debug("Shutting down...\n");
				return 2;
			break;
			
			default:
				debugc("Error: Invalid opcode\n");
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	

	/* printf("%d---\n",argc);
	if (argc != 2) {
		printf("Trop ou trop peu d'argument(s)....\n");
		exit(1);
	} */

	/* NomS = (char*) malloc(20);
	strcpy(NomS,argv[1]); */

	// Recureration des ressources

	/* Action.sa_handler = Handler;
	sigemptyset(&Action.sa_mask);
	Action.sa_flags = 0;
	if(sigaction(SIGALRM,&Action,NULL)) {
		perror("Err. de siagaction()");
		exit(1);
	}

	alarm(5);

	if ((idQ = msgget(IDQ,0)) == -1) {
		// TraceErr(__LINE__,__FILE__,"Err. de msgget()");
		exit(1);
	}

	MessageId.lType = 1L;
	MessageId.idProcess = getpid();
	MessageId.requete = PROTO_LOGIN;

	if(msgsnd(idQ,&MessageId,sizeof(pid_t) + sizeof(int),0)) {
		perror("Err. de msgsnd()");
		exit(1);
	} */

	// traiter la reponse ...

	/* WAS CALL */
}
