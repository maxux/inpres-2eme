#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

int restore_console(tcflag_t *c_lflag) {
	struct termios tty_attr;
	
	if(tcgetattr(STDIN_FILENO, &tty_attr) < 0)
		return -1;
	
	/* Restoring */
	tty_attr.c_lflag = *c_lflag;

	if(tcsetattr(STDIN_FILENO, TCSANOW, &tty_attr) < 0)
		return -1;
	
	return 0;
}

int disable_echo(tcflag_t *c_lflag) {
	struct termios tty_attr;
	
	if(tcgetattr(STDIN_FILENO, &tty_attr) < 0)
		return -1;
	
	/* Saving state */
	*c_lflag = tty_attr.c_lflag;
	
	/* Changing */
	// tty_attr.c_lflag &= ~ICANON;
	tty_attr.c_lflag &= ~ECHO;
	
	if(tcsetattr(STDIN_FILENO, TCSANOW, &tty_attr) < 0)
		return -1;
	
	return 0;
}

void trim(char *data) {
	int len;
	
	len = strlen(data);
	
	if(data[len - 1] == '\n')
		data[len - 1] = '\0';
}
