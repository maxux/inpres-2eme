#ifndef __SOCKET_H
	#define __SOCKET_H
	
	int transaction_transmit(int sockfd, transac_t *transaction, size_t size, struct sockaddr_in *psos, struct sockaddr_in *psor);
	
	typedef struct sock_send_t {
		int sockfd;
		transac_t *transaction;
		size_t size;
		struct sockaddr_in *psor;
		pthread_mutex_t *mutex;
		pthread_cond_t *cond;
		
	} sock_send_t;
	
	ushort cksum(const unsigned char *buf, uint len);
	int SendDatagram_crc(int sockfd, transac_t *data, size_t size, struct sockaddr_in *sock);
	int ReceiveDatagram_crc(int sockfd, transac_t *data, size_t size, struct sockaddr_in *sock);
#endif
