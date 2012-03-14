#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define CLOCK_REALTIME	0

timer_t killme;
struct itimerspec tval;

void sighandler(int sig) {
	printf("[+] Signal %d. Server alive\n", sig);
	timer_settime(killme, 0, &tval, NULL);
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

int main(void) {
	struct sigevent sev;
	
	printf("[+] Display PID: %d\n", (int) getpid());
	
	signal_intercept(SIGUSR1, sighandler);
	
	printf("[+] Init Timer...\n");
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo  = SIGTERM;
	sev.sigev_value.sival_ptr = &killme;
	
	timer_create(CLOCK_REALTIME, &sev, &killme);
	
	tval.it_value.tv_sec  = 8;
	tval.it_value.tv_nsec = 59000000;
	
	tval.it_interval.tv_sec  = tval.it_value.tv_sec;
	tval.it_interval.tv_nsec = tval.it_value.tv_nsec;
	
	timer_settime(killme, 0, &tval, NULL);
	
	printf("[+] Waiting...\n");
	
	while(1)
		pause();
	
	return 0;
}


