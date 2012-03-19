#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "astar.h"
#include "sdl_grind.h"
#include "thread_hero.h"
#include "interface.h"

pthread_t tHero;
pthread_mutex_t mutexHero, mutexHeroPix;

int porteCle = 0;
int heroPix  = 0;

void * threadHero(void *dummy) {
	int valeursAutorisees[] = {VIDE, CLE, SERRURE};
	position_t *chemin = NULL;
	int nbCases, i, j, killStatue;
	struct timespec ts;
	S_PERSONNAGE *me;
	
	/* Linking Specific Data */
	me = (S_PERSONNAGE*) malloc(sizeof(S_PERSONNAGE));
	
	me->whoami   = HERO;
	me->cache    = VIDE;
	me->porteCle = 0;
	pthread_setspecific(spec_key, me);
	
	printf("[+] Hero ID: %d\n", (int) pthread_self());
	
	/* Init Position */
	pthread_mutex_lock(&mutexDestination);
	pthread_mutex_lock(&mutexPosition);
	
	position_hero.C    = 16;
	position_hero.L    = (rand() % 2) ? 5 : 9;
	
	destination_hero.L = position_hero.L;
	destination_hero.C = position_hero.C;
	
	pthread_mutex_unlock(&mutexPosition);
	pthread_mutex_unlock(&mutexDestination);
	
	printf("[+] Hero: Spawning at (%d,%d)\n", position_hero.L, position_hero.C);
	
	/* Writing Hero's position */
	set_tab(get_position_hero(), HERO);
	
	/* Drawing */
	DessineSprite(get_position_hero(), HERO_FACE_SANS);
	
	/* Init Sleep Time */
	ts.tv_sec = (int) delay;
	
	ts.tv_nsec = ((delay - (int) delay) * 1000000000) / debug_speed;
	// ts.tv_nsec = ((delay - (int) delay) * 1000000000) / 60;
	
	while(1) {
		printf("[ ] Hero: waiting event...\n");
		
		pthread_mutex_lock(&mutexDestination);
		
		if(!(heroFlags & NEW_DEST))
			pthread_cond_wait(&condDestination, &mutexDestination);
		
		/* Unlock: using get/set after */
		pthread_mutex_unlock(&mutexDestination);
		
		/* Accepting new destination */
		heroFlags &= ~NEW_DEST;
		printf("[+] Hero: new position request: %d,%d\n", destination_hero.L, destination_hero.C);
		
		/* Checking destination */
		switch(get_tab(get_destination_hero())) {
			case VIDE:
			case SERRURE:
			case CLE:
			case PERLE:
			case TRESOR:
				break;
			
			default:
				printf("[-] Hero: Case denied\n");
				continue;
		}
		
		/* Path finding */
		nbCases = RechercheChemin(&tab[0][0], NB_LIGNES, NB_COLONNES, valeursAutorisees, sizeof(valeursAutorisees) / sizeof(int), get_position_hero(), get_destination_hero(), &chemin);
		
		if(!nbCases || !chemin) {
			printf("[-] Hero: Cannot reach destination\n");
			continue;
		}
		
		// pthread_mutex_lock(&mutexDestination);
		
		/* Moving */
		i = 0;
		while(position_hero.C != destination_hero.C || position_hero.L != destination_hero.L) {
			if(heroFlags & NEW_DEST) {
				printf("[ ] Hero: New destination called. Cancelling this one...\n");
				break;
			}
			
			/* Updating cache and current position */
			set_tab(get_position_hero(), me->cache);
			
			/* Updating UI */
			EffaceCarre(get_position_hero());
			if(me->cache != VIDE)
				DessineSprite(get_position_hero(), me->cache);
			
			/* Saving next case */
			me->cache = get_tab(chemin[i]);
			EffaceCarre(chemin[i]);
			
			/* Checking if got a key */
			if(me->cache == CLE && !me->porteCle) {
				pthread_mutex_lock(&mutexScoreFlags);
				scoreFlags = GOT_NEW_KEY;
				
				me->cache    = VIDE;
				me->porteCle = 1;
				
				/* Checking old SERRURE mapping */
				for(j = 0; j < SERRURES_COUNT; j++) {
					if(compare_position(get_position_hero(), positionSerrures[j])) {
						printf("[ ] Hero: previous was SERRURE, restoring...\n");
						
						me->cache  = SERRURE;
						scoreFlags = GOT_OLD_KEY;
						break;
					}
				}
				
				pthread_cond_signal(&condScore);
			}
			
			if(me->cache == TRESOR || me->cache == PERLE) {
				pthread_mutex_lock(&mutexScoreFlags);
				
				if(me->cache == PERLE) {
					scoreFlags = GOT_PERLE;
					killStatue = rand() % nbStatue;
					printf("[+] Hero: Killing Statue #%d\n", killStatue);
					pthread_kill(tStatues[killStatue], SIGUSR2);
					
				} else scoreFlags = GOT_TRESOR;
				
				me->cache = VIDE;
				
				pthread_cond_signal(&condScore);
			}
			
			/* Updating next case */
			set_tab(chemin[i], HERO);
			
			/* Writing UI */
			set_heropix(hero_getpix(get_position_hero(), *(chemin + i), me->porteCle));
			set_position_hero(chemin[i]);
			
			printf("[ ] Hero: position: %d,%d (destination: %d,%d)\n", position_hero.L, position_hero.C, destination_hero.L, destination_hero.C);
			DessineSprite(chemin[i], get_heropix());
			
			nanosleep(&ts, NULL);
			i++;
		}
		
		printf("[+] Hero: destination reached\n");
		
		/* Cleaning */
		if(chemin) {
			free(chemin);
			chemin = NULL;
		}
		
		/* Checking end of Level */
		pthread_mutex_lock(&mutexPosition);
		
		if(compare_position(position_hero, position_porte)) {
			pthread_mutex_lock(&mutexScoreFlags);
			scoreFlags = EOLEVEL;
			
			pthread_cond_signal(&condScore);
			
			EffaceCarre(position_hero);
			DessineSprite(position_hero, PORTE);
			
			position_hero.C    = 16;
			position_hero.L    = (rand() % 2) ? 5 : 9;
			
			pthread_cond_signal(&condNiveau);
			
			DessineSprite(get_position_hero_nonblock(), HERO_FACE_SANS);	
		}
		
		pthread_mutex_unlock(&mutexPosition);
	}
	
	return dummy;
}

int hero_getpix(position_t prev, position_t new, int porteCle) {
	/* printf("Previous: %d,%d\nNew: %d, %d\n", prev->L, prev->C, new->L, new->C); */
	
	/* Left or Right */
	if(prev.L == new.L) {
		if(prev.C < new.C)
			return (porteCle) ? HERO_DROITE_AVEC : HERO_DROITE_SANS;
			
		else return (porteCle) ? HERO_GAUCHE_AVEC : HERO_GAUCHE_SANS;
		
	/* Top or Bottom */
	} else {
		if(prev.L < new.L)
			return (porteCle) ? HERO_FACE_AVEC : HERO_FACE_SANS;
			
		else return HERO_DOS;
	}
	
	return HERO_FACE_SANS;
}
