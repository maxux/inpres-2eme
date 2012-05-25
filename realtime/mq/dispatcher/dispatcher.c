#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <sched.h>
#include <string.h>

#include "../common.h"

typedef struct global_t {
	mqd_t mq_buffer;
	mqd_t mq_destination[MAX_DESTINATION];
	
	size_t mq_size_buffer;
	unsigned char dst_present[MAX_DESTINATION];
	
} global_t;

global_t global;

void buffering(payload_t *payload);

void diep(char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

/* override mq_send, with retry on EINTR */
int nmq_send(mqd_t mq, void *data, size_t data_size) {
	int z;
	
	while((z = mq_send(mq, (char*) data, data_size, 0) == -1)) {
		/* Interruption */
		if(errno == EINTR)
			continue;
		
		/* Nonblock and queue full */
		if(errno != EAGAIN)
			diep("[-] mq_send");
	}
		
	return z;
}

/* override mq_receive, with retry on EINTR */
ssize_t nmq_receive(mqd_t mq, void *data, size_t data_size) {
	size_t z;
	
	while((z = mq_receive(mq, (char*) data, data_size, NULL) == -1)) {
		/* Interruption */
		if(errno == EINTR)
			continue;
		
		/* Nonblock and queue full */
		if(errno != EAGAIN)
			diep("[-] mq_send");
	}
		
	return z;
}

int signal_intercept(int signal, void (*function)(int, siginfo_t*, void*)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = (void (*)(int)) function;
	sig.sa_flags	 = SA_SIGINFO;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

void sighandler(int sig, siginfo_t *info, void *context) {
	payload_t payload;
	size_t i, size, flushed;
	
	printf("[+] Signal: value %d\n", info->si_value.sival_int);
	context = NULL;
	
	if(sig == SIGUSR1) {
		printf("[+] Signal: new destination: %d\n", info->si_value.sival_int);
		
		if(info->si_value.sival_int > MAX_DESTINATION) {
			fprintf(stderr, "[-] Signal:  if out of range, ignoring...\n");
			return;
			
		} else global.dst_present[info->si_value.sival_int] = 1;
		
		/* clearning buffer */
		size    = global.mq_size_buffer;
		flushed = 0;
		
		for(i = 0; i < size; i++) {
			if(nmq_receive(global.mq_buffer, &payload, sizeof(payload_t)) == -1) {
				printf("[-] Buffer: waz empty, this shoulnd be happened\n");
				return;
			}
			
			if(payload.destination == info->si_value.sival_int) {
				if(mq_send(global.mq_destination[payload.destination], (char*) &payload, sizeof(payload_t), 0) == -1) {
					if(errno == EINTR) {
						break;
						
					} else if(errno == EAGAIN) {
						printf("[-] Flush: destination %d is full\n", payload.destination);
						break;
					
					} else diep("[-] mq_send");
				}
				
				flushed++;
				
			} else buffering(&payload);
		}
		
		printf("[+] Signal: destination ready: %d, flushed %lu messages\n", info->si_value.sival_int, flushed);
		
	} else if (sig == SIGUSR2){
		printf("[+] Signal: destination disconnected: %d\n", info->si_value.sival_int);
		
		if(info->si_value.sival_int > MAX_DESTINATION) {
			fprintf(stderr, "[-] Signal: destination if out of range, ignoring...\n");
			return;
			
		} else global.dst_present[info->si_value.sival_int] = 0;
	}
}

void buffering(payload_t *payload) {
	if(nmq_send(global.mq_buffer, payload, sizeof(payload_t)) == -1) {
		if(errno == EAGAIN)
			diep("[-] Buffering: buffer full");
		
		else diep("[-] mq_send");
	}
	
	global.mq_size_buffer++;
}

void init() {
	char buffer[64];
	FILE *fp;
	
	fp = fopen(PID_FILE, "w");
	if(!fp)
		diep("[-] fopen");
	
	sprintf(buffer, "%d", (int) getpid());
	if(fwrite(buffer, sizeof(char), strlen(buffer), fp) != strlen(buffer))
		perror("[-] fwrite");
	
	fclose(fp);
}

void init_queues(mqd_t *mq_block, mqd_t *mq_nonblock) {
	struct mq_attr attr;
	char buffer[255];
	int i;
	
	printf("[+] Init: Initializing queues...\n");
	
	attr.mq_flags   = O_NONBLOCK;
	attr.mq_msgsize = sizeof(payload_t);
	attr.mq_curmsgs = 0;
	
	/* Creating dispatcher queues */
	attr.mq_maxmsg  = mq_sizes[DISPATCHER];
	
	mq_unlink(mq_names[DISPATCHER]);
	if((*mq_block = mq_open(mq_names[DISPATCHER], O_RDONLY | O_CREAT | O_EXCL, 0777, &attr)) == -1)
		diep("[-] mq_open");
	
	if((*mq_nonblock = mq_open(mq_names[DISPATCHER], O_NONBLOCK | O_WRONLY, 0777, &attr)) == -1)
		diep("[-] mq_open");
	
	/* Creating buffer queue */
	attr.mq_maxmsg = mq_sizes[BUFFER];
	mq_unlink(mq_names[BUFFER]);
	
	if((global.mq_buffer = mq_open(mq_names[BUFFER], O_NONBLOCK | O_RDWR | O_CREAT | O_EXCL, 0777, &attr)) == -1)
		diep("[-] mq_open");
	
	/* Creating destinations queues */
	attr.mq_maxmsg = mq_sizes[DESTINATION];
	
	for(i = 0; i < MAX_DESTINATION; i++) {
		sprintf(buffer, "%s_%d", mq_names[DESTINATION], i);
		
		mq_unlink(buffer);		
		if((global.mq_destination[i] = mq_open(buffer, O_NONBLOCK | O_CREAT | O_EXCL | O_WRONLY, 0777, &attr)) == -1)
			diep("[-] mq_open");
	}
}

int main(void) {
	mqd_t mq_block, mq_nonblock;
	payload_t payload;
	
	printf("[+] Core: pid: %d\n", (int) getpid());

	signal_intercept(SIGUSR1, sighandler);
	signal_intercept(SIGUSR2, sighandler);
	
	/* Init PID file */
	init();
	
	/* Common settings */
	global.mq_size_buffer = 0;
	bzero(&global.dst_present, sizeof(char) * MAX_DESTINATION);
	
	/* Initializing */
	init_queues(&mq_block, &mq_nonblock);
	
	/* Working */
	while(1) {
		printf("[+] Core: Waiting message...\n");
		
		/* Waiting payload */
		if(nmq_receive(mq_block, &payload, sizeof(payload_t)) == -1)
			diep("[-] mq_receive");
		
		printf("[+] Core: new Message to %d, data: %d\n", payload.destination, payload.data);
		
		/* Checking destination */
		if(payload.destination > MAX_DESTINATION) {
			printf("[-] Core: destination %d is out of range, dropping\n", payload.destination);
			continue;
		}
		
		if(!global.dst_present[payload.destination]) {
			printf("[-] Core: destination %d is unreachable, buffering...\n", payload.destination);
			buffering(&payload);
			
			/* if(nmq_send(mq_nonblock, &payload, sizeof(payload_t)) == -1) {
				printf("[-] Dispatcher full, buffering...\n");
				buffering(&payload);
			} */
			
			continue;
		}
		
		/* Dispatching */
		if(nmq_send(global.mq_destination[payload.destination], &payload, sizeof(payload_t)) == -1) {
			printf("[-] Core: queue for destination %d is full, retrying...\n", payload.destination);
			
			if(nmq_send(mq_nonblock, &payload, sizeof(payload_t)) == -1) {
				printf("[-] Core: dispatcher full, buffering...\n");
				buffering(&payload);
				
				continue;
			}
		}
	}
	
	return 0;
}
