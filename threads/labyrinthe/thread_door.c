#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "thread_door.h"
#include "interface.h"

pthread_t tPorte;

void * threadPorte(void *dummy) {
	unsigned int i;
	char level_full;
	
	printf("[+] Porte ID: %d\n", (int) pthread_self());

	while(1) {	
		/* Flag to 1 */
		level_full = 1;
		
		for(i = 0; i < SERRURES_COUNT; i++) {
			if(get_tab(positionSerrures[i]) != CLE) {
				/* Clearing flag */
				level_full = 0;
				break;
			}
		}
		
		/* Checking flag */
		if(level_full) {
			printf("[+] ___ LEVEL COMPLETED ___\n");
			for(i = 0; i < SERRURES_COUNT; i++) {
				/* Remove Keys */
				if(!compare_position(get_position_hero(), positionSerrures[i])) {
					set_tab(positionSerrures[i], SERRURE);
					EffaceCarre(positionSerrures[i]);
					DessineSprite(positionSerrures[i], SERRURE);
					
				} else cache = SERRURE;
			}
			
			/* Remove Door */
			set_tab(position_porte, VIDE);
			EffaceCarre(position_porte);
		}
		
		/* Waiting next time */
		alarm(3);
		sleep(3); //
	}
	
	return dummy;
}
