#ifndef __GEREMETRO_H
	#define __GEREMETRO_H
	
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/types.h>
	
	#include "metro_protocol.h"
	
	/* Code Prototypes */
	void sighandler(int signal);
	int signal_intercept(int signal, void (*function)(int));
	void stopping_metro();
		
	/* Global Variables */
	typedef struct metro_t {
		int line;
		int sens;
		pthread_t thread;
		
	} metro_t;
	
	typedef struct metro_position_t {
		int id;
		int current;
		int station;
		int next;
		
	} metro_position_t;
	
	typedef struct global_t {
		int running;
		FILE *log;
		
		int *mkey_id;
		metro_t **metros;
		int *nbthreads;
		int pipe_read;
		int pipe_write;
		
	} global_t;
#endif
