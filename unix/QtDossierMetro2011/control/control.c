#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#include "control.h"
#include "stack_client.h"
#include "station_data.h"

global_t sys;


int send_message(int request, void *data, pid_t pid) {
	message_t message;	/* Message Queue */
	
	message.lType   = pid;
	message.request = request;
	message.pid     = getpid();
	strcpy(message.text, (char*) data);
	
	if(msgsnd(*(sys.mkey_id), &message, sizeof(message) - sizeof(long), 0)) {
		perror("msgsnd");
		return 0;
	}
	
	return 1;
}

int main(void) {
	message_t message;	/* Message Queue */
	key_t mkey, skey;	/* Message key, Shared Memory key */
	int mkey_id, skey_id;	/* Effective Message key, Effective Shared Memory key */
	int message_len;	
	// pid_t leader_pid;	/* Group Process leader pid */
	char *shm;		/* Shared Memory Pointer */
	
	client_table_t **client_head, *clients, *new_client = NULL;
	
	clients = NULL;
	client_head = &clients;
	
	/* Init Global Variables */
	sys.running = 1;
	sys.mkey_id = &mkey_id;
	
	/* Intercepting Signals */
	signal_intercept(SIGINT, sighandler);
	
	/* Creating Message Queue */
	debug("Creating Message Queue\n");
	
	mkey = MESSAGE_KEY_ID;
	if((mkey_id = msgget(mkey, IPC_CREAT | 0666)) < 0) {
		debugc("Cannot create message key\n");
		return 1;
	}
	
	/* Creating Shared Memory Segment */
	debug("Creating Shared Memory Segment\n");
	skey = SHARED_MEMORY_ID;
	if((skey_id = shmget(skey, SHARED_MEMORY_SIZE, IPC_CREAT | 0666)) < 0) {
		debugc("Cannot create shared memory segment\n");
		return 2;
	}
	
	if((shm = shmat(skey_id, NULL, 0)) == (void *) -1) {
		debugc("Cannot attach shared memory segment id\n");
		return 2;
	}
	
	/* Clearing Shared Memory Segment */
	memset(shm, '\n', SHARED_MEMORY_SIZE);
	
	// DEBUG
	strcpy(shm, "Hello World");
		
	/* Init Process Group */
	debug("Control PID: %d\n", getpid());
	
	while(sys.running) {
		printf("Waiting Messages...\n");
		message_len = msgrcv(mkey_id, (struct message_t *) &message, sizeof(message_t), 0L, 0);
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
				
				/* if(leader_pid == 0)
					leader_pid = message.pid;
				
				if(setpgid(message.pid, leader_pid) == -1)
					perror("[-] setpgid"); */
			break;
			
			case PROTO_QRY_LOGOUT:
				debug("User logout: %d\n", message.pid);
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
			
			case PROTO_QRY_PATHLIST:
				debug("Path List Request\n");
				if(!send_message(PROTO_ACK_PATHLIST, stations, message.pid))
					debugc("Cannot send map\n");
				
				debug("PATH LIST SENT\n");
				
			break;
			
			case PROTO_QRY_SEARCH:
				debug("New PathFinding...\n");
				/* Fucking fork(); */
			break;
			
			case PROTO_QRY_SHUTDOWN:
				debug("Shutting down...\n");
				stopping_server();
				return 2;
			break;
			
			default:
				debugc("Error: Invalid opcode\n");
		}
	}
	
	stopping_server();
	
	return 0;
}

void stopping_server() {
	debug("Cleaning Messages Queue...\n");
	
	if(msgctl(*(sys.mkey_id), IPC_RMID, NULL) < 0)
		perror("msgctl");
}

void sighandler(int signal) {
	printf("\n");
	
	switch(signal) {
		case SIGINT:
			stopping_server();
			exit(EXIT_SUCCESS);
		break;
	}
}

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = 0;
	/* sig.sa_sigaction = NULL; */
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}
