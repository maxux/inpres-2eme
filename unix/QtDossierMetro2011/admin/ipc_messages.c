#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "admin.h"
#include "metro_protocol.h"
#include "ipc_messages.h"
#include "debug.h"

extern global_t sys;

int send_message(metro_protocol request, void *data, size_t bsize) {
	message_t message;	/* Message Queue */
	
	message.lType   = 1L;
	message.request = request;
	message.pid     = getpid();
	
	/* Checking if it's a string or a binary data */
	if(bsize > 0) {
		/* Avoid memory over flow */
		if(bsize > sizeof(message.text))
			bsize = sizeof(message.text);
		
		/* Copy binary data */
		memcpy(message.text, data, bsize);
		
	} else strcpy(message.text, (char*) data);
		
	
	if(msgsnd(*(sys.mkey_id), &message, sizeof(message) - sizeof(long), 0)) {
		perror("msgsnd");
		return 0;
	}
	
	return 1;
}


int read_message(message_t *destination) {
	int rc;
	
	if((rc = msgrcv(*(sys.mkey_id), destination, sizeof(message_t), getpid(), 0)) == -1) {
		perror("msgrcv:");
		exit(1);
	}
	
	return rc;
}
