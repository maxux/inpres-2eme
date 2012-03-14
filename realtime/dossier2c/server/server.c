#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENT	64

pid_t client[MAX_CLIENT] = {0};
int nbits[MAX_CLIENT]    = {0};

pid_t dsp_pid;

void debug(pid_t mypid) {
	unsigned int i;
	
	printf("[+] Reverse Binary: ");
	
	for(i = 0; i < sizeof(pid_t) * 8; i++) {
		printf("%d ", (mypid & 1) ? 1 : 0);
		mypid >>= 1;
	}
	
	printf("\n");
}

void keepalive(int sig, siginfo_t *info, void *unused) {
	sig    = 0;
	unused = NULL;
	info   = NULL;
	
	kill(dsp_pid, SIGUSR1);
}

void sighandler(int sig, siginfo_t *info, void *unused) {
	int id;
	unused = NULL;
	
	/* Extracting ID (ignoring last bit) */
	id = info->si_value.sival_int & (0xFFFFFFFF / 2);	
	
	if(sig == SIGRTMIN) {
		client[id] >>= 1;
		nbits[id]++;
		
		/* Extracting last bit */
		if(info->si_value.sival_int & ~(0xFFFFFFFF / 2))
			client[id] |= ~(0xFFFFFFFF / 2);
		else
			client[id] &= (0xFFFFFFFF / 2);
	}
	
	// debug(client[id]);
	
	if(nbits[id] == 32) {
		debug(client[id]);
		printf("[+] Got 32 bits, killing: %d\n", (int) client[id]);
		kill(client[id], SIGTERM);
		
		client[id] = 0;
		nbits[id]  = 0;
	}
}

int signal_intercept(int signal, void (*function)(int, siginfo_t*, void*)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = (void (*)(int)) function;
	sig.sa_flags	 = SA_SIGINFO;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

int main(int argc, char *argv[]) {
	timer_t timer_dsp;
	float delay;
	struct sigevent sev;
	struct itimerspec tval;
	
	if(argc < 3) {
		fprintf(stderr, "[-] Missing arguments (PID, Delay)\n");
		return 1;
	}
	
	dsp_pid = (pid_t) atoi(argv[1]);
	delay   = atof(argv[2]);
	
	printf("[+] Server  PID: %d\n", (int) getpid());
	printf("[+] Display PID: %d (interval %f)\n", (int) dsp_pid, delay);
	
	signal_intercept(SIGUSR1, sighandler);
	signal_intercept(SIGUSR2, sighandler);
	signal_intercept(SIGRTMIN, sighandler);
	signal_intercept(SIGALRM, keepalive);
	
	/* Init Timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo  = SIGALRM;
	sev.sigev_value.sival_ptr = &timer_dsp;
	
	timer_create(CLOCK_REALTIME, &sev, &timer_dsp);
	
	tval.it_value.tv_sec  = (int) delay;
	tval.it_value.tv_nsec = (delay - (int) delay); //59000000;
	
	// Repeat Timer
	tval.it_interval.tv_sec  = tval.it_value.tv_sec;
	tval.it_interval.tv_nsec = tval.it_value.tv_nsec;
	
	timer_settime(timer_dsp, 0, &tval, NULL);
	
	printf("[+] Waiting...\n");
	
	while(1) {
		sleep(32);
	}
	
	return 0;
}
