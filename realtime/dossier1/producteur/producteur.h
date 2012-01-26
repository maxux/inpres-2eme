#ifndef __PROD_HEADER
	#define __PROD_HEADER

	int signal_intercept(int signal, void (*function)(int));
	void sig_handler(int signum);
#endif
