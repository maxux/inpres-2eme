#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>

#include "machine-pot.h"

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	
	/* Ignoring Zombies Process */
	if(signal == SIGCHLD)
		sig.sa_flags	 = SA_NOCLDWAIT;
		
	else sig.sa_flags	 = 0;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

void spout(semap_t *sem) {
	while(1) {
		//
		break;
	}
}

void jar(semap_t *sem) {
	while(1) {
		printf("[ ] Got spout %p\n", sem);
		usleep(20000000);
	}
}

int main(int argc, char *argv[]) {
	int model, i, j, wait, svalue;
	pid_t forking;
	semap_t *sems, sglob;
	
	printf("[+] Initializing...\n");
	
	/* Check Machine Model */
	if(argc > 1) {
		model = atoi(argv[1]);
		if(model < 0) {
			fprintf(stderr, "[-] Invalid Model\n");
			return 1;
		}
		
	} else model = DEFAULT_MODEL;
	
	printf("[+] Model: %d\n", model);

	printf("[+] Initializing shared memory...\n");
	if((sems = mmap(NULL, sizeof(semap_t) * model, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == (void *) -1) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	printf("[+] Initializing semaphores...\n");
	
	/* Allocating semaphores, one per machine */
	for(i = 0; i < model; i++) {
		(sems + i)->free = malloc(sizeof(sem_t));
		(sems + i)->used = malloc(sizeof(sem_t));
		
		if(sem_init((sems + i)->free, 1, SIMUL_POT)) {
			perror("sem_init free");
			return 1;
		}
		
		if(sem_init((sems + i)->used, 1, 0)) {
			perror("sem_init used");
			return 1;
		}
	}
	
	/* Allocating global semaphore */
	sglob = (sem_t*) malloc(sizeof(sem_t));
	if(sem_init(sglob, model * SIMUL_POT)) {
		perror("sem_init glob");
		return 1;
	}
	
	/* Removing SIGCHILD */
	signal_intercept(SIGCHLD, NULL);
	
	/* Forking bec */
	printf("[+] Spawning spouts...\n");
	for(i = 0; i < model; i++) {
		forking = fork();
		if(forking == 0) {
			spout(sems + i);
			return 0;
			
		} else if(forking > 0) {
			printf("[+] Spawned: %d\n", (int) forking);
			
		} else {
			perror("fork");
			return 1;
		}
	}
	
	printf("[+] Let's start spawning jars...\n");
	
	srand(time(NULL));
	
	while(1) {
		wait = (rand() % MAX_GEN_POT_TIME) + 1;
		usleep(wait * 1000000 / 10);
		
		printf("[+] New jar !\n");
		
		forking = fork();
		if(forking == 0) {
			// Init Cycle
			for(i = 0; i < model; i++) {
				/* Checking if there is place left */
				if(sem_getvalue((sems + i)->free, &svalue)) {
					perror("sem_getvalue");
					return 1;
				}
				
				if(svalue) {
					
					jar(sems + i);
					return 0;
					
				} else i++;
			}
			
			printf("[-] No free space. Dropping.\n");
			
			return 0;
			
		} else if(forking > 0) {
			printf("[+] Jar spawned: pid %d\n", (int) forking);
			
		} else {
			perror("fork");
			return 1;
		}
		
		i++;
	}
	
	return 0;
}
