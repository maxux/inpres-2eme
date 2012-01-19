#ifndef __DOSSIER_JANVIER_H
	#define __DOSSIER_JANVIER_H
	
	void sig_handler(int signum);
	int signal_intercept(int signal, void (*function)(int));
#endif
