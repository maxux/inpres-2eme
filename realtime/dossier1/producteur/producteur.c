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
#include <string.h>

#include "producteur.h"

#define SDISPO_NAME	"sema_dispo_danielancia"
#define SPROD_NAME	"sema_prod_danielancia"
#define SHM_NAME	"shm_danielancia"

#define MAX_PLACE	20

jmp_buf jump_buffer;

int main(void) {
	sem_t *sema_prod, *sema_dispo;
	int shm;
	int i, waittime;
	char *shmarea;
	
	printf("[+] Initializing semaphores\n");
	
	/* Opening semaphore: produit */
	if((sema_prod = sem_open(SPROD_NAME, O_CREAT, 664, 0)) == SEM_FAILED) {
		perror("(prod) sem_open");
		exit(EXIT_FAILURE);
	}
	
	/* Opening semaphore: libre */
	if((sema_dispo = sem_open(SDISPO_NAME, O_CREAT, 664, MAX_PLACE)) == SEM_FAILED) {
		perror("(dispo) sem_open");
		exit(EXIT_FAILURE);
	}
	
	printf("[+] Initializing shared memory\n");
	
	/* Opening Shared Memory Area */
	if((shm = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_TRUNC, 664)) == -1) {
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
	
	if((shmarea = mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0)) == (void *) -1) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	
	/* DEBUG */
	strcpy(shmarea, "Hello woorld !");
	
	printf("[+] Intercepting signal\n");
	
	/* Intercepting SIGINT */
	signal_intercept(SIGINT, sig_handler);
	
	/* Randomize */
	srand(time(NULL));
	
	printf("[+] Working\n");
	
	if(!setjmp(jump_buffer)) {
		for(i = 0; i < 30; i++) {
			waittime = ((rand() % 4) + 1);
			// printf("Waiting: %d\n", waittime);
			
			usleep(waittime * 1000000);
			printf("[ ] Hello world\n");
		}
	}
	
	printf("\n[+] Closing shared memory\n");
	
	/* Closing Shared Memory Area */
	if(close(shm))
		perror("(shm) close");
	
	if(shm_unlink(SHM_NAME))
		perror("shm_unlink");
	
	printf("[+] Closing semaphores\n");
	
	/* Closing semaphores */
	if(sem_close(sema_prod))
		perror("(prod) sem_close");
		
	if(sem_close(sema_dispo))
		perror("(dispo) sem_close");
	
	/* Unlink files */
	if(sem_unlink(SPROD_NAME))
		perror("(prod) sem_unlink");
		
	if(sem_unlink(SDISPO_NAME))
		perror("(dispo) sem_unlink");

	return 0;
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

void sig_handler(int signum) {
	switch(signum) {
		case SIGINT:
			longjmp(jump_buffer, SIGINT);
		break;
	}
}
