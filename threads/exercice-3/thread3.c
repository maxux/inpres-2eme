#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct thread_text_t {
	char *data;
	int timeout;
	
} thread_text_t;

pthread_mutex_t mut;
pthread_cond_t cond;
int nbthreads;

void * thread_works(void *d) {
	thread_text_t *t;
	
	t = d;
	
	// Waiting
	usleep(t->timeout * 1000000);
	printf(">> %s - %d\n", t->data, t->timeout);
	
	pthread_mutex_lock(&mut);
	nbthreads--;
	pthread_mutex_unlock(&mut);
	
	pthread_cond_signal(&cond);
}

void * thread_dsp_time() {
	time_t t;
	
	while(1) {
		t = time(NULL);
		
		printf("\033[s \033[0;0H\033[K ->> %s", ctime(&t));
		
		printf("\033[u");
		fflush(stdout);
		
		usleep(1000000);
	}
}

int main(void) {
	pthread_t hour, *th;
	int i;
	thread_text_t t[] = {
		{"Wagner", 5},
		{"MerceD", 2},
		{"Hello", 3},
		{"Uhuhu", 6},
		{"Synchro", 3},
		{"", 0}
	};
	
	system("clear");
	printf("\n");
	
	/* Threading clock */
	if(pthread_create(&hour, NULL, thread_dsp_time, NULL)) {
		perror("pthread_create");
		return 1;
	}
	
	nbthreads = 0;
	i = 0;
	
	while(t[i++].timeout)
		nbthreads++;
	
	th = (pthread_t*) malloc(sizeof(pthread_t) * nbthreads);
	
	for(i = 0; i < nbthreads; i++) {
		if(pthread_create(th+i, NULL, thread_works, &t[i])) {
			perror("pthread_create");
			return 1;
		}
	}
	
	pthread_mutex_lock(&mut);
	while(nbthreads)
		pthread_cond_wait(&cond, &mut);
		
	pthread_mutex_unlock(&mut);
	
	return 0;
}
