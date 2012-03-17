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
	int thiscase;
	
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
					case 'D':
						if(!porteCle)
							continue;
						
						thiscase = get_tab(get_position_hero());
						
						if(cache != SERRURE)
							continue;
						
						pthread_kill(tHero, SIGUSR1);
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
