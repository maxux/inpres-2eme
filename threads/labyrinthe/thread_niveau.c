#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "astar.h"
#include "thread_niveau.h"
#include "interface.h"

pthread_t tNiveau;
pthread_mutex_t mutexNiveau;
pthread_cond_t condNiveau;

int niveau = 1;

void * threadNiveau(void *dummy) {		
	while(1) {
		pthread_mutex_lock(&mutexNiveau);
		pthread_cond_wait(&condNiveau, &mutexNiveau);
		
		printf("[+] Level: New level request...\n");
		
		/* Increase 20% */
		delay *= 0.8;
		level_update(++niveau);
		
		set_nbcle(0);
		pthread_cond_signal(&condNbCles);
		
		pthread_mutex_unlock(&mutexNiveau);
	}
	
	return dummy;
}

void level_update(int level) {
	int pix[10] = {ZERO, UN, DEUX, TROIS, QUATRE, CINQ, SIX, SEPT, HUIT, NEUF};
	position_t pos[2] = {{13, 17}, {13, 18}};
	int index[2] = {10, 1};
	int i, value;
	
	for(i = 0; i < 2; i++) {
		value = level / index[i];
		
		EffaceCarre(pos[i]);
		DessineSprite(pos[i], pix[value]);
		
		level -= value * index[i];
	}
}
