#ifndef __IPC_MESSAGES_H
	#define __IPC_MESSAGES_H
	
	#define SHARED_MEMORY_ID	1342
	#define SHARED_MEMORY_SIZE	2048
	
	#define MESSAGE_MAX_SIZE	2048
	#define MESSAGE_KEY_ID		1342
	
	#include "metro_protocol.h"

	typedef struct message_t {
		long lType;
		pid_t pid;
		metro_protocol request;
		char text[MESSAGE_MAX_SIZE];
		
	} message_t;
	
	int send_message(metro_protocol request, void *data, pid_t pid, size_t bsize);
#endif
