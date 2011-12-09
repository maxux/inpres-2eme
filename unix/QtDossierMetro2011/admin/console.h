#ifndef __CONSOLE_H
	#define __CONSOLE_H
	
	int restore_console(tcflag_t *c_lflag);
	int disable_echo(tcflag_t *c_lflag);
	
	void trim(char *data);
#endif
