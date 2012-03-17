#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "thread_keymaster.h"
#include "interface.h"

pthread_t tMaitreCles;

void * threadMaitreCles(void *dummy) {
	position_t pos;
		
	/* Initializing mutex */
	pthread_mutex_init(&mutexNbCles, NULL);
	
	while(1) {
		while(get_nbcle() != 9) {
			pthread_mutex_lock(&mutexTab);
			set_nbcle(get_nbcle() + 1);
			
			while(get_tab_nonblock(pos) != VIDE || is_statue_position(pos)) {
				pos.L = (rand() % 12) + 1;
				pos.C = (rand() % 12) + 1;
				
				printf("[+] KeyMaster: Spawning Key #%d: %d,%d\n", get_nbcle(), pos.L, pos.C);
			}
			
			set_tab_nonblock(pos, CLE);
			DessineSprite(pos, CLE);
			
			set_listetache(pos, get_indiceinsertion());
			set_indiceinsertion(get_indiceinsertion() + 1);
			
			pthread_cond_signal(&condListeTaches);
			
			pthread_mutex_unlock(&mutexTab);
		}
		
		pthread_mutex_lock(&mutexNbCles);
		
		pthread_cond_wait(&condNbCles, &mutexNbCles);
		printf("[+] KeyMaster: New key request\n");
		
		pthread_mutex_unlock(&mutexNbCles);
	}
	
	return dummy;
}
