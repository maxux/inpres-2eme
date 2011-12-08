#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "control.h"
#include "stack_client.h"
#include "station_data.h"
#include "ads.h"

global_t sys;

int send_message(int request, void *data, pid_t pid, size_t bsize) {
	message_t message;	/* Message Queue */
	
	message.lType   = pid;
	message.request = request;
	message.pid     = getpid();
	
	/* Checking if it's a string or a binary data */
	if(bsize > 0) {
		/* Avoid memory over flow */
		if(bsize > sizeof(message.text))
			bsize = sizeof(message.text);
		
		/* Copy binary data */
		memcpy(message.text, data, bsize);
		
	} else strcpy(message.text, (char*) data);
		
	
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
	
	pthread_t ads;
	
	client_table_t **client_head, *clients, *new_client = NULL;
	
	clients = NULL;
	client_head = &clients;
	
	/* Init Global Variables */
	sys.running      = 1;
	sys.mkey_id      = &mkey_id;
	sys.clients_head = &clients;
	
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
	
	/* Linking shm to global */
	sys.shm     = shm;
	
	/* Clearing Shared Memory Segment */
	memset(shm, '\n', SHARED_MEMORY_SIZE);
	
	// DEBUG
	strcpy(shm, "Advertissment");
		
	/* Init Process Group */
	debug("Control PID: %d\n", getpid());
	
	/* Starting threads */
	debug("THR: Threading ads processing...\n");
	if(pthread_create(&ads, NULL, (void *) show_ads, NULL) != 0)
		return 1;
	
	while(sys.running) {
		printf("DBG: Waiting Messages...\n");
		
		/*
		 *	Server listen on lType 1. Each message to the server must be send with this type. 
		 */
		message_len = msgrcv(mkey_id, (struct message_t *) &message, sizeof(message_t), 1L, 0);
		if(message_len == -1) {
			debugc("ERR: Reading Message Failed\n");
			return 1;
		}
		
		printf("RAW: New Message from PID: %d (Type: 0x%02x) :: %s\n", message.pid, message.request, message.text);
		
		switch(message.request) {
			case PROTO_QRY_LOGIN:
				debug("QRY: New authentifcation: \n");
				/* TODO: Check Message Validity... */
				
				new_client = (client_table_t *) malloc(sizeof(client_table_t));
				if(!new_client) {
					debugc("ERR: Allocation failed. Client skipped\n");
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
				debug("QRY: User logout: %d\n", message.pid);
				/* TODO: Check validity */
				
				if(unstack_client(client_head, message.pid) != 1)
					debugc("ERR: Unstacking failed. WTF ?\n");
				
				stack_client_print(clients);
			break;
			
			/* DEBUG OPCODE */
			case 0x42:
				debug("DEBUUUUUG\n");
				stack_sending_signal(clients, SIGUSR1);
			break;
			
			case PROTO_QRY_PATHLIST:
				debug("QRY: Path List Request (%d)\n", message.pid);
				
				if(!send_message(PROTO_ACK_PATHLIST, stations, message.pid, sizeof(stations)))
					debugc("Cannot send map\n");
				
				debug("OK : Path List Sent\n");
				
			break;
			
			case PROTO_QRY_LINESLIST:
				debug("QRY: Lines List Request (%d)\n", message.pid);
				
				if(!send_message(PROTO_ACK_LINESLIST, lignes, message.pid, sizeof(lignes)))
					debugc("Cannot send lines\n");
				
				debug("OK : Lines List Sent\n");
				
			break;
			
			case PROTO_ACK_PATHLIST:
				debug("ACK: Path List\n");
			break;
			
			case PROTO_QRY_SEARCH:
				debug("QRY: New PathFinding...\n");
				/* Fucking fork(); */
			break;
			
			case PROTO_QRY_SHUTDOWN:
				debug("QRY: Shutting down...\n");
				stopping_server();
				return 2;
			break;
			
			default:
				debugc("ERR: Invalid opcode\n");
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
