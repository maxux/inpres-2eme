#include <iostream>
#include <termios.h>
#include <string>
#include <fstream>

#include "UserInterface.hxx"
#include "UserInterfaceMenu.hxx"
#include "UserInterfaceMenuLink.hxx"

UI::UI() {
	_level = 0;
}

UI::~UI() {
	
}

int UI::login() {
	cout << "Login: ";
	cin >> _login;
	
	cout << "Password: ";
	
	disable_echo();	
	cin >> _passwd;
	
	restore_echo();
	cout << endl << endl;;
	
	return check_login();
}

int UI::check_login() {
	ifstream passfile(FILENAME_LOGIN);
	string line;
	string read_user, read_pass;
	char *tok;
	
	/* File doesn't exists. Fallback to admin */
	if(!passfile)
		return anti_flood((_login == "admin" && _passwd == ADMIN_DEFAULT_PASSWORD));	
	
	/* Reading each lines */
	while(passfile >> line) {
		tok = strtok((char*) line.c_str(), ":");
		read_user = tok;
		
		/* Checking Login */
		if(read_user != _login)
			continue;
		
		/* Compare password */
		tok = strtok(NULL, ":");
		read_pass = tok;
		
		return anti_flood((read_pass == _passwd));
	}
	
	/* Not found */
	return anti_flood(0);
}

inline int UI::anti_flood(int value) {
	if(!value)
		usleep(2000000);
	
	return value;
}


void UI::prepare() {
	/* Pre-Check Login Access */
	if(_login == "admin") {
		_level = USER_LEVEL_ADMIN;
		return;
	}
	
	
}

int dummy(void*) {
	cout << "DUMMY" << endl;
	return 0;
}

int UI::start_events() {
	UIMenu menu;
	
	switch(_level) {
		case USER_LEVEL_ADMIN:
			menu.create("Menu d'administration");
			
			menu.append("Afficher la liste des utilisateurs", '1', admin_display_userlist, NULL);
			menu.append("Afficher les infos d'un utilisateur", '2', dummy, NULL);
			menu.append("Créer un collectionneur", '3', NULL, NULL);
			menu.append("Créer un concepteur d'album", '4', NULL, NULL);
			menu.append("Changer le mot de passe administrateur", '5', admin_change_passwd, NULL);
			menu.append("Fermer la session", 'N', NULL, NULL, true);
			
			return menu.process();
		break;
		
		case USER_LEVEL_DESIGNER:
			menu.create("Concepteur d'album - ");
			
			menu.append("Sélectionner un album courant", '1', NULL, NULL);
			menu.append("Ajouter une carte", '2', dummy, NULL);
			menu.append("Afficher l'album", '3', NULL, NULL);
			menu.append("Fermer la session", 'N', NULL, NULL, true);
			
			return menu.process();
		break;
		
		case USER_LEVEL_COLLECT:
			menu.create("Collectionneur - ");
			
			menu.append("Sélectionner une carte courante", '1', NULL, NULL);
			menu.append("Pour la collection courante", '2', dummy, NULL);
			menu.append("Afficher la liste de mes collections", '3', NULL, NULL);
			menu.append("Comparer deux collections", '4', NULL, NULL);
			menu.append("Fermer la session", 'N', NULL, NULL, true);
			
			return menu.process();
		break;
		
		default:
			cerr << "Wrong userlevel this should not arrive !" << endl;
			return 1;
	}
}

/* Console Handling */
void UI::disable_echo() {
	struct termios tty_attr;
	
	if(tcgetattr(STDIN_FILENO, &tty_attr) < 0)
		return;
	
	/* Saving state */
	c_lflag = tty_attr.c_lflag;
	
	/* Changing */
	// tty_attr.c_lflag &= ~ICANON;
	tty_attr.c_lflag &= ~ECHO;

	if(tcsetattr(STDIN_FILENO, 0, &tty_attr) < 0)
		return;
}

void UI::restore_echo() {
	struct termios tty_attr;
	
	if(tcgetattr(STDIN_FILENO, &tty_attr) < 0)
		return;
	
	/* Restoring state */
	tty_attr.c_lflag = c_lflag;

	if(tcsetattr(STDIN_FILENO, 0, &tty_attr) < 0)
		return;
}
