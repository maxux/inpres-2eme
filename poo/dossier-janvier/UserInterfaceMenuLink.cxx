#include <iostream>
#include <termios.h>
#include <string>
#include <fstream>

#include "Collectionneur.hxx"

#include "UserInterface.hxx"
#include "UserInterfaceMenu.hxx"
#include "UserInterfaceMenuLink.hxx"

int admin_change_passwd(void* none) {
	fstream passfile(FILENAME_LOGIN, fstream::in), tempfile;
	string newpass, line, line_keep, read_user;
	char *tok;
	UI ui;
	
	none = NULL;
	
	cout << "Nouveau mot de passe: ";
	
	ui.disable_echo();
	cin >> newpass;
	cout << endl;
	
	ui.restore_echo();

	if(!passfile.is_open()) {
		passfile.open(FILENAME_LOGIN, ios::out);
		
		if(passfile.is_open()) {
			passfile << "admin:" << newpass << endl;
			
		} else cout << "Erreur lors de la creation du fichier " << FILENAME_LOGIN << endl;
		
		passfile.close();
		
		return 0;
	}
	
	tempfile.open(FILENAME_LOGIN_TEMP, fstream::out);
	
	while(passfile >> line) {
		line_keep = line.c_str();
		
		tok = strtok((char*) line.c_str(), ":");
		read_user = tok;
		
		/* Checking Login */
		if(read_user != "admin") {
			tempfile << line_keep << endl;
		}
	}
	
	passfile.close();
	
	/* Inserting */
	tempfile << "admin:" << newpass << endl;
	tempfile.close();
	
	/* Replacing File */
	remove(FILENAME_LOGIN);
	rename(FILENAME_LOGIN_TEMP, FILENAME_LOGIN);
	
	return 0;
}

int admin_display_userlist(void *none) {
	fstream passfile(FILENAME_LOGIN);
	string line;
	
	none = NULL;
	
	while(passfile >> line)
		cout << line << endl;
	
	passfile.close();
	
	return 0;
}

int admin_display_userinfo(void *none) {
	UI temp;
	string user;
	
	none = NULL;
	
	cout << "Login: ";
	cin >> user;
	
	if(temp.check_login_exists(user)) {
		cout << "Level: " << temp.user_level(user) << endl;
		
	} else cout << "Utilisateur inconnu" << endl;
	
	return 0;
}

string admin_add_user() {
	UI ui;
	fstream passfile(FILENAME_LOGIN, fstream::out | fstream::app), newuser;
	string user, pass;
	
	do {
		cout << "Login: ";
		cin >> user;
		
	} while((ui.check_login_exists(user) || user == "admin") && cout << "Cet utilisateur existe déjà" << endl);
	
	ui.disable_echo();
	cout << "Password: ";
	cin >> pass;
	
	ui.restore_echo();
	cout << endl;
	
	passfile << user << ":" << pass << endl;
	passfile.close();
	
	return user;
}

int admin_add_collect(void *none) {
	Collectionneur addme;
	fstream newuser;
	string login, filename;
	
	none = NULL;
	
	/* Modify userlist */
	login = admin_add_user();
	
	cin >> addme;
	
	/* Creating user file */
	filename = "data/" + login + ".co";
	newuser.open(filename.c_str(), fstream::out);
	
	/* Saving data */
	newuser << addme;
	
	newuser.close();
	
	return 0;
}

int admin_add_designer(void *none) {
	UI ui;
	fstream passfile(FILENAME_LOGIN, fstream::out | fstream::app);
	string login;
	
	none = NULL;
	
	login = admin_add_user();
	
	return 0;
}
