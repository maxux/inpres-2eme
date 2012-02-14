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

semap_t *sems;
sem_t *tapis, *total;

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
		break;
	}
}

int jar(semap_t *sems, int model) {
	int i, svalue;
	
	i = 0;
			
	printf("%p - %p - %p\n", sems, sems->total, sems->free);

	/* Checking place */
	if(sem_getvalue(sems->tapis, &svalue)) {
		perror("sem_getvalue");
		return 1;
	}

	if(sem_wait(sems->tapis))
		perror("sem_post");

	if(svalue > 0) {
		printf("[+] Waiting free spout\n");
		
		/* Waiting free spout */
		if(sem_wait((sems + i)->total) != 0) {
			perror("sem_wait");
			return 1;
		}
		
		printf("[+] Free: %d. Searching first free spout...\n", svalue);
		
		/* Searching free spout */
		for(i = 0; i < model; i++) {
			if(sem_getvalue((sems + i)->free, &svalue)) {
				perror("sem_getvalue");
				return 1;
			}
			
			if(svalue == 1) {
				printf("[+] Got it: %d !\n", i);
				
				if(sem_trywait((sems + i)->total) != 0) {
					printf(">> %d\n", errno);
					perror("sem_trywait");
					// return 1;
				}
				
				if(sem_trywait((sems + i)->free) != 0) {
					printf("-> %d\n", errno);
					perror("sem_trywait");
					// return 1;
				}

				usleep(123 * 100000);
				
				printf("[+] Leaving spout...\n");
				
				if(sem_post((sems + i)->total) != 0) {
					perror("sem_post");
					return 1;
				}
				
				if(sem_post((sems + i)->free) != 0) {
					perror("sem_post");
					return 1;
				}
				
				return 0;
			}
		}
	}

	printf("[-] No free space. Dropping.\n");
}

int main(int argc, char *argv[]) {
	int model, i, j, wait, svalue;
	pid_t forking;
	
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

	/* Shared Memory */
	printf("[+] Initializing shared memory...\n");
	if((sems = mmap(NULL, sizeof(semap_t) * model, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == (void *) -1) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	/* Semaphores */
	printf("[+] Initializing semaphores...\n");
	
	/* Allocating total spout semaphores */
	total = (sem_t*) malloc(sizeof(sem_t));
	if(sem_init(total, 1, model)) {
		perror("sem_init total");
		return 1;
	}
	
	/* Allocating global semaphore */
	tapis = (sem_t*) malloc(sizeof(sem_t));
	if(sem_init(tapis, 1, SIMUL_POT)) {
		perror("sem_init glob");
		return 1;
	}
	
	/* Allocating semaphores, one per machine */
	for(i = 0; i < model; i++) {
		(sems + i)->total = total;
		(sems + i)->tapis = tapis;
		(sems + i)->free  = malloc(sizeof(sem_t));
		
		/* Mutex: free or not */
		if(sem_init((sems + i)->free, 1, 1)) {
			perror("sem_init free");
			return 1;
		}
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
			return jar(sems, model);
			
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
