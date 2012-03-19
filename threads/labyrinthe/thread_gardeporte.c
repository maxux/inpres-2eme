#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "astar.h"
#include "thread_gardeporte.h"
#include "interface.h"

pthread_t tGardePorte;

void * threadGardePorte(void *dummy) {
	int valeursAutorisees[] = {VIDE, HERO};
	position_t dest, depart, position;
	position_t *chemin = NULL;
	int nbCases, i, gardePix = GARDE_FACE;
	struct timespec ts;
	
	position_t _depart = {5, 18}, _dest = {9, 18};
	
	depart  = _depart;	/* Session destination */
	dest    = _dest;	/* Session destination */
	
	position = _depart;
	
	printf("[+] Grunt: ID: %d\n", (int) pthread_self());
	printf("[+] Grunt: Spawning at (%d,%d)\n", depart.L, depart.C);
	
	/* Writing Hero's position */
	set_tab(depart, GARDE_FACE);
	
	/* Drawing */
	DessineSprite(depart, GARDE_FACE);
	
	/* Init Sleep Time */
	ts.tv_sec = (int) delay;
	ts.tv_nsec = ((((delay - (int) delay))) * 1000000000) / debug_speed;
	
	while(1) {	
		printf("[ ] Grunt: Starting move to %d,%d...\n", dest.L, dest.C);
		
		/* Path finding */
		calcul:
		pthread_mutex_lock(&mutexTab);
		nbCases = RechercheChemin(&tab[0][0], NB_LIGNES, NB_COLONNES, valeursAutorisees, sizeof(valeursAutorisees) / sizeof(int), depart, dest, &chemin);
		
		if(!nbCases || !chemin) {
			printf("[-] Grunt: Cannot reach destination (%d,%d)... retrying...\n", dest.L, dest.C);
			
			pthread_mutex_unlock(&mutexTab);
			nanosleep(&ts, NULL);
			
			goto calcul;
		}
		
		/* Moving */
		i = 0;
		while(position.C != dest.C || position.L != dest.L) {
			if(get_tab_nonblock(chemin[i]) == HERO) {
				printf("[+] Grunt: I killed Hero ! FUCK YEAH\n");
				EffaceCarre(chemin[i]);
				DessineSprite(chemin[i], gardePix);
				pthread_kill(tHero, SIGUSR2);
			}
			
			/* Updating cache and current position */
			set_tab_nonblock(position, VIDE);
			
			/* Updating UI */
			EffaceCarre(position);
			
			/* Updating next case */
			set_tab_nonblock(chemin[i], GARDE);
			
			// Unlocking
			pthread_mutex_unlock(&mutexTab);
			
			/* Writing UI */
			gardePix = garde_getpix(position, *(chemin + i));
			position = chemin[i];
			
			printf("[ ] Grunt: position: %d,%d (destination: %d,%d)\n", position.L, position.C, dest.L, dest.C);
			DessineSprite(chemin[i], gardePix);
			
			nanosleep(&ts, NULL);
			i++;
		}
		
		free(chemin);
		
		printf("[+] Grunt: destination reached\n");
		
		depart = position;
		
		/* We are back to home */
		if(compare_position(position, _dest)) {
			printf("[+] Grunt: Rollback to home...\n");
			dest   = _depart;
			
		} else {
			printf("[+] Grunt: Rounding...\n");
			dest   = _dest;
		}
	}
	
	return dummy;
}

int garde_getpix(position_t prev, position_t new) {
	/* printf("Previous: %d,%d\nNew: %d, %d\n", prev->L, prev->C, new->L, new->C); */
	
	/* Top or Bottom */
	if(prev.L < new.L)
		return GARDE_FACE;
		
	else return GARDE_DOS;
}
