#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "control.h"
#include "stack_client.h"

extern global_t sys;

void show_ads() {
	char *messages[10] = {"Hello", "World", "How", "Are"};
	int i;
	
	// printf("Starting ads loop...\n");
	
	while(1) {
		for(i = 0; i < 4; i++) {
			// printf("Sending ads...\n");
			
			strcpy(sys.shm, messages[i]);
			stack_sending_signal(*(sys.clients_head), SIGUSR1);
			usleep(3000000);
		}
	}
}
