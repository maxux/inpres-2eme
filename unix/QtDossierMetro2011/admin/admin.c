#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "metro_protocol.h"
#include "ipc_messages.h"
#include "console.h"
#include "admin.h"
#include "debug.h"

global_t sys;

int check_login(char *passwd) {
	message_t message;
	
	send_message(QRY_ADMIN_LOGIN, (void*) passwd);
	read_message(&message);
	
	switch(message.request) {
		case ACK_ADMIN_LOGIN:
			return 1;
		break;
		
		case ERR_DENIED:
			return 0;
		break;
		
		default:
			debugc("ERR: Invalid opcode from server\n");
			return 0;
	}
}

int request_login() {
	tcflag_t c_lflag;
	
	char passwd[64];
	
	printf("Password: ");
	disable_echo(&c_lflag);
	
	while(fgets(passwd, sizeof(passwd), stdin) == NULL);
	trim(passwd);
	
	restore_console(&c_lflag);
	printf("\n");
	
	if(!check_login(passwd)) {
		usleep(2000000);		
		return 0;
		
	} else return 1;
}

int main(void) {
	int mkey_id;		/* Message Queue ID */
	
	/* Setting global variables */
	sys.mkey_id = &mkey_id;
	
	if((mkey_id = msgget(MESSAGE_KEY_ID, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	if(!request_login())
		return 1;
	
	return 0;
}


