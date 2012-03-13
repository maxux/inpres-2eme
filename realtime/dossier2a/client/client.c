#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define PID_SIZE	sizeof(pid_t) * 8

int main(int argc, char *argv[]) {
	pid_t mypid, server;
	int i;
	
	if(argc < 2) {
		fprintf(stderr, "[-] Missing arguments\n");
		return 1;
	}
	
	mypid  = getpid();
	server = (pid_t) atoi(argv[1]);

	printf("[+] Client PID: %d\n", (int) mypid);
	printf("[+] Server PID: %d\n", (int) server);
	
	for(i = 0; i < PID_SIZE; i++) {
		kill(server, (mypid & 1) ? SIGUSR1 : SIGUSR2);
		mypid = mypid >> 1;
	}
	
	printf("[+] Sent: %d bits\n", PID_SIZE);
	sleep(10);
	
	return 0;
}

