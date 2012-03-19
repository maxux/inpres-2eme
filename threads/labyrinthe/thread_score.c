#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "astar.h"
#include "thread_score.h"
#include "interface.h"

pthread_t tScore;
pthread_mutex_t mutexScore, mutexScoreFlags;
pthread_cond_t condScore;
score_flags_t scoreFlags;

void * threadScore(void *dummy) {
	int score = 0;
	
	while(1) {
		pthread_mutex_lock(&mutexScore);
		pthread_cond_wait(&condScore, &mutexScore);
		
		printf("[+] Score: Updating score...\n");
		
		switch(scoreFlags) {
			case GOT_NEW_KEY:
			case PUT_KEY:
				score += 5;
			break;
			
			case GOT_TRESOR:
				score += 20;
			break;
			
			case GOT_PERLE:
				score += 40;
			break;
			
			case EOLEVEL:
				score += 50;
			break;
			
			case GOT_OLD_KEY:
				score -= 7;
			break;
		}
		
		if(score < 0)
			score = 0;
		
		printf("[+] Score: new score: %d\n", score);
		score_update(score);
		
		pthread_mutex_unlock(&mutexScoreFlags);
		pthread_mutex_unlock(&mutexScore);
	}
	
	return dummy;
}

void score_update(int score) {
	int pix[10] = {ZERO, UN, DEUX, TROIS, QUATRE, CINQ, SIX, SEPT, HUIT, NEUF};
	position_t pos[4] = {{2, 15}, {2, 16}, {2, 17}, {2, 18}};
	int index[4] = {1000, 100, 10, 1};
	int i, value;
	
	for(i = 0; i < 4; i++) {
		value = score / index[i];
		
		EffaceCarre(pos[i]);
		DessineSprite(pos[i], pix[value]);
		
		score -= value * index[i];
	}
}
