#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "metro_protocol.h"
#include "ipc_messages.h"
#include "geremetro.h"
#include "metro.h"
#include "debug.h"

global_t sys;

ligne_legacy_t legacy_lignes[METRO_MAX_LIGNE];
metro_nodes_t nodes[METRO_MAX_STATION];

int main(void) {
	message_t message;	/* Message Queue */
	key_t mkey;		/* Message key */
	int mkey_id;		/* Effective Message key */
	int temp, i;
	
	int pfd[2];		/* Pipe */
	
	metro_t **metros;
	metro_position_t metro_message;
	
	/* Init Logs */
	sys.log = fopen("../log/geremetro.log", "w");
	if(!sys.log) {
		perror("fopen");
		return 2;
	}
	
	debug("DBG: Starting up metro process (PID: %d)\n", (int) getpid());
	
	/* Init Global Variables */
	sys.running      = 1;
	
	/* Intercepting Signals */
	signal_intercept(SIGINT, sighandler);
	
	/* Creating Message Queue */
	debug("DBG: Creating Message Queue\n");
	
	mkey = MESSAGE_KEY_ID;
	if((mkey_id = msgget(mkey, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	sys.mkey_id         = &mkey_id;
	
	
	/* Downloading Lines List */
	debug("QRY: Lignes List\n");
	if(!send_message(QRY_LINESLIST, (void*) "Wantz Lines List", 0))
		debugc("Cannot downloading lignes\n");
	
	read_message(&message);
	
	if(message.request == ACK_LINESLIST) {
		debug("ACK: Ligne List\n");
		memcpy(legacy_lignes, message.text, sizeof(legacy_lignes));
		
	} else debugc("Wrong opcode: %d\n", message.request);
	
	
	/* Downloading nodes list */
	debug("QRY: Nodes List\n");
	if(!send_message(QRY_NODESLIST, (void*) "Wantz Nodes List", 0))
		debugc("Cannot downloading nodes\n");
	
	read_message(&message);
	
	if(message.request == ACK_NODESLIST) {
		debug("ACK: Nodes List\n");
		memcpy(nodes, message.text, sizeof(nodes));
		
	} else debugc("ERR: Wrong opcode: 0x%x\n", message.request);
	
	
	/* Counting lines */
	temp = 0;
	sys.nbthreads = &temp;
	while(legacy_lignes[temp].couleur != LCOLOR_EOF)
		temp++;
	
	printf("Lines: %d\n", temp);
	
	/* Building Threads List */
	metros = (metro_t **) malloc(sizeof(metro_t *) * temp * 2);
	sys.metros = metros;
	
	debug("THR: Threading...\n");
	
	/* For realism on thread :D */
	srand(time(NULL));
	
	/* Init Pipes */
	if(pipe(pfd) != 0) {
		perror("pipe");
		return 1;
	}
	
	sys.pipe_read  = pfd[0];
	sys.pipe_write = pfd[1];
	
	/* Allocation and startings threads */
	for(i = 0; i < (temp * 2); i++) {
		*(metros + i) = (metro_t *) malloc(sizeof(metro_t));
		
		(*(metros + i))->line = (i > temp - 1) ? i - temp : i;
		(*(metros + i))->sens = i / temp;
		
		if(pthread_create(&(*(metros + i))->thread, NULL, (void *) metro, *(metros + i)) != 0) {
			debugc("ERR: Cannot thread metro %d\n", i);
			return 1;
		}
	}
	
	/* Waiting message form Metro */
	while(sys.running) {		
		debug("DBG: Waiting Metro activities...\n");
		
		i = read(sys.pipe_read, &metro_message, sizeof(metro_message));
		send_message(QRY_METRO_MOVE, &metro_message, sizeof(metro_message));
		read_message(&message);
		
		if(message.request != ACK_METRO_MOVE)
			debugc("ERR: Wrong Response from Server: 0x%x\n", message.request);
	}
	
	stopping_metro();
	
	return 0;
}

void stopping_metro() {
	int i;
	
	debug("DBG: Stopping threads...\n");
	for(i = 0; i < *(sys.nbthreads) * 2; i++) {
		debug("THR: Stopping threads %d\n", i);
		
		if(pthread_cancel((*(sys.metros + i))->thread))
			debugc("ERR: Error while stopping thread %d\n", i);

		pthread_join((*(sys.metros + i))->thread, NULL);
		
		free(*(sys.metros + i));
	}
	
	free(sys.metros);
	
	fclose(sys.log);
	
	exit(0);
}

void sighandler(int signal) {
	printf("\n");
	
	switch(signal) {
		case SIGINT:
			stopping_metro();
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
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}
