#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <sys/ipc.h>
#include <sys/msg.h>

#include "FenetrePlanVille.hxx"
#include "ipc_messages.hxx"
#include "metro_protocol.hxx"
#include "debug.hxx"

extern global_t sys;

int send_message(metro_protocol request, void *data) {
	message_t message;	/* Message Queue */
	
	message.lType   = 1L;
	message.request = request;
	message.pid     = getpid();
	strcpy(message.text, (char*) data);
	
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
