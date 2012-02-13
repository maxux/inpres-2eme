#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int main(void) {
	pthread_t t_id[4];
	
	for(i = 0; i < 4; i++) {
		if(pthread_create(&t_id[i], NULL, thread_works, &t_oc[i])) {
			fprintf(stderr, "Threading failed\n");
			return 1;
		}
	}
		
	for(i = 0; i < 4; i++)
		pthread_join(t_id[i], (void**) &t_ret[i]);
	
	for(i = 0; i < 4; i++) {
		printf("Return %d (%-10s on %-30s): %d\n", i, t_oc[i].occurence, t_oc[i].filename, *t_ret[i]);
		free(t_ret[i]);
	}
	
	return 0;
}
