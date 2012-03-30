#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "../common/transac.h"
#include "../common/physlib/physlib.h"
#include "server.h"
#include "transacserver.h"

#define SLEEP_TIMEOUT	18.0

void sighandler(int signal) {
	switch(signal) {
		case SIGUSR1:
			printf("[X] SIGUSR1 Intercepted. Sleeping for %.2f seconds...\n", SLEEP_TIMEOUT);
			usleep(SLEEP_TIMEOUT * 1000000);
			printf("[X] Restoring...\n");
		break;
	}
}

int main(int argc,char *argv[]) {
	int rc, sockfd;
	struct sockaddr_in psoo; /* o = origine */
	struct sockaddr_in psos; /* s = serveur */
	struct sockaddr_in psor; /* r = remote */
	FILE *fp;
	
	char *bind_address;
	int port;
	
	char remote_ip[16];
	int remote_ip_raw, remote_port;

	transac_t transaction;

	memset(&psoo,0,sizeof(struct sockaddr_in));
	memset(&psos,0,sizeof(struct sockaddr_in)); 
	memset(&psor,0,sizeof(struct sockaddr_in));

	printf("[+] Initializing Server (version %.1f - DANIEL Maxime)...\n", VERSION);
	if(argc == 1) {
		bind_address = DEFAULT_ADDRESS;
		port = DEFAULT_PORT;
		
	} else if(argc == 3) {
		bind_address = argv[1];
		port = atoi(argv[2]);
		
	} else {
		printf("%s [[address-bind] [port]]\n", argv[0]) ;
		exit(1);
	}
	
	/* Intercepting SIGUSR1 */
	signal_intercept(SIGUSR1, sighandler);

	/* Opening files */
	if(!(fp = ticket_init())) {
		fprintf(stderr, "[-] Cannot open data file\n");
		exit(EXIT_FAILURE);
	}

	sockfd = CreateSockets(&psoo, NULL, bind_address, port, NULL, 0);
	if(sockfd == -1)
		perror("CreateSockets");
	else
		printf("CreateSockets: %d\n", sockfd);

	while(1) {
		/* Waiting data */
		rc = ReceiveDatagram(sockfd, &transaction, sizeof(transaction), &psor);
		
		if(rc == -1) {
			perror("ReceiveDatagram");
			continue;
			
		} else fprintf(stderr, "[+] Read: %d bytes. Action: %u\n", rc, transaction.action);
		
		/* Extracting ip/port */
		remote_ip_raw = GetIP(&psor);
		remote_port   = GetPort(&psor);
		
		/* Copy ip */
		memset(remote_ip, '\0', sizeof(remote_ip));
		Ipv4ToS(remote_ip_raw, remote_ip);
	  
		printf("[+] Incomming connection: %s:%d\n", remote_ip, remote_port);
		
		transaction.ip   = remote_ip_raw;
		transaction.port = remote_port;
		
		/* Check double */
		if(transac_check_exists(fp, &transaction)) {
			printf("[-] Transaction already found\n");
			transaction.action = ERROR;
			SendDatagram(sockfd, &transaction, sizeof(transaction), &psor);
			continue;
		}
		
		switch(transaction.action) {
			case RESERVATION:				
				transaction = ticket_reserver(fp, transaction);

				/* Responding */
				rc = SendDatagram(sockfd, &transaction, sizeof(transaction), &psor);
				if(rc == -1)
					perror("SendDatagram:");
					
				else fprintf(stderr, "[+] Sent: %d bytes\n", rc);
			break;
			
			case BUY:
				transaction = ticket_payer(fp, transaction);

				/* Responding */
				rc = SendDatagram(sockfd, &transaction, sizeof(transaction), &psor);
				if(rc == -1)
					perror("SendDatagram:");
					
				else fprintf(stderr, "[+] Sent: %d bytes\n", rc);
			break;
			
			default:
				fprintf(stderr, "[-] Unknown protocol request\n");
		}
		
		printf("[x] DEBUG Table:\n");
		ticket_lister(fp);
	}
	
	return 0;
}


int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = 0;
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}
