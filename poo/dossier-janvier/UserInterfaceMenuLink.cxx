#include <iostream>
#include <termios.h>
#include <string>
#include <fstream>

#include "UserInterface.hxx"
#include "UserInterfaceMenu.hxx"
#include "UserInterfaceMenuLink.hxx"

int admin_change_passwd(void* none) {
	fstream passfile(FILENAME_LOGIN);
	string newpass, line, read_user;
	char *tok;
	UI ui;
	
	none = NULL;
	
	/*
	 *  FIXME
	 */
	
	cout << "Nouveau mot de passe: ";
	
	ui.disable_echo();
	cin >> newpass;
	
	ui.restore_echo();

	if(!passfile) {
		/* CREATE FILE */
		return 0;
	}
	
	while(passfile >> line) {
		cout << line << endl;
		
		tok = strtok((char*) line.c_str(), ":");
		read_user = tok;
		
		/* Checking Login */
		if(read_user != "admin")
			continue;
		
		passfile << "admin:" << newpass << endl;
		return 0;
	}
	
	/* Not found on file. Adding it */
	passfile << "admin:" << newpass << endl;
	
	return 0;
}

int admin_display_userlist(void *none) {
	fstream passfile(FILENAME_LOGIN);
	string line;
	
	none = NULL;
	
	while(passfile >> line)
		cout << line << endl;
	
	return 0;
}
