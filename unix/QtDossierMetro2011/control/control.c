#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "debug.h"
#include "metro_protocol.h"
#include "ipc_messages.h"
#include "control.h"
#include "stack_client.h"
#include "station_data.h"
#include "ads.h"

global_t sys;

int main(void) {
	message_t message;	/* Message Queue */
	key_t mkey, skey;	/* Message key, Shared Memory key */
	int mkey_id, skey_id;	/* Effective Message key, Effective Shared Memory key */
	int message_len;	
	// pid_t leader_pid;	/* Group Process leader pid */
	char *shm;		/* Shared Memory Pointer */
	int i;			/* Counter */
	
	pthread_t ads, ping;	/* Threads for multiple processing */
	
	client_table_t **client_head, *clients, *new_client = NULL, *client_temp = NULL;
	
	clients = NULL;
	client_head = &clients;
	
	/* Init Global Variables */
	sys.running      = 1;
	sys.mkey_id      = &mkey_id;
	sys.skey_id      = &skey_id;
	sys.clients_head = &clients;
	
	/* Intercepting Signals */
	signal_intercept(SIGINT, sighandler);
	
	/* Creating Message Queue */
	debug("Creating Message Queue\n");
	
	mkey = MESSAGE_KEY_ID;
	if((mkey_id = msgget(mkey, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	/* Creating Shared Memory Segment */
	debug("Creating Shared Memory Segment\n");
	skey = SHARED_MEMORY_ID;
	if((skey_id = shmget(skey, SHARED_MEMORY_SIZE, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 2;
	}
	
	if((shm = shmat(skey_id, NULL, 0)) == (void *) -1) {
		perror("shmat");
		return 2;
	}
	
	/* Linking shm to global */
	sys.shm     = shm;
	
	/* Clearing Shared Memory Segment */
	memset(shm, '\0', SHARED_MEMORY_SIZE);
	
	// DEBUG
	strcpy(shm, "Advertissment");
		
	/* Init Process Group */
	debug("Control PID: %d\n", getpid());
	
	/* Starting threads */
	debug("THR: Threading ads processing...\n");
	if(pthread_create(&ads, NULL, (void *) show_ads, NULL) != 0)
		return 1;
	
	debug("THR: Threading ping processing...\n");
	if(pthread_create(&ping, NULL, (void *) keep_alive, NULL) != 0)
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
		
		debugn("RAW: New Message from PID: %d (Type: 0x%02x) :: %s\n", message.pid, message.request, message.text);
		
		switch(message.request) {
			case ACK_PONG:
				client_temp = stack_return_client(clients, message.pid);
				
				if(client_temp != NULL) {
					client_temp->alive = 1;
					
				} else debugc("ERR: Pong message from an unknown client ! (Pid: %d)\n", message.pid);
			break;
			
			case QRY_LOGIN:
				debug("QRY: New authentifcation from station: %s\n", message.text);
				
				i = 1;
				while(stations[i].L && stations[i].C) {
					if(strcmp(stations[i].station, message.text) == 0) {
						i = -1;
						break;
						
					} else i++;
				}
				
				/* Client denied */
				if(i > 0) {
					send_message(ERR_DENIED, (void*) "Access denied", message.pid, 0);
					debugc("ERR: Request station not allowed. Pid %d skipped.\n", message.pid);
					break;
				}
				
				/* Client already logged in */
				client_temp = stack_search_station(clients, message.text);
				if(client_temp != NULL) {
					send_message(ERR_DENIED, (void*) "Station already registred", message.pid, 0);
					debugc("ERR: A second instance of station claimed by %d. Original is %d.\n", message.pid, client_temp->pid);
					break;
				}
				
				/* Client validated */
				new_client = (client_table_t *) malloc(sizeof(client_table_t));
				if(!new_client) {
					debugc("ERR: Allocation failed. Client skipped\n");
					break;
				}
					
				new_client->pid   = message.pid;
				new_client->alive = 1;
				strncpy(new_client->name, message.text, sizeof(new_client->name));
				
				stack_client(client_head, new_client);
				stack_client_print(clients);
				
				send_message(ACK_LOGIN, (void*) "Welcome :)", message.pid, 0);
				
				/* if(leader_pid == 0)
					leader_pid = message.pid;
				
				if(setpgid(message.pid, leader_pid) == -1)
					perror("[-] setpgid"); */
			break;
			
			case QRY_LOGOUT:
				debug("QRY: User logout: %d\n", message.pid);
				/* TODO: Check validity */
				
				if(unstack_client(client_head, message.pid) != 1)
					debugc("ERR: Unstacking failed. WTF ?\n");
				
				stack_client_print(clients);
			break;
			
			case QRY_ADMIN_LOGIN:
				if(strcmp(message.text, ADMIN_PASSWORD) == 0) {
					debugn("ADM: Admin connected from pid %d\n", message.pid);
					send_message(ACK_ADMIN_LOGIN, (void*) "Access granted", message.pid, 0);
					
				} else {
					debugc("ADM: Admin request failed from pid %d\n", message.pid);
					send_message(ERR_DENIED, (void*) "Wrong password", message.pid, 0);
				}
			break;
			
			case QRY_PATHLIST:
				debug("QRY: Path List Request (%d)\n", message.pid);
				
				if(!send_message(ACK_PATHLIST, stations, message.pid, sizeof(stations)))
					debugc("Cannot send map\n");
				
				debug("OK : Path List Sent\n");
				
			break;
			
			case QRY_LINESLIST:
				debug("QRY: Lines List Request (%d)\n", message.pid);
				
				if(!send_message(ACK_LINESLIST, lignes, message.pid, sizeof(lignes)))
					debugc("Cannot send lines\n");
				
				debug("OK : Lines List Sent\n");
				
			break;
			
			case ACK_PATHLIST:
				debug("ACK: Path List\n");
			break;
			
			case QRY_SEARCH:
				debug("QRY: New PathFinding...\n");
				/* Fucking fork(); */
			break;
			
			case QRY_SHUTDOWN:
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
	
	debug("Cleaning Shared Memory Area...\n");
	
	if(shmctl(*(sys.skey_id), IPC_RMID, NULL) < 0)
		perror("shmctl");
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
