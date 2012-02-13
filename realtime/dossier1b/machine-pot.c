#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>

#include "machine-pot.h"

void spout(int id, sem_t *sema) {
	printf("Spawned !\n");
}

int main(int argc, char *argv[]) {
	int model, i;
	sem_t *freespace;
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
	
	printf("[+] Initializing semaphores...\n");
	
	/* Allocating semaphores, one per machine */
	freespace = (sem_t*) malloc(sizeof(sem_t) * model);
	for(i = 0; i < model; i++) {
		if(sem_init(freespace + i, 1, SIMUL_POT)) {
			perror("sem_init");
			return 1;
		}
	}
	
	/* Forking bec */
	printf("[+] Spawning spout...\n");
	for(i = 0; i < model; i++) {
		forking = fork();
		if(forking == 0) {
			spout(i, freespace + i);
			return 0;
			
		} else if(fork > 0) {
			printf("[+] Spawned: %d\n", (int) forking);
			
		} else {
			perror("fork");
			return 1;
		}
	}
	
	
	return 0;
}
