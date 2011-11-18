#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include "../control/Control.h"

void sig_handler(int signum) {
	int id;
	
	switch(signum) {
		case SIGALRM:
			printf("SIGALRM intercepted\n");
		break;
		
		case SIGUSR1:
			// printf("SIGUSR1 intercepted\n");
			id = setpgid(getpid(), 32439);
			perror("setpgid");
		break;
		
		case SIGUSR2:
			printf("SIGUSR2 intercepted\n");
		break;
	}
}

int main(void) {
	message_t message;
	key_t mkey;
	int key_id, message_len;
	pid_t leader_pid;
	
	/* Intercept ALARM */
	signal(SIGALRM, sig_handler);
	
	/* Intercept SIGUSR1: Advertissment */
	signal(SIGUSR1, sig_handler);
	
	/* Incercept SIGUSR2: Sys Admin Message */
	signal(SIGUSR2, sig_handler);
	
	/* Creating Message Queue */
	mkey = 1342;
	key_id = msgget(mkey, IPC_CREAT | 0666);
	if(key_id == -1) {
		debugc("Cannot create message key\n");
		return 1;
	}
	
	printf("DEBUGGER -- Sending...\n");
	message.lType = 1L;
	message.request = PROTO_QRY_LOGIN;
	message.pid = getpid();
	/* MessageEnvoie.message[0] = 1;
	MessageEnvoie.message[1] = Arrivee; */
	strcpy(message.text, "Hello World");
	
	if(msgsnd(key_id, &message, sizeof(message) - sizeof(long), 0)) {
		fprintf(stderr, "Err. de msgsnd: %d %s", __LINE__, __FILE__);
		exit(1);
	}

	printf("DEBUGGER -- Waiting...\n");
	message_len = msgrcv(key_id, (struct message_t *) &message, sizeof(message_t), 0L, 0);
	if(message_len == -1) {
		debugc("Reading Message Failed\n");
		return 1;
	}
	getchar();
}
