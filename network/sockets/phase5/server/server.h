#ifndef __SERVER_H
	#define __SERVER_H

	#define VERSION		1.0
	
	
	#define DEFAULT_ADDRESS		"127.0.0.1"
	#define DEFAULT_PORT		2000
	
	int signal_intercept(int signal, void (*function)(int));
	void sighandler(int signal);
#endif
