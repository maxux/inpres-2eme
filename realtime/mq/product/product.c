#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sched.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "../common.h"

#define PACKET_PRIOR_MAX	3		// [0-2]
#define PACKET_DESTI_MAX	3		// [0-2]

void diep(char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

int main(void) {
	mqd_t mq;
	payload_t payload;
	struct timespec ts;
	int a;

	if((mq = mq_open(mq_names[DISPATCHER], O_NONBLOCK | O_WRONLY, 0777, NULL)) == -1)
		diep("[-] mq_open");

	srand(time(NULL));

	ts.tv_sec  = 0;
	ts.tv_nsec = PRODUCT_DELAY * 1000000;

	while(1) {
		// Wait 250 ms
		nanosleep(&ts, NULL);

		// Prepare a packet
		payload.destination = (char)(a = (rand() % PACKET_DESTI_MAX));
		payload.data        = rand();

		// Send a packet with a random priority
		if(mq_send(mq, (char*) &payload, sizeof(payload_t), (unsigned int)(rand() % PACKET_PRIOR_MAX)) == -1){
			if(errno == EAGAIN)
				printf("[-] dispatcher is full\n");
				
			else perror("[-] mq_send");
			
		} else printf("[+] Message (%d/%d) sent: %lu bytes\n", payload.destination, payload.data, sizeof(payload_t));
	}
	
	return 0;
}
