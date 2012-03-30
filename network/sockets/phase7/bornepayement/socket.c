#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>
#include "payement.h"
#include "../common/physlib/physlib.h"
#include "../common/transac.h"
#include "socket.h"

void transaction_send(sock_send_t *send) {
	int dlen;
	
	if((dlen = ReceiveDatagram(send->sockfd, send->transaction, sizeof(transac_t), send->psor)) != -1) {
		fprintf(stderr, "[+] Read: %d\n", dlen);
		
		pthread_mutex_lock(send->mutex);
		pthread_cond_signal(send->cond);
		pthread_mutex_unlock(send->mutex);
		
	} else perror("ReceiveDatagram");
}

int transaction_transmit(int sockfd, transac_t *transaction, size_t size, struct sockaddr_in *psos, struct sockaddr_in *psor) {
	int dlen, i, rc;
	
	int arg;
	transac_t t;
	
	pthread_t retry;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct timespec ts;
	struct timeval tv;
	
	transac_t sender = *transaction;
	sock_send_t send;
	
	i = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	
	while(i < 4) {
		printf("[+] Sending %d bytes (%d/4)...\n", (int) size, i + 1);
		
		/* Sending data */
		if((dlen = SendDatagram(sockfd, &sender, sizeof(transac_t), psos)) != -1) {
			fprintf(stderr, "[+] Sent: %d bytes\n", dlen);
		
		} else perror("SendDatagram");

		/* Clearing transaction */
		memset(transaction, 0, sizeof(transac_t));

		/* Waiting response */
		printf("[+] Waiting response...\n");
		
		/* Preparing thread */
		send.sockfd      = sockfd;
		send.transaction = transaction;
		send.size	 = size;
		send.psor	 = psor;
		send.mutex	 = &mutex;
		send.cond	 = &cond;
		
		if(gettimeofday(&tv, NULL) == -1)
			perror("gettimeofday");
		
		ts.tv_sec  = tv.tv_sec + 10;
		ts.tv_nsec = tv.tv_usec * 1000;
		
		/* Threading */
		if(pthread_create(&retry, NULL, (void*) transaction_send, &send)) {
			perror("pthread");
			return 1;
		}
		
		/* Waiting thread */
		pthread_mutex_lock(&mutex);
		rc = pthread_cond_timedwait(&cond, &mutex, &ts);
		pthread_mutex_unlock(&mutex);
		
		if(rc == ETIMEDOUT) {
			/* Interrupting thread */
			pthread_cancel(retry);
			printf("[-] Time out: %d\n", ++i);
			
			continue;
			
		}
		
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
		
		arg = fcntl(sockfd, F_GETFL);
		
		
		
		/* Cleaning UDP Stack */
		tv.tv_sec = 1;
		tv.tv_usec = 0;
        
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof tv))
			perror("setsockopt");
			
		while(i-- > 0) {
			printf("[ ] Doublon supprimé\n");
			ReceiveDatagram(sockfd, &t, sizeof(transac_t), psor);
		}
		
		tv.tv_sec = 0;
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof tv))
			perror("setsockopt");
		
		return 0;	
	}
	
	fprintf(stderr, "[-] Timed out\n");
	return 1;
}
