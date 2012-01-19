#include <iostream>
#include <string>
#include <fstream>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "LinkCarte.hxx"

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
		/* Skip wrong line */
		if(line == ":")
			break;
			
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
		/* Skip wrong line */
		if(line == ":")
			break;
			
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
	struct stat statbuff;
	string pathcheck;
	
	/* If admin */
	if(username == "admin")
		return USER_LEVEL_ADMIN;
	
	/* Building collectionneur filename */
	pathcheck = PATH_DATA + username + ".co";
	
	/* Checking if Collectionneur */
	if(!stat(pathcheck.c_str(), &statbuff))
		return USER_LEVEL_COLLECT;
	
	/* Fallback to ConcepteurAlbum */
	return USER_LEVEL_DESIGNER;
}

void UI::prepare() {
	/* Pre-Check Login Access */
	_level = user_level(_login);	
}

int UI::start_events(LinkCarte *origin) {
	UIMenu menu;
	string title, temp;
	
	origin->_login = _login;
	
	switch(_level) {
		case USER_LEVEL_ADMIN:
			menu.create("Menu d'administration");
			
			menu.append("Afficher la liste des utilisateurs", '1', admin_display_userlist, NULL);
			menu.append("Afficher les infos d'un utilisateur", '2', admin_display_userinfo, NULL);
			menu.append("Créer un collectionneur", '3', admin_add_collect, NULL);
			menu.append("Créer un concepteur d'album", '4', admin_add_designer, origin);
			menu.append("Changer le mot de passe administrateur", '5', admin_change_passwd, NULL);
			menu.append("Fermer la session", 'N', NULL, NULL, true);
			
			return menu.process();
		break;
		
		case USER_LEVEL_DESIGNER:
			// Preparing environment
			origin->setIteratorDesigner(_login);
			origin->_alb = NULL;
			
			// Settign name
			temp = (*origin->_cit)->getNom();
			
			title = "Concepteur d'album - " + temp;
			menu.create(title.c_str());
			
			menu.append("Créer un nouvel album", '1', designer_create_album, origin);
			menu.append("Charger un album", '2', designer_load_album, origin);
			menu.append("Charger un de mes albums", '3', designer_load_custom, origin);
			menu.append("Ajouter une carte", '4', designer_add_card, origin);
			menu.append("Afficher l'album", '5', designer_display_album, origin);
			menu.append("Fermer la session", 'N', NULL, NULL, true);
			
			return menu.process();
		break;
		
		case USER_LEVEL_COLLECT:
			title = "Collectionneur - " + _login;
			menu.create(title.c_str());
			
			menu.append("Sélectionner une carte courante", '1', NULL, NULL);
			menu.append("Pour la collection courante", '2', NULL, NULL);
			menu.append("Afficher la liste de mes collections", '3', NULL, NULL);
			menu.append("Comparer deux collections", '4', NULL, NULL);
			menu.append("Fermer la session", 'N', NULL, NULL, true);
			
			return menu.process();
		break;
		
		default:
			cerr << "Wrong userlevel, this is not normal !" << endl;
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

	if(tcsetattr(STDIN_FILENO, TCSANOW, &tty_attr) < 0)
		return;
}

void UI::restore_echo() {
	struct termios tty_attr;
	
	if(tcgetattr(STDIN_FILENO, &tty_attr) < 0)
		return;
	
	/* Restoring state */
	tty_attr.c_lflag = c_lflag;

	if(tcsetattr(STDIN_FILENO, TCSANOW, &tty_attr) < 0)
		return;
}
