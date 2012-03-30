#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "petra.h"

#define USE_FATHER	0
#define USE_SON		1

int father_policy;
int son_policy;

struct sched_param father_priority;
struct sched_param son_priority;

int first;

void init_vars(char *argv[]) {
	if(!strcmp(argv[1], "FIFO")) {
		printf("[+] Father: FIFO\n");
		father_policy = SCHED_FIFO;
	}
	
	if(!strcmp(argv[1], "RR")) {
		printf("[+] Father: ROUND ROBIN\n");
		father_policy = SCHED_RR;
	}
	
	if(!strcmp(argv[2], "FIFO")) {
		printf("[+] Son   : FIFO\n");
		son_policy = SCHED_FIFO;
	}
	
	if(!strcmp(argv[2], "RR")) {
		printf("[+] Son   : ROUND ROBIN\n");
		son_policy = SCHED_RR;
	}
	
	father_priority.sched_priority = atoi(argv[3]);
	son_priority.sched_priority    = atoi(argv[4]);
	
	if(!strcmp(argv[5], "FATHER"))
		first = USE_FATHER;
		
	else first = USE_SON;
	
	printf("[+] Father Priority: %d\n", father_priority.sched_priority);
	printf("[+] Son    Priority: %d\n", son_priority.sched_priority);
}

int main(int argc, char *argv[]) {
	pid_t frk;
	int i = 0;
	
	/* Timer */
	struct itimerspec tval;
	struct sigevent sev;
	timer_t killme;
	
	/* Capteurs */
	int fd_petra_in;
	u_capt capt;
	
	capt.byte = 0;
	
	if(argc < 6) {
		printf("Arguments: father_policy son_policy father_priority son_priority whois_first\n");
		return 1;
	}
	
	init_vars(argv);
	
	/* Scheduling */
	if(sched_setscheduler(0, father_policy, &father_priority))
		perror("sched_setscheduler");
	
	/* Forking */
	frk = fork();
	if(!frk) {
		printf("[+] Forked...\n");
		if(sched_setscheduler(0, son_policy, &son_priority))
			perror("sched_setscheduler");
		
		sev.sigev_notify = SIGEV_SIGNAL;
		sev.sigev_signo  = SIGTERM;
		
		timer_create(CLOCK_REALTIME, &sev, &killme);
		
		tval.it_value.tv_sec  = 30;
		tval.it_value.tv_nsec = 0;
		
		tval.it_interval.tv_sec  = 0;
		tval.it_interval.tv_nsec = 0;
		
		timer_settime(killme, 0, &tval, NULL);

		if(first == USE_FATHER)
			sleep(1);
			
		printf("[+] Infinite loop...\n");
		while(1)
			i++;
		
		exit(EXIT_SUCCESS);
	}
	
	/* Opening Petra */
	fd_petra_in = open("/dev/capteursPETRA", O_RDONLY);
	
	if(first == USE_SON)
		sleep(1);
			
	while(1) {
		read(fd_petra_in, &capt.byte, 1);
		printf("[+] Capteur: %4x\n", capt.capt.S);
		sleep(1);
	}
	
	return 0;
}
