#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

pid_t client = 0;
int i = 0;

void sighandler(int sig) {
	switch(sig) {
		case SIGUSR1:
			i++;
			// printf("USR1\n");
		break;
		
		case SIGUSR2:
			i++;
			// printf("USR2\n");
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

void * th(void *a) {
	while(1) {
		printf("%d\n", i);
		sleep(2);
	}
	
	return a;
}

int main(void) {
	pthread_t thr;
	
	printf("[+] PID: %d\n", (int) getpid());
	
	signal_intercept(SIGUSR1, sighandler);
	signal_intercept(SIGUSR2, sighandler);
	
	if(pthread_create(&thr, NULL, th, NULL))
		perror("pthread_create");
	
	while(1) {
		sleep(32);
	}
	
	return 0;
}
