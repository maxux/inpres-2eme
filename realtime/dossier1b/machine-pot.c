#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>

#include "machine-pot.h"

semap_global_t *shared;

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

void spout(void *sem) {
	return;
}

int jar(semap_global_t *shared, int model) {
	int i, svalue;
	
	i = 0;
	
	/* Checking place on tapis */
	if(sem_getvalue(&(shared->tapis), &svalue)) {
		perror("sem_getvalue");
		return 1;
	}
	
	printf("[ ][%05d] Place left on tapis: %d\n", (int) getpid(), svalue);
	
	if(svalue > 0) {
		printf("[+][%05d] Waiting free spout...\n", (int) getpid());
		
		/* Decrementing tapis */
		if(sem_wait(&(shared->tapis))) {
			perror("sem_post");
			return 1;
		}
		
		if(sem_wait(&(shared->total))) {
			perror("sem_post");
			return 1;
		}
		
		printf("[+][%05d] Searching first free spout...\n", (int) getpid());
		
		// Searching free spout
		for(i = 0; i < model; i++) {
			if(sem_getvalue((sem_t *) &(shared->spouts) + (sizeof(sem_t) * i), &svalue)) {
				perror("sem_getvalue");
				return 1;
			}
			
			if(svalue == 1) {
				printf("[+][%05d] Got it: %d !\n", (int) getpid(), i);
				
				/* Leaving tapis */
				if(sem_post(&(shared->tapis)) != 0) {
					perror("sem_post");
					return 1;
				}
				
				/* Marking spout as used */
				if(sem_wait((sem_t *) &(shared->spouts) + (sizeof(sem_t) * i)) != 0) {
					perror("sem_trywait");
					return 1;
				}

				printf("[+][%05d] Fill in...\n", (int) getpid());

				usleep(FILL_IN_TIME * 1000000);
				
				printf("[+][%05d] Leaving spout...\n", (int) getpid());
				
				/* Marking spout as ready */
				if(sem_post((sem_t *) &(shared->spouts) + (sizeof(sem_t) * i)) != 0) {
					perror("sem_post");
					return 1;
				}
				
				/* Updating total spout status */
				if(sem_post(&(shared->total)) != 0) {
					perror("sem_post");
					return 1;
				}
				
				return 0;
			}
		}
		
		/* Should not happen */
		printf("[-][%05d] WTF ?\n", (int) getpid());
		return 1;
	}

	/* Tapis full */
	printf("[-][%05d] No free space. Dropping.\n", (int) getpid());
}

int main(int argc, char *argv[]) {
	int model, i, j, wait, svalue;
	pid_t forking;
	
	printf("[+] Initializing...\n");
	
	/* Check Machine Model */
	if(argc > 1) {
		model = atoi(argv[1]);
		if(model <= 0) {
			fprintf(stderr, "[-] Invalid Model\n");
			return 1;
		}
		
	} else model = DEFAULT_MODEL;
	
	printf("[+] Model: %d\n", model);

	/* Shared Memory */
	printf("[+] Initializing shared memory...\n");
	
	// Mapping:
	// [<--------------- Shared Memory --------------->]
	//
	// [total][tapis]   [spout_1][spout_2][spout_3][...]
	// [<---------->]   [<---------------------------->]
	// semap_global_t            sem_t * model
	//

	if((shared = mmap(NULL, sizeof(semap_global_t) + (sizeof(sem_t) * model), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == (void *) -1) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	/* Semaphores */
	printf("[+] Initializing semaphores...\n");
	
	/* Allocating total spout semaphores */
	if(sem_init(&(shared->total), 1, model)) {
		perror("sem_init total");
		return 1;
	}
	
	/* Allocating global semaphore */
	if(sem_init(&(shared->tapis), 1, SIMUL_POT)) {
		perror("sem_init glob");
		return 1;
	}
	
	/* Allocating semaphores, one per machine */
	for(i = 0; i < model; i++) {
		/* Mutex: free or not */
		if(sem_init((sem_t *) &(shared->spouts) + (sizeof(sem_t) * i), 1, 1)) {
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
			// spout(sems + i);
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
			return jar(shared, model);
			
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
