#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "debug.h"
#include "metro_protocol.h"
#include "ipc_messages.h"
#include "geremetro.h"
#include "metro.h"

extern ligne_legacy_t legacy_lignes[];
extern metro_nodes_t nodes[];
extern global_t sys;

void metro(metro_t *me) {
	int i;
	metro_position_t naow;
	
	debug("THR: (DBG) Thread: %p - Station: %d - Sens: %d\n", &(me->thread), me->line, me->sens);
	
	naow.id      = me->line;
	naow.current = 0;
	
	if(me->sens) {
		for(i = 0; i < METRO_MAX_POSITION; i++) {
			if(!legacy_lignes[me->line].position[i].N) {
				naow.current = i;
				break;
			}
		}
	}
	
	/* Add realism :D */
	if(RAND_MAX < 128000)
		usleep(rand() * 200);
	else
		usleep(rand() / 200);
	
	while(1) {
		debug("THR: (DBG) Line %d (Sens: %d): current: %d\n", me->line, me->sens, naow.current);
		
		naow.station = legacy_lignes[me->line].position[naow.current].N;
		if(!me->sens) {
			if(!legacy_lignes[me->line].position[naow.current + 1].N) {
				naow.next = legacy_lignes[me->line].position[naow.current - 1].N;
				
			} else naow.next = legacy_lignes[me->line].position[naow.current + 1].N;
			
		} else {
			if(naow.current == 0) {
				naow.next = legacy_lignes[me->line].position[naow.current + 1].N;
				
			} else naow.next = legacy_lignes[me->line].position[naow.current - 1].N;
		}
		
		
		if(write(sys.pipe_write, &naow, sizeof(naow)) != sizeof(naow))
			perror("write");
		
		/* Waiting next station */
		usleep(METRO_FAKE_TIME * 1000000);
		
		if(!me->sens) {
			naow.current++;
			
			if(!legacy_lignes[me->line].position[naow.current].N) {
				me->sens = 1;
				naow.current--;
			}
			
		} else {
			if(naow.current == 0) {
				naow.current++;
				me->sens = 0;
				
			} else naow.current--;
		}
	}
}
