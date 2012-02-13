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

void * thread_works(void *d) {
	
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
	pthread_t hour;
	
	
	if(pthread_create(&hour, NULL, thread_dsp_time, NULL)) {
		perror("pthread_create");
		return 1;
	}
	
	printf("Hello World\n");
	printf("Hello World\n");
	
	pthread_join(hour, (void**) NULL);
	
		
	/* for(i = 0; i < 4; i++)
		
	
	for(i = 0; i < 4; i++) {
		printf("Return %d (%-10s on %-30s): %d\n", i, t_oc[i].occurence, t_oc[i].filename, *t_ret[i]);
		free(t_ret[i]);
	} */
	
	return 0;
}
