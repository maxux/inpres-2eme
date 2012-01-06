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
	
	return check_login(_login);
}

int UI::check_login_exists(string login) {
	ifstream passfile(FILENAME_LOGIN);
	string line;
	string read_user, read_pass;
	char *tok;
	
	/* File doesn't exists. Fallback to admin */
	if(!passfile)
		return (login == "admin");
	
	/* Reading each lines */
	while(passfile >> line) {
		tok = strtok((char*) line.c_str(), ":");
		read_user = tok;
		
		/* Checking Login */
		if(read_user == login) {
			passfile.close();
			return 1;
		}
	}
	
	/* Not found */
	return 0;
}

int UI::check_login(string login) {
	ifstream passfile(FILENAME_LOGIN);
	string line;
	string read_user, read_pass;
	char *tok;
	
	/* File doesn't exists. Fallback to admin */
	if(!passfile.is_open())
		return anti_flood((login == "admin" && _passwd == ADMIN_DEFAULT_PASSWORD));	
	
	/* Reading each lines */
	while(passfile >> line) {
		tok = strtok((char*) line.c_str(), ":");
		read_user = tok;
		
		/* Checking Login */
		if(read_user != login)
			continue;
		
		/* Compare password */
		tok = strtok(NULL, ":");
		read_pass = tok;
		
		passfile.close();
		
		return anti_flood((read_pass == _passwd));
	}
	
	/* Fallback admin (not from file) */
	if(login == "admin" && _passwd == ADMIN_DEFAULT_PASSWORD)
		return 1;
		
	/* Not found */
	passfile.close();
	return anti_flood(0);
}

inline int UI::anti_flood(int value) {
	if(!value)
		usleep(2000000);
	
	return value;
}

int UI::user_level(string username) {
	if(username == "admin") {
		return USER_LEVEL_ADMIN;
	}
	
	return USER_LEVEL_COLLECT;
}

void UI::prepare() {
	/* Pre-Check Login Access */
	_level = user_level(_login);	
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
			menu.append("Afficher les infos d'un utilisateur", '2', admin_display_userinfo, NULL);
			menu.append("Créer un collectionneur", '3', admin_add_collect, NULL);
			menu.append("Créer un concepteur d'album", '4', admin_add_designer, NULL);
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
