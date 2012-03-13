#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define PID_SIZE	sizeof(pid_t) * 8

void debug(pid_t mypid) {
	unsigned int i;
	
	printf("[+] Reverse Binary: ");
	
	for(i = 0; i < PID_SIZE; i++) {
		printf("%d ", (mypid & 1) ? 1 : 0);
		mypid = mypid >> 1;
	}
	
	printf("\n");
}

int main(int argc, char *argv[]) {
	pid_t mypid, server;
	sigval_t val;
	unsigned int i;
	
	if(argc < 2) {
		fprintf(stderr, "[-] Missing arguments\n");
		return 1;
	}
	
	mypid  = getpid();
	server = (pid_t) atoi(argv[1]);

	printf("[+] Client PID: %d\n", (int) mypid);
	printf("[+] Server PID: %d\n", (int) server);
	debug(mypid);
	
	for(i = 0; i < PID_SIZE; i++) {
		sigqueue(server, (mypid & 1) ? SIGUSR2 : SIGUSR1, val);
		mypid = mypid >> 1;
	}
	
	printf("[+] Sent: %lu bits, waiting...\n", PID_SIZE);
	pause();
	
	return 0;
}

