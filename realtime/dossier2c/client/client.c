#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define PID_SIZE	sizeof(pid_t) * 8
#define CLOCK_REALTIME	0

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
	timer_t killme;
	struct itimerspec tval;
	struct sigevent sev;
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
	
	printf("[+] Init Timer...\n");
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo  = SIGTERM;
	sev.sigev_value.sival_ptr = &killme;
	
	timer_create(CLOCK_REALTIME, &sev, &killme);
	
	tval.it_value.tv_sec  = 5;
	tval.it_value.tv_nsec = 20900000;
	
	tval.it_interval.tv_sec  = tval.it_value.tv_sec;
	tval.it_interval.tv_nsec = tval.it_value.tv_nsec;
	
	timer_settime(killme, 0, &tval, NULL);
	
	printf("[+] Sending data...\n");
	
	for(i = 0; i < PID_SIZE; i++) {
		kill(server, (mypid & 1) ? SIGUSR1 : SIGUSR2);
		mypid = mypid >> 1;
		usleep(20000);
	}
	
	printf("[+] Sent: %lu bits, waiting...\n", PID_SIZE);
	pause();
	
	return 0;
}

