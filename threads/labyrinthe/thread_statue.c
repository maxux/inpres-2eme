#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "astar.h"
#include "thread_statue.h"
#include "interface.h"

pthread_t tStatues[9];
pthread_mutex_t mutexListeTaches, mutexStatueReady;
pthread_cond_t condListeTaches, condStatueReady;

position_t ListeTaches[10];

void * threadStatue(void *_id) {
	S_STATUE *id = (S_STATUE*) _id;
	int __porteCle = 0, __cache = VIDE;
	int valeursAutorisees[] = {VIDE, CLE, HERO, SERRURE, TRESOR, PERLE};
	position_t dest;
	position_t *chemin = NULL;
	int nbCases, i, statuePix;
	struct timespec ts;
	
	printf("[+] Statue #%d ID: %d\n", id->indice, (int) pthread_self());
	printf("[+] Statue #%d: Spawning at (%d,%d)\n", id->indice, id->depart.L, id->depart.C);
	
	/* Writing Hero's position */
	set_tab(id->depart, STATUE_FACE_SANS);
	
	/* Drawing */
	DessineSprite(id->depart, STATUE_FACE_SANS);
	
	/* Init Sleep Time */
	ts.tv_sec = (int) delay;
	ts.tv_nsec = ((((delay - (int) delay)) * 1.5) * 1000000000) / debug_speed;
	
	pthread_cond_signal(&condStatueReady);
	
	while(1) {		
		printf("[ ] Statue #%d: waiting key...\n", id->indice);
		
		pthread_mutex_lock(&mutexListeTaches);
		pthread_cond_wait(&condListeTaches, &mutexListeTaches);
		
		/* Unlock: using get/set after */
		pthread_mutex_unlock(&mutexListeTaches);
		
		/* Accepting new destination */
		pthread_mutex_lock(&mutexListeTaches);
		pthread_mutex_lock(&mutexIndiceExtraction);
		
		dest = get_listetache_nonblock(indiceExtraction);
		set_indiceextraction_nonblock(get_indiceextraction_nonblock() + 1);
		
		pthread_mutex_unlock(&mutexListeTaches);
		pthread_mutex_unlock(&mutexIndiceExtraction);
		
		calcul:
		printf("[+] Statue #%d: new position request: %d,%d\n", id->indice, dest.L, dest.C);
		
		/* Path finding */
		pthread_mutex_lock(&mutexTab);
		nbCases = RechercheChemin(&tab[0][0], NB_LIGNES, NB_COLONNES, valeursAutorisees, sizeof(valeursAutorisees) / sizeof(int), id->position, dest, &chemin);
		
		if(!nbCases || !chemin) {
			if(compare_position(id->position, dest))
				continue;
				
			printf("[-] Statue #%d: Cannot reach destination (%d,%d)... retrying...\n", id->indice, dest.L, dest.C);
			
			pthread_mutex_unlock(&mutexTab);
			nanosleep(&ts, NULL);
			
			goto calcul;
		}
		
		/* Moving */
		i = 0;
		while(id->position.C != dest.C || id->position.L != dest.L) {
			/* Checking key presence */
			if(!__porteCle && get_tab_nonblock(dest) != CLE && get_tab_nonblock(dest) != STATUE && get_tab_nonblock(dest) != HERO) {
				printf("[-] Statue #%d: Key is no longer available (%d,%d = %d). Back at home.\n", id->indice, dest.L, dest.C, get_tab_nonblock(dest));
				
				pthread_mutex_unlock(&mutexTab);
				
				nanosleep(&ts, NULL);
				dest = id->depart;
				
				goto calcul;
			}
			
			/* Checking Collisions */
			if(get_tab_nonblock(chemin[i]) == STATUE) {
				printf("[x] Statue #%d: Collision (%d,%d -> %d,%d = %d), recalculing...\n", id->indice, id->position.L, id->position.C, chemin[i].L, chemin[i].C, get_tab_nonblock(chemin[i]));
				free(chemin);
				
				pthread_mutex_unlock(&mutexTab);
				nanosleep(&ts, NULL);
				
				goto calcul;
			}
			
			if(get_tab_nonblock(chemin[i]) == HERO) {
				printf("[+] Statue #%d: I killed Hero ! FUCK YEAH\n", id->indice);
				pthread_kill(tHero, SIGUSR2);
			}
			
			/* Updating cache and current position */
			set_tab_nonblock(id->position, __cache);
			
			/* Updating UI */
			EffaceCarre(id->position);
			if(__cache != VIDE)
				DessineSprite(id->position, __cache);
			
			/* Saving next case */
			__cache = get_tab_nonblock(chemin[i]);
			EffaceCarre(chemin[i]);
			
			/* Updating next case */
			set_tab_nonblock(chemin[i], STATUE);
			
			// Unlocking
			pthread_mutex_unlock(&mutexTab);
			
			/* Writing UI */
			statuePix = (debug_speed > 1) ? ZERO + id->indice : statue_getpix(id->position, *(chemin + i), __porteCle);
			id->position = chemin[i];
			
			printf("[ ] Statue #%d: position: %d,%d (destination: %d,%d)\n", id->indice, id->position.L, id->position.C, dest.L, dest.C);
			DessineSprite(chemin[i], statuePix);
			
			nanosleep(&ts, NULL);
			i++;
		}
		
		free(chemin);
		
		printf("[+] Statue #%d: destination reached\n", id->indice);
		
		/* We are back to home */
		if(compare_position(id->position, id->depart)) {
			printf("[+] Statue #%d: I got the key, mouwhahaha\n", id->indice);
			
			if(__porteCle)
				set_nbcle(get_nbcle() - 1);
			
			/* Tell KeyMaster to pop new key */	
			pthread_cond_signal(&condNbCles);
			
			/* Clearing key */
			__porteCle = 0;			
			continue;
		}
		
		/* We got the key */
		if(__cache == CLE) {
			__porteCle = 1;
			__cache    = VIDE;
			
			/* Updating game map */
			set_tab(dest, VIDE);
			EffaceCarre(dest);
			DessineSprite(dest, (debug_speed > 1) ? TRESOR : VIDE);
			
			printf("[+] Statue #%d: Go back at home !\n", id->indice);
			
			/* New destination: home */
			dest = id->depart;
			
			goto calcul;
		}
		
		printf("[-] Statue #%d: WTF ?\n", id->indice);
	}
	
	return id;
}

int statue_getpix(position_t prev, position_t new, int __porteCle) {
	/* printf("Previous: %d,%d\nNew: %d, %d\n", prev->L, prev->C, new->L, new->C); */
	
	/* Left or Right */
	if(prev.L == new.L) {
		if(prev.C < new.C)
			return (__porteCle) ? STATUE_DROITE_AVEC : STATUE_DROITE_SANS;
			
		else return (__porteCle) ? STATUE_GAUCHE_AVEC : STATUE_GAUCHE_SANS;
		
	/* Top or Bottom */
	} else {
		if(prev.L < new.L)
			return (__porteCle) ? STATUE_FACE_AVEC : STATUE_FACE_SANS;
			
		else return STATUE_DOS;
	}
	
	return STATUE_FACE_SANS;
}
