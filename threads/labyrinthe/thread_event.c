#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "thread_event.h"
#include "interface.h"

pthread_t tEvent;

void * threadEvent(void *dummy) {
	EVENT_GRILLE_SDL event;
	int i;
	
	while(1) {
		printf("[ ] Event: waiting event...\n");
		event = ReadEvent();
		
		switch(event.type) {
			case CLIC_GAUCHE:
				if((debug_speed != 1) || (get_position_hero().C == event.colonne || get_position_hero().L == event.ligne)) {
					set_destination_hero(mk_position_t(event.ligne, event.colonne));
					
					heroFlags |= NEW_DEST;
					
					pthread_cond_signal(&condDestination);
				}
			break;
			
			case CLAVIER:
				switch(event.touche) {
					case 'd':
						pthread_kill(tHero, SIGUSR1);
					break;
					
					/* DEBUG KEYS */
					case 'p':
						printf("[*] Master: freezing...\n");
						pthread_mutex_lock(&mutexTab);
						
						for(i = 0; i < 9; i++)
							if(__s_statue_debug[i])
								printf("[D] Statue #%d: position %d,%d\n", i, __s_statue_debug[i]->position.L, __s_statue_debug[i]->position.C);
						
						debug_tab();
					break;
					
					case 'o':
						printf("[*] Master: unfreez...\n");
						pthread_mutex_unlock(&mutexTab);
					break;
				}
			break;
			
			case CROIX:
				pthread_cancel(tHero);
			break;
		}
	}
	
	return dummy;
}
