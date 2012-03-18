#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "labyrinthe.h"
#include "sdl_grind.h"
#include "astar.h"
#include "thread_bonus.h"
#include "interface.h"

pthread_t tBonus;

void * threadBonus(void *dummy) {
	int generated;
	struct timespec ts;
	position_t pos = {0, 0};
	
	printf("[+] Bonus: Starting...\n");
	
	while(1) {
		pthread_mutex_lock(&mutexTab);
		
		if(get_tab_nonblock(pos) == VIDE || get_tab_nonblock(pos) == STATUE) {
			printf("[-] Bonus: Hero seems to got the bonus... waiting next iteration...\n");
			pos.L = 0;
			goto wait;
		}
		
		if(pos.L != 0 && get_tab_nonblock(pos) != HERO) {
			EffaceCarre(pos);
			set_tab_nonblock(pos, VIDE);
		}
		
		generated = (rand() % 2) ? TRESOR : PERLE;
		printf("[ ] Bonus: Spawning %d generated, searching free space...\n", generated);
		
		pos.L = 0;
		
		while(get_tab_nonblock(pos) != VIDE || is_statue_position(pos)) {
			pos.L = (rand() % 12) + 1;
			pos.C = (rand() % 12) + 1;
			
			printf("[ ] Bonus: Spawning %d at %d,%d\n", generated, pos.L, pos.C);
		}
		
		set_tab_nonblock(pos, generated);
		
		EffaceCarre(pos);
		DessineSprite(pos, generated);
		
		wait:
		pthread_mutex_unlock(&mutexTab);
		
		ts.tv_sec  = (rand() % 10) + 5;
		ts.tv_nsec = rand();
		
		printf("[+] Bonus: Waiting %ld.%ld seconds...\n", (long) ts.tv_sec, (long) ts.tv_nsec);
		nanosleep(&ts, NULL);
	}
	
	return dummy;
}
