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
	int valeursAutorisees[] = {VIDE, CLE, HERO, SERRURE, TRESOR, PERLE};
	position_t dest;
	position_t *chemin = NULL;
	int nbCases, i, statuePix;
	struct timespec ts;
	S_PERSONNAGE *me;
	
	/* Pushing exit cleanup */
	pthread_cleanup_push(killStatue, _id);
	
	printf("[+] Statue #%d ID: %d\n", id->indice, (int) pthread_self());
	
	/* Linking Specific Data */
	me = (S_PERSONNAGE*) malloc(sizeof(S_PERSONNAGE));
	
	me->whoami   = STATUE;
	me->cache    = VIDE;
	me->porteCle = 0;
	pthread_setspecific(spec_key, me);


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
			if(!me->porteCle && get_tab_nonblock(dest) != CLE && get_tab_nonblock(dest) != STATUE && get_tab_nonblock(dest) != HERO) {
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
			set_tab_nonblock(id->position, me->cache);
			
			/* Updating UI */
			EffaceCarre(id->position);
			if(me->cache != VIDE)
				DessineSprite(id->position, me->cache);
			
			/* Saving next case */
			me->cache = get_tab_nonblock(chemin[i]);
			EffaceCarre(chemin[i]);
			
			/* Updating next case */
			set_tab_nonblock(chemin[i], STATUE);
			
			/* Writing UI */
			statuePix = (debug_speed > 1) ? ZERO + id->indice : statue_getpix(id->position, *(chemin + i), me->porteCle);
			id->position = chemin[i];
			
			printf("[ ] Statue #%d: position: %d,%d (destination: %d,%d)\n", id->indice, id->position.L, id->position.C, dest.L, dest.C);
			DessineSprite(chemin[i], statuePix);
			
			// Unlocking
			pthread_mutex_unlock(&mutexTab);
			
			nanosleep(&ts, NULL);
			i++;
		}
		
		free(chemin);
		
		printf("[+] Statue #%d: destination reached\n", id->indice);
		
		/* We are back to home */
		if(compare_position(id->position, id->depart)) {
			printf("[+] Statue #%d: I got the key, mouwhahaha\n", id->indice);
			
			if(me->porteCle)
				set_nbcle(get_nbcle() - 1);
			
			/* Tell KeyMaster to pop new key */	
			pthread_cond_signal(&condNbCles);
			
			/* Clearing key */
			me->porteCle = 0;			
			continue;
		}
		
		/* We got the key */
		if(me->cache == CLE) {
			me->porteCle = 1;
			me->cache    = VIDE;
			
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
	
	pthread_cleanup_pop(1);
	
	return id;
}

int statue_getpix(position_t prev, position_t new, int porteCle) {
	/* printf("Previous: %d,%d\nNew: %d, %d\n", prev->L, prev->C, new->L, new->C); */
	
	/* Left or Right */
	if(prev.L == new.L) {
		if(prev.C < new.C)
			return (porteCle) ? STATUE_DROITE_AVEC : STATUE_DROITE_SANS;
			
		else return (porteCle) ? STATUE_GAUCHE_AVEC : STATUE_GAUCHE_SANS;
		
	/* Top or Bottom */
	} else {
		if(prev.L < new.L)
			return (porteCle) ? STATUE_FACE_AVEC : STATUE_FACE_SANS;
			
		else return STATUE_DOS;
	}
	
	return STATUE_FACE_SANS;
}

void killStatue(void *arg) {
	S_STATUE *s = (S_STATUE*) arg;
	struct timespec ts;
	pthread_t save;
	
	pthread_mutex_lock(&mutexTab);
	
	if(get_tab_nonblock(s->position) == STATUE) {
		EffaceCarre(s->position);
		set_tab_nonblock(s->position, VIDE);
	}
	
	
	/* Waiting ... */
	ts.tv_sec  = 5;
	ts.tv_nsec = 0;
	
	nanosleep(&ts, NULL);
	
	save = tStatues[s->indice];
	if(pthread_create(&tStatues[s->indice], NULL, threadStatue, sid))
			diep("[-] pthread_create");
	
	
}
