#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

pid_t client = 0;
unsigned int nbits = 0;

void debug(pid_t mypid) {
	unsigned int i;
	
	printf("[+] Reverse Binary: ");
	
	for(i = 0; i < sizeof(pid_t) * 8; i++) {
		printf("%d ", (mypid & 1) ? 1 : 0);
		mypid >>= 1;
	}
	
	printf("\n");
}

void sighandler(int sig) {
	if(sig == SIGUSR1 || sig == SIGRTMIN) {
		client >>= 1;
		nbits++;
			
		client &= (0xFFFFFFFF / 2);

	} else if(sig == SIGUSR2 || sig == SIGRTMIN) {
		client >>= 1;
		nbits++;
			
		client |= ~(0xFFFFFFFF / 2);
	}
	
	// debug(client);
	
	if(nbits == 32) {
		debug(client);
		printf("[+] Got 32 bits, killing: %d\n", client);
		kill(client, SIGTERM);
		
		client = 0;
		nbits  = 0;
	}
}

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = SA_SIGINFO;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

void * th(void *a) {
	while(1) {
		printf("[ ] Received: %d bits\n", nbits);
		sleep(2);
	}
	
	return a;
}

int main(void) {
	pthread_t thr;
	
	printf("[+] PID: %d\n", (int) getpid());
	
	signal_intercept(SIGUSR1, sighandler);
	signal_intercept(SIGUSR2, sighandler);
	signal_intercept(SIGRTMIN, sighandler);
	signal_intercept(SIGRTMAX, sighandler);
	
	if(pthread_create(&thr, NULL, th, NULL))
		perror("pthread_create");
	
	while(1) {
		sleep(32);
	}
	
	return 0;
}
