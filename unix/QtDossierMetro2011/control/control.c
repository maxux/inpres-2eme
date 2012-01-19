#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sqlite3.h>

/* Demo */
#include <sys/sem.h>

#include "metro_protocol.h"
#include "ipc_messages.h"
#include "control.h"
#include "stack_client.h"
#include "station_data.h"
#include "ads.h"
#include "debug.h"

global_t sys;

int main(void) {
	message_t message;	/* Message Queue */
	key_t mkey, skey;	/* Message key, Shared Memory key */
	int mkey_id, skey_id;	/* Effective Message key, Effective Shared Memory key */
	int message_len;	
	// pid_t leader_pid;	/* Group Process leader pid */
	char *shm;		/* Shared Memory Pointer */
	int i, id;		/* Counter, station id (admin) */
	
	pid_t forking;		/* PID of the fork (chemin) */
	pthread_t ads, ping;	/* Threads for multiple processing */
	
	ads_struct_t *ads_data;
	client_table_t **client_head, *clients, *new_client = NULL, *client_temp = NULL;
	
	metro_position_t *mpos;	/* Metro Position (from GereMetro) */
	
	char *buffer;		/* Another buffer (SQL) */
	
	clients = NULL;
	client_head = &clients;
	
	/* Init Logs */
	sys.log = fopen("../log/control.log", "w");
	if(!sys.log) {
		perror("fopen");
		return 2;
	}
	
	debug("DBG: Starting up control process (PID: %d)\n", (int) getpid());
	
	/* Init Global Variables */
	sys.running      = 1;
	sys.mkey_id      = &mkey_id;
	sys.skey_id      = &skey_id;
	sys.clients_head = &clients;
	
	sys.ping         = NULL;
	sys.ads          = NULL;
	
	sys.ads_db	 = NULL;
	sys.ads_index	 = NULL;
	sys.ads_count	 = 0;
	
	sys.admin_msg	 = 0;
	
	/* Intercepting Signals */
	signal_intercept(SIGINT, sighandler);
	
	signal_intercept(SIGCHLD, sighandler);
	
	/* Init SQLite */
	sys.ads_db = ads_init();
	if(!sys.ads_db)
		return 1;
	
	if(!ads_load())
		return 1;
	
	/* Creating Message Queue */
	debug("DBG: Creating Message Queue\n");
	
	mkey = MESSAGE_KEY_ID;
	if((mkey_id = msgget(mkey, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	/* Creating Shared Memory Segment */
	debug("DBG: Creating Shared Memory Segment\n");
	skey = SHARED_MEMORY_ID;
	if((skey_id = shmget(skey, SHARED_MEMORY_SIZE, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 2;
	}
	
	if((shm = shmat(skey_id, NULL, 0)) == (void *) -1) {
		perror("shmat");
		return 2;
	}
	
	/* Semaphore demo:
	 * 
	 * semkey_id = semget(semakey, 1, IPC_CREAT);
	 * 
	 */
	
	/* Linking shm to global */
	sys.shm     = shm;
	
	/* Clearing Shared Memory Segment */
	memset(shm, '\0', SHARED_MEMORY_SIZE);
	
	/* Fallback safe message */
	strcpy(shm, "Advertissment");
	
	/* Starting threads */
	debug("THR: Threading ads processing...\n");
	if(pthread_create(&ads, NULL, (void *) ads_show, NULL) != 0)
		return 1;
	
	sys.ads = &ads;
	
	debug("THR: Threading ping processing...\n");
	if(pthread_create(&ping, NULL, (void *) keep_alive, NULL) != 0)
		return 1;
	
	sys.ping = &ping;
	
	while(sys.running) {		
		printf("DBG: Waiting Messages...\n");
		
		/*
		 *	Server listen on lType 1. Each message to the server must be send with this type. 
		 */
		message_len = msgrcv(mkey_id, (struct message_t *) &message, sizeof(message_t), 1L, 0);
		if(message_len == -1) {
			if(errno == EINTR) {
				debug("DBG: System Call interrupted. Resuming...\n");
				continue;
			}
				
			perror("msgrcv");
			return 1;
		}
		
		debugn("RAW: Message from PID: %d (opcode: 0x%02x)\n", (int) message.pid, message.request);
		
		switch(message.request) {
			case ACK_PONG:
				client_temp = stack_return_client(clients, message.pid);
				
				if(client_temp != NULL) {
					client_temp->alive = 1;
					
				} else debugc("ERR: Pong message from an unknown client ! (Pid: %d)\n", (int) message.pid);
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
					debugc("ERR: Request station not allowed. Pid %d skipped.\n", (int) message.pid);
					break;
				}
				
				/* Client already logged in */
				client_temp = stack_search_station(clients, message.text);
				if(client_temp != NULL) {
					send_message(ERR_DENIED, (void*) "Station already registred", message.pid, 0);
					debugc("ERR: A second instance of station claimed by %d. Original is %d.\n", (int) message.pid, (int) client_temp->pid);
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
				new_client->admin = 0;
				new_client->next  = NULL;
				strncpy(new_client->name, message.text, sizeof(new_client->name));
				
				stack_client(client_head, new_client);
				stack_client_print(clients);
				
				send_message(ACK_LOGIN, (void*) "Welcome :)", message.pid, 0);
				
				/* Sending first ping */
				kill(message.pid, SIGPWR);
				
				/*
				 * if(leader_pid == 0)
				 * 	leader_pid = message.pid;
				 * 
				 * send_message(ACK_LOGIN, (void*) leader_pid, message.pid, 0);
				 * 
				 */
			break;
			
			case QRY_METRO_MOVE:
				debug("QRY: Metro Move (Position)\n");
				
				mpos = (metro_position_t*) message.text;
				debug("MOV: Line #%d is on station #%d (Next: #%d)\n", mpos->id, mpos->station, mpos->next);
				
				/* Searching Client PID */
				client_temp = stack_search_station(clients, stations[mpos->next].station);
				if(client_temp != NULL) {
					debugn("MOV: Client online, sending signal\n");
					kill(client_temp->pid, SIGCONT);
				}
				
				send_message(ACK_METRO_MOVE, (void*) "Rulz", message.pid, 0);
			break;
			
			case QRY_LOGOUT:
				debug("QRY: User logout: %d\n", (int) message.pid);
				/* TODO: Check validity */
				
				if(unstack_client(client_head, message.pid) != 1)
					debugc("ERR: Unstacking failed. WTF ?\n");
				
				stack_client_print(clients);
			break;
			
			case QRY_ADMIN_LOGIN:
				if(strcmp(message.text, ADMIN_PASSWORD) == 0) {
					debugn("ADM: Admin connected from pid %d\n", (int) message.pid);
					
					new_client = (client_table_t *) malloc(sizeof(client_table_t));
					if(!new_client) {
						debugc("ERR: Allocation failed. Client skipped\n");
						break;
					}
						
					new_client->pid   = message.pid;
					new_client->alive = 1;
					new_client->admin = 1;
					new_client->next  = NULL;
					strncpy(new_client->name, "ADMIN", sizeof(new_client->name));
					
					stack_client(client_head, new_client);
					stack_client_print(clients);
					
					send_message(ACK_ADMIN_LOGIN, (void*) "Access granted", message.pid, 0);
					
				} else {
					debugc("ADM: Admin request failed from pid %d\n", (int) message.pid);
					send_message(ERR_DENIED, (void*) "Wrong password", message.pid, 0);
				}
			break;
			
			case QRY_PATHLIST:
				debug("QRY: Path List Request (%d)\n", (int) message.pid);
				
				if(!send_message(ACK_PATHLIST, stations, message.pid, sizeof(stations)))
					debugc("Cannot send map\n");
				
				debug("OK : Path List Sent\n");
				
			break;
			
			case QRY_LINESLIST:
				debug("QRY: Lines List Request (%d)\n", (int) message.pid);
				
				if(!send_message(ACK_LINESLIST, lignes, message.pid, sizeof(lignes)))
					debugc("Cannot send lines\n");
				
				debug("OK : Lines List Sent\n");
			break;
			
			case QRY_NODESLIST:
				debug("QRY: Nodes List Request (%d)\n", (int) message.pid);
				
				if(!send_message(ACK_NODESLIST, nodes, message.pid, sizeof(nodes)))
					debugc("Cannot send nodes\n");
				
				debug("OK : Nodes List Sent\n");
			break;
			
			case QRY_SEARCH:
				debug("QRY: New PathFinding...\n");
				debug("FRK: Forking chemin...\n");
				
				forking = fork();
				
				if(forking == 0) {
					/* We are the child */
					execl("../chemin/chemin", "../chemin/chemin", (char*) NULL);
					perror("execl");
					exit(0);
					
				} else if(forking > 0) {
					debug("FRK: Chemin forked, sending data...\n")					
					send_message(QRY_SEARCH_DATA, (void*) message.text, forking, sizeof(ask_pathway_t));
					
				} else debugc("FKR: Failed\n");
			break;
			
			case QRY_SHUTDOWN:
				debug("QRY: Shutting down server\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					debugn("ADM: Shutting down request validated !\n");
					send_message(ACK_SHUTDOWN, (void*) "Okay master", message.pid, 0);
					stopping_server();
				}
			break;
			
			case QRY_ADMIN_MESSAGE:
				debug("QRY: Admin Message\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					/* Stopping ads thread */
					debugn("ADM: Stopping ads thread...\n");
					if(pthread_cancel(*(sys.ads)))
						debugc("ERR: Error while stopping ads thread\n");
					
					pthread_join(*(sys.ads), NULL);
					
					/* Setting message to shm */
					debugn("ADM: Setting message...\n");
					strcpy(shm, message.text);
					sys.admin_msg = 1;
					
					/* Restarting ads thread */
					debug("THR: Threading ads processing...\n");
					if(pthread_create(&ads, NULL, (void *) ads_show, NULL) != 0)
						return 1;
					
					sys.ads = &ads;
					
					/* Sending ACK to admin */
					send_message(ACK_ADMIN_MESSAGE, (void*) "Okay master", message.pid, 0);
				}
			break;
			
			case QRY_DISABLE_STATION:
				debug("QRY: Disable Station\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					memcpy(&id, message.text, sizeof(id));
					
					debug("QRY: Disabling Station #%d...\n", id);
					nodes[id].visite     = -1;
					stations[id].enabled = 0;
					
					send_message(ACK_DISABLE_STATION, (void*) "Okay", message.pid, 0);
					
					stack_sending_signal(clients, SIGWINCH, 0);
				}
			break;
			
			case QRY_ENABLE_STATION:
				debug("QRY: Enable Station\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					memcpy(&id, message.text, sizeof(id));
					
					debug("QRY: Enabling Station #%d...\n", id);
					nodes[id].visite     = 0;
					stations[id].enabled = 1;
					
					send_message(ACK_ENABLE_STATION, (void*) "Okay", message.pid, 0);
					
					stack_sending_signal(clients, SIGWINCH, 0);	
				}
			break;
			
			case QRY_ADS_LIST:
				debug("QRY: Advertisement list\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					ads_data = (ads_struct_t*) malloc(sizeof(ads_struct_t));
					
					/* Sending list of ads */
					for(i = 0; i < sys.ads_count; i++) {
						ads_data->id = sys.ads_index[i];
						
						if(!ads_content(sys.ads_index[i], &(ads_data->timeout), ads_data->message, sizeof(ads_data->message)))
							debugc("ERR: SQL Error ?\n");
						
						send_message(ACK_ADS_LIST, (void*) ads_data, message.pid, sizeof(ads_struct_t));
					}
					
					/* End of list: timeout = 0 */
					ads_data->timeout = 0;
					send_message(ACK_ADS_LIST, (void*) ads_data, message.pid, sizeof(ads_struct_t));
					
					free(ads_data);
				}
			break;
			
			case QRY_ADS_INSERT:
				debug("QRY: Advertisement insert\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					ads_data = (ads_struct_t*) message.text;
					
					/* Inserting data */			
					buffer = sqlite3_mprintf("INSERT INTO ads (id, text, timeout) VALUES (NULL, '%q', %d)", ads_data->message, ads_data->timeout);
					
					if(ads_simple_query(sys.ads_db, buffer))
						send_message(ACK_ADS_INSERT, (void*) "SQL Okay. Data inserted.", message.pid, 0);
					else
						send_message(ERR_FAILED, (void*) "SQL Failed.", message.pid, 0);

					sqlite3_free(buffer);
				}
			break;
	
			case QRY_ADS_DELETE:
				debug("QRY: Advertisement delete\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					buffer = sqlite3_mprintf("DELETE FROM ads WHERE id = %d", atoi(message.text));
					
					if(ads_simple_query(sys.ads_db, buffer)) {
						/* Forcing commit */
						ads_load();
						
						send_message(ACK_ADS_DELETE, (void*) "Rulz data rm", message.pid, 0);
						
					} else  send_message(ERR_FAILED, (void*) "SQL Failed.", message.pid, 0);
					
					sqlite3_free(buffer);
				}
			break;
			
			case QRY_ADS_COMMIT:
				debug("QRY: Request commit ads\n");
				
				if(request_admin(message.request, message.pid, clients)) {
					if(ads_load())
						send_message(ACK_ADS_COMMIT, (void*) "Commitz rulz. Have fnu wiz it", message.pid, 0);
					else
						send_message(ERR_FAILED, (void*) "Commit failed", message.pid, 0);
				}
			break;
			
			default:
				debugc("ERR: Invalid opcode 0x%x\n", message.request);
		}
	}
	
	stopping_server();
	
	return 0;
}

int request_admin(metro_protocol request, pid_t pid, client_table_t *clients) {
	if(!stack_check_admin(clients, pid)) {
		debugc("ERR: Admin Request (0x%x) from not grant station #%d. Denied.\n", request, (int) pid);
		send_message(ERR_DENIED, (void*) "Permission denied", pid, 0);
		return 0;
		
	} else return 1;
}

void stopping_server() {
	debug("DBG: Stopping advertissment thread...\n");
	
	/* Closing SQL */
	ads_close();
	
	/* Stopping thread */
	if(pthread_cancel(*(sys.ads)))
		debugc("ERR: Error while stopping ads thread\n");
	
	pthread_join(*(sys.ads), NULL);
	
	debug("DBG: Stopping ping thread...\n");
	
	/* Stopping thread */
	if(pthread_cancel(*(sys.ping)))
		debugc("ERR: Error while stopping ping thread\n");
	
	pthread_join(*(sys.ping), NULL);
	
	debug("DBG: Cleaning Messages Queue...\n");
	
	/* Closing message queue */
	if(msgctl(*(sys.mkey_id), IPC_RMID, NULL) < 0)
		perror("msgctl");
	
	debug("DBG: Cleaning Shared Memory Area...\n");
	
	/* Closing shared memory */
	if(shmctl(*(sys.skey_id), IPC_RMID, NULL) < 0)
		perror("shmctl");
	
	fclose(sys.log);
	
	exit(0);
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
	
	/* Ignoring Zombies Process */
	if(signal == SIGCHLD)
		sig.sa_flags	 = SA_NOCLDWAIT;
		
	else sig.sa_flags	 = 0;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}
