#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sched.h>

#include "../common.h"

pid_t pid;
unsigned char destination;

void diep(char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

void connecting() {
	union sigval value = {.sival_int = destination };
	
	printf("[+] Connecting...\n");
	if(sigqueue(pid, SIGUSR1, value) == -1)
		diep("[-] sigqueue");
}

void disconnecting() {
	union sigval value = {.sival_int = destination };
	
	printf("[+] Disconnecting...\n");
	if(sigqueue(pid, SIGUSR2, value) == -1)
		diep("[-] sigqueue");
}

void sighandler(int sig) {
	switch(sig) {
		case SIGINT:
			disconnecting(pid);
			exit(EXIT_SUCCESS);
		break;
	}
}

pid_t init() {
	char buffer[64];
	FILE *fp;
	
	fp = fopen(PID_FILE, "r");
	if(!fp)
		diep("[-] fopen");
	
	if(fread(buffer, sizeof(buffer), 1, fp) != 1 && !feof(fp))
		perror("[-] fread");
	
	fclose(fp);
	
	pid = atoi(buffer);
	
	return pid;
}

int main(int argc, char *argv[]) {
	mqd_t mq;
	char buffer[255];
	size_t i;
	payload_t payload;
	struct timespec ts;
	struct mq_attr attr;
	
	if(argc < 2) {
		fprintf(stderr, "Usage: destination\n");
		return 1;
	}
	
	/* Global */
	init();
	destination = atoi(argv[1]);
	
	printf("[+] PID: %d\n", (int) pid);
	printf("[+] Destination: %d\n", destination);

	ts.tv_sec  = 3;
	ts.tv_nsec = 0;
	
	signal_intercept(SIGINT, sighandler);

	sprintf(buffer, "%s_%d", mq_names[DESTINATION], destination);
	if((mq = mq_open(buffer, O_RDONLY, 0777, NULL)) == -1)
		diep("[-] mq_open");

	while(1) {
		/* Connecting dispatcher */
		connecting();
			
		/* Pooling */
		printf("[+] Pooling...\n");
		while(!mq_getattr(mq, &attr) && attr.mq_curmsgs != (int) mq_sizes[DESTINATION])
			sched_yield();
			
		/* Disconnecting */
		disconnecting();

		/* Flushing queue */
		for(i = 0; i < mq_sizes[DESTINATION]; i++) {
			if(mq_receive(mq, (char*)&payload, sizeof(payload_t), NULL) > -1)
				printf("[+] Packet removed: %d\n", payload.data);
				
			else diep("[-] mq_receive");
		}

		/* Waiting */
		nanosleep(&ts, NULL);
	}
	
	return 0;
}

