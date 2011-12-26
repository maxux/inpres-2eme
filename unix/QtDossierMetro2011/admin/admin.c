#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <signal.h>

#include "admin.h"
#include "metro_protocol.h"
#include "ipc_messages.h"
#include "interface.h"
#include "console.h"
#include "debug.h"

global_t sys;

int check_login(char *passwd) {
	message_t message;
	
	send_message(QRY_ADMIN_LOGIN, (void*) passwd, 0);
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
			send_message(ACK_PONG, (void*) "Pong !", 0);
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

int download_data(station_t *stations) {
	message_t message;
	
	/* Downloading stations */
	debug("QRY: Stations List\n");
	send_message(QRY_PATHLIST, (void*) "Wantz Stations", 0);
	read_message(&message);
	
	if(message.request != ACK_PATHLIST) {
		debugc("ERR: Invalid answer from server (0x%x).\n", message.request);
		return 1;
		
	} else memcpy(stations, message.text, sizeof(station_t) * METRO_MAX_STATION);
	
	return 0;
}

int disable_station(void) {
	message_t message;
	station_t stations[METRO_MAX_STATION];
	char ask[64];
	int i = 1, id;
	
	if(download_data(stations))
		return 0;
	
	printf("Stations actives: \n");
	while(stations[i].L && stations[i].C) {
		if(stations[i].enabled)
			printf(" %d) %s\n", i, stations[i].station);
		
		i++;
	}
	
	printf("Désactiver (id): ");
	while(fgets(ask, sizeof(ask), stdin) == NULL);
	id = atoi(ask);
	
	debug("ADM: Disabling Station #%d...\n", id);
	
	send_message(QRY_DISABLE_STATION, (void*) &id, sizeof(id));
	read_message(&message);
	
	if(message.request != ACK_DISABLE_STATION) {
		debugc("ERR: Wrong response from control (0x%x)\n", message.request);
		
	} else debug("ADM: Station disabled !\n");
	
	return 0;
}

int enable_station(void) {
	message_t message;
	station_t stations[METRO_MAX_STATION];
	char ask[64];
	int i = 1, id;
	
	if(download_data(stations))
		return 0;
	
	printf("Stations innactives: \n");
	while(stations[i].L && stations[i].C) {
		if(!stations[i].enabled)
			printf(" %d) %s\n", i, stations[i].station);
		
		i++;
	}
	
	printf("Activer (id): ");
	while(fgets(ask, sizeof(ask), stdin) == NULL);
	id = atoi(ask);
	
	debug("ADM: Enabling Station #%d...\n", id);
	
	send_message(QRY_ENABLE_STATION, (void*) &id, sizeof(id));
	read_message(&message);
	
	if(message.request != ACK_ENABLE_STATION) {
		debugc("ERR: Wrong response from control (0x%x)\n", message.request);
		
	} else debug("ADM: Station enabled !\n");
	
	return 0;
}

int admin_message(void) {
	char msg[256];
	message_t message;
	
	printf("Message: ");
	while(fgets(msg, sizeof(msg), stdin) == NULL);
	
	send_message(QRY_ADMIN_MESSAGE, (void*) msg, 0);
	read_message(&message);
	
	if(message.request != ACK_ADMIN_MESSAGE) {
		debugc("ERR: Invalid response (0x%x).\n", message.request);
		
	} else debug("ADM: Message sent !\n");
	
	return 0;
}

int shutdown_control(void) {
	message_t message;
	
	printf("Sending...\n");
	send_message(QRY_SHUTDOWN, (void*) "Fuck", 0);
	read_message(&message);
	
	if(message.request != ACK_SHUTDOWN) {
		debugc("ERR: Negative ack (0x%x).\n", message.request);
		
	} else exit(0);
	
	return 0;
}

int insert_ads(void) {
	char buffer[32];
	message_t message;
	ads_struct_t ads;
	
	ads.id = 0;
	
	printf("Ads: ");
	while(fgets(ads.message, sizeof(ads.message), stdin) == NULL);
	ads.message[strlen(ads.message) - 1] = '\0';
	
	printf("Timeout: ");
	while(1) {
		if(fgets(buffer, sizeof(buffer), stdin) == NULL)
			continue;
			
		if((ads.timeout = atoi(buffer)) == 0)
			continue;
		
		break;
	}
	
	debug("Inserting <%s> (timeout: %d seconds)\n", ads.message, ads.timeout);
	
	send_message(QRY_ADS_INSERT, (void*) &ads, sizeof(ads));
	read_message(&message);
	
	if(message.request != ACK_ADS_INSERT) {
		debugc("ERR: Invalid response (0x%x).\n", message.request);
		
	} else debug("ADM: Ads inserted !\n");
	
	return 0;
}

int list_ads(void) {
	message_t message;
	ads_struct_t *ads;
	
	send_message(QRY_ADS_LIST, (void*) "Wantz ADS List Plz", 0);
	
	printf("Ads list: (Warning: only ads commited is displayed)\n");
	
	while(1) {
		read_message(&message);
		
		if(message.request != ACK_ADS_LIST) {
			debugc("ERR: Invalid response (0x%x).\n", message.request);
			break;	
		}
		
		ads = (ads_struct_t*) message.text;
		
		if(ads->timeout == 0)
			break;
		
		printf(" #%d (Timeout: %d) %s\n", ads->id, ads->timeout, ads->message);
	}
	
	return 0;
}

int delete_ads(void) {
	message_t message;
	char buffer[32];
	
	list_ads();
	
	printf("Delete ID: ");
	while(fgets(buffer, sizeof(buffer), stdin) == NULL);
	
	if(*buffer != '\n') {	
		send_message(QRY_ADS_DELETE, (void*) buffer, 0);
		read_message(&message);
		
		if(message.request != ACK_ADS_DELETE) {
			debugc("ERR: Invalid response (0x%x).\n", message.request);
			
		} else debug("ADM: Ads deleted !\n");
		
	} else printf(" -> Cancel\n");
	
	return 0;
}

int commit_ads(void) {
	message_t message;
	
	send_message(QRY_ADS_COMMIT, (void*) "Commit it plz", 0);
	read_message(&message);
	
	if(message.request == ACK_ADS_COMMIT) {
		debug("ACK: Commit okay\n");
		
	} else debugc("ERR: Commit failed: 0x%x\n", message.request);
	
	return 0;
}

int main(void) {
	int mkey_id;		/* Message Queue ID */
	menu_t *menu;		/* Main Menu */
	
	/* Init Logs */
	sys.log = fopen("../log/chemin.log", "w");
	if(!sys.log) {
		perror("fopen");
		return 2;
	}
	
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
	menu_append(menu, "Désactiver une station", 2, disable_station, NULL);
	menu_append(menu, "Activer une station", 3, enable_station, NULL);
	menu_append(menu, "Ajouter une pub", 4, insert_ads, NULL);
	menu_append(menu, "Supprimer une pub", 5, delete_ads, NULL);
	menu_append(menu, "Commit pub", 6, commit_ads, NULL);
	menu_append(menu, "Shutdown Control", 8, shutdown_control, NULL);
	menu_append(menu, "Quitter l'administration", 9, NULL, NULL);
	
	/* Menu Process */
	while(menu_process(menu));
	menu_free(menu);

	
	send_message(QRY_LOGOUT, (void*) "I leave thx", 0);
	
	fclose(sys.log);
	
	return 0;
}


