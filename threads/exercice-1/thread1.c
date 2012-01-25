#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct thread_occurence_t {
	int id;
	char *occurence;
	char *filename;
	
} thread_occurence_t;

void * thread_works(void *d) {
	int *ret;
	thread_occurence_t *data = (thread_occurence_t *) d;
	FILE *fp;
	char buffer[1024], *temp;
	
	/* Init Return */
	ret = (int*) malloc(sizeof(int));
	*ret = 0;
	
	fp = fopen(data->filename, "r");
	if(!fp) {
		perror("fopen");
		return ret;
	}
	
	while(fgets(buffer, sizeof(buffer), fp)) {
		temp = buffer;
		while((temp = strstr(temp, data->occurence) + strlen(data->occurence)) != (void*) strlen(data->occurence)) {
			printf("[%d] %*s\n", data->id, data->id, "*");
			(*ret)++;
		}
	}
	
	fclose(fp);
	
	return ret;
}

void thread_prepare(thread_occurence_t *o, int id, char *occurence, char *filename) {
	o->id        = id;
	o->occurence = occurence;
	o->filename  = filename;
}

int main(void) {
	thread_occurence_t t_oc[4];
	pthread_t t_id[4];
	int *t_ret[4], i;
	
	thread_prepare(&t_oc[0], 0, "printf", "/usr/include/stdio.h");
	thread_prepare(&t_oc[1], 1, "pthread", "/usr/include/pthread.h");
	thread_prepare(&t_oc[2], 2, "void", "/usr/include/cursesw.h");
	thread_prepare(&t_oc[3], 3, "boolean", "/usr/include/bfd.h");
	
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
