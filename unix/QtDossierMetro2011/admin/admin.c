#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>

#include "metro_protocol.h"
#include "ipc_messages.h"
#include "interface.h"
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

void sig_handler(int signum) {		
	switch(signum) {
		/* SIGPWR: Ping */
		case SIGPWR:
			send_message(ACK_PONG, (void*) "Pong !");
		break;
		
		/* SIGUSR1: New ads */
		case SIGUSR1:
			// 
		break;
		
		case SIGUSR2:
			// 
		break;
		
		case SIGINT:
			// 
		break;
	}
}

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = 0;
	/* sig.sa_sigaction = NULL; */
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}

int admin_message(void) {
	char msg[256];
	message_t message;
	
	printf("Message: ");
	while(fgets(msg, sizeof(msg), stdin) == NULL);
	
	send_message(QRY_ADMIN_MESSAGE, (void*) msg);
	read_message(&message);
	
	if(message.request != ACK_ADMIN_MESSAGE) {
		debugc("ERR: Invalid response (0x%x).\n", message.request);
		
	} else debug("ADM: Message sent !\n");
	
	return 0;
}

int shutdown_control(void) {
	message_t message;
	
	printf("Sending...\n");
	send_message(QRY_SHUTDOWN, (void*) "Fuck");
	read_message(&message);
	
	if(message.request != ACK_SHUTDOWN) {
		debugc("ERR: Negative ack (0x%x).\n", message.request);
		
	} else exit(0);
	
	return 0;
}

int main(void) {
	int mkey_id;		/* Message Queue ID */
	menu_t *menu;		/* Main Menu */
	
	/* Setting global variables */
	sys.mkey_id = &mkey_id;
	
	if((mkey_id = msgget(MESSAGE_KEY_ID, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	if(!request_login())
		return 1;
	
	/* Intercept SIGPWR: Ping Query */
	signal_intercept(SIGPWR, sig_handler);
	
	menu = menu_create("Administration Dossier Unix 2011 - Menu Principal", 9);
	menu_append(menu, "Message administrateur", 1, admin_message, NULL);
	menu_append(menu, "Shutdown Control", 8, shutdown_control, NULL);
	menu_append(menu, "Quitter l'administration", 9, NULL, NULL);
	
	/* Menu Process */
	while(menu_process(menu));
	menu_free(menu);

	
	send_message(QRY_LOGOUT, (void*) "I leave thx");
	
	return 0;
}


