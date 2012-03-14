#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#define PID_SIZE	sizeof(pid_t) * 8
#define CLOCK_REALTIME	0
#define MAX_CLIENT	64

#define SIG_1	SIGUSR1
#define SIG_0	SIGUSR2

timer_t killme;

void debug(pid_t mypid) {
	unsigned int i;
	
	printf("[+] Reverse Binary: ");
	
	for(i = 0; i < PID_SIZE; i++) {
		printf("%d ", (mypid & 1) ? 1 : 0);
		mypid = mypid >> 1;
	}
	
	printf("\n");
}

void sighandler(int sig) {
	struct itimerspec tval;
	
	if(sig == SIGTERM) {
		timer_gettime(killme, &tval);
		
		if(tval.it_value.tv_sec != 0 || tval.it_value.tv_nsec != 0)
			printf("[-] Time left: %d.%ld\n", (int) tval.it_value.tv_sec, tval.it_value.tv_nsec);
		
		exit(EXIT_SUCCESS);
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

int main(int argc, char *argv[]) {
	pid_t mypid, server;
	union sigval val;
	struct itimerspec tval;
	struct sigevent sev;
	struct timespec ts;
	unsigned int i;
	int id;
	
	if(argc < 2) {
		fprintf(stderr, "[-] Missing arguments\n");
		return 1;
	}
	
	mypid  = getpid();
	server = (pid_t) atoi(argv[1]);

	printf("[+] Client PID: %d\n", (int) mypid);
	
	/* Random Client ID */
	clock_gettime(CLOCK_REALTIME, &ts);
	srand(ts.tv_nsec);
	
	id            = rand() % MAX_CLIENT;
	val.sival_int = id;
	printf("[+] Client ID : %d\n", id);
	
	printf("[+] Server PID: %d\n", (int) server);
	debug(mypid);
	
	printf("[+] Init Timer...\n");
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo  = SIGTERM;
	sev.sigev_value.sival_ptr = &killme;
	
	timer_create(CLOCK_REALTIME, &sev, &killme);
	
	tval.it_value.tv_sec  = 5;
	tval.it_value.tv_nsec = 20900000;
	
	tval.it_interval.tv_sec  = 0;
	tval.it_interval.tv_nsec = 0;
	
	timer_settime(killme, 0, &tval, NULL);
	signal_intercept(SIGTERM, sighandler);
	
	printf("[+] Sending data...\n");
	
	for(i = 0; i < PID_SIZE; i++) {
		/* Bit à 1 */
		if(mypid & 1) {
			val.sival_int = (id | ~(0xFFFFFFFF / 2));
		
		/* Bit à 0 */
		} else val.sival_int = id;
					
		sigqueue(server, SIGRTMIN, val);
		mypid = mypid >> 1;
	}
	
	printf("[+] Sent: %u bits, waiting...\n", (unsigned int) PID_SIZE);
	pause();
	
	return 0;
}

