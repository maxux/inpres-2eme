#include <iostream>
#include <termios.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <glob.h>

#include "Album.hxx"
#include "Collection.hxx"
#include "Collectionneur.hxx"
#include "ConcepteurAlbum.hxx"

#include "LinkCarte.hxx"

#include "UserInterface.hxx"
#include "UserInterfaceMenu.hxx"
#include "UserInterfaceMenuLink.hxx"


/*
	ADMINISTRATEUR
*/

int admin_change_passwd(LinkCarte *link) {
	fstream passfile(FILENAME_LOGIN, fstream::in), tempfile;
	string newpass, line, line_keep, read_user;
	char *tok;
	UI ui;
	
	link = NULL;
	
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

int admin_display_userlist(LinkCarte *link) {
	fstream passfile(FILENAME_LOGIN);
	string line;
	
	link = NULL;
	
	while(passfile >> line)
		cout << line << endl;
	
	passfile.close();
	
	return 0;
}

int admin_display_userinfo(LinkCarte *link) {
	UI temp;
	string user;
	
	link = NULL;
	
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

int admin_add_collect(LinkCarte *link) {
	Collectionneur addme;
	fstream newuser;
	string login, filename;
	
	link = NULL;
	
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

int admin_add_designer(LinkCarte *link) {
	ConcepteurAlbum *addme = new ConcepteurAlbum;
	string login;
	
	/* Modify userlist */
	login = admin_add_user();

	cin >> *addme;
	addme->setLogin(login.c_str());
	
	link->_cit = link->_concept.insert(link->_concept.begin(), addme);
	
	return 0;
}




/*
	CONCEPTEUR D'ALBUM
*/

int designer_create_album(LinkCarte *link) {
	Album *n;
	string name;
	
	n = new Album;
	
	cout << "Nom: ";
	cin >> name;
	
	n->setName(name);
	
	if(link->_alb)
		link->_alb->Save();
		
	link->_alb = n;
	
	return 0;
}
	
int designer_load_album(LinkCarte *link) {
	Album *n;
	string temp;
	
	if(link->_alb)
		link->_alb->Save();
	
	delete link->_alb;
	
	n = new Album;
	link->_alb = n;
	
	cout << "Nom: ";
	cin >> temp;
	link->_alb->Load(temp.c_str());
	
	return 0;
}
	
int designer_load_custom(LinkCarte *link) {
	link = NULL;
	return 0;
}

int designer_add_card(LinkCarte *link) {
	Carte *n = new Carte;
	
	if(link->_alb) {
		cin >> *n;
		link->_alb->AddCarte(n);
		
	} else cout << "Aucun album sélectionné" << endl;
	
	return 0;
}

int designer_display_album(LinkCarte *link) {
	if(link->_alb) {
		link->_alb->displayCards();
		
	} else cout << "Aucun album sélectionné" << endl;
	
	return 0;
}


/*
	COLLECTIONNEUR
*/

int collec_add_collec(LinkCarte *link) {
	string collname;
	
	cout << "Nom: ";
	cin >> collname;
	
	return collec_add_collec_name(link, collname.c_str());
}

int collec_add_collec_name(LinkCarte *link, const char *name) {
	Collection *c;
	Album *a;
	string collname;
	
	collname = name;
	
	/* Checking Album Exists */
	a = new Album;
	if(!a->Load(collname.c_str())) {
		cout << "Oops: introuvable" << endl;
		delete a;
		
		return 0;
	}
	
	if(link->_alb)
		link->_alb->Save();
		
	link->_alb = a;
	
	/* Creating Collection */
	if(link->_coll != NULL)
		link->_coll->Save();
	
	c = new Collection;
	link->_coll = c;
	
	link->_coll->setName(collname.c_str());
	link->_coll->setUsername(link->_login.c_str());
	
	return 1;
}

int collec_load_collect(LinkCarte *link) {
	string collname;
	
	cout << "Nom: ";
	cin >> collname;
	
	return collec_load_collect_name(link, collname.c_str());
}

int collec_load_collect_name(LinkCarte *link, const char *name) {
	if(collec_add_collec_name(link, name))
		return link->_coll->Load(name, link);
	else
		return 0;
}

int collec_add_card(LinkCarte *link) {
	string id;
	
	if(link->_coll) {
		link->_alb->displayCards();
		
		cout << "ID: ";
		cin >> id;
		
		link->_coll->AddCarte(atoi(id.c_str()), link);
		
	} else cout << "Aucune collection sélectionnée" << endl;
	
	return 0;
}

int collec_check_full(LinkCarte *link) {
	if(link->_coll) {
		if(link->_coll->size() == link->_alb->size())
			cout << "Félicitation, collection complète !" << endl;
		else
			cout << "Hélas non, il te manque " << link->_alb->size() - link->_coll->size() << " cartes !" << endl;
		
	} else cout << "Aucune collection sélectionnée" << endl;
	
	return 0;
}

int collec_give_card(LinkCarte *link) {
	if(link->_coll) {
		
		cout << "Garde tes cartes, ça peut servir plus tard !" << endl;
		
	} else cout << "Aucune collection sélectionnée" << endl;
	
	return 0;
}

int collec_display_collect(LinkCarte *link) {
	if(link->_coll) {
		cout << *(link->_coll);
		
	} else cout << "Aucune collection sélectionnée" << endl;
	
	return 0;
}

int collec_display_bestcard(LinkCarte *link) {
	vector <CarteCollectionnee *> :: iterator it;
	
	if(link->_coll) {
		cout << *(link->_coll->searchBest());
		
	} else cout << "Aucune collection sélectionnée" << endl;
	
	return 0;
}

int collec_display_lesscard(LinkCarte *link) {
	if(link->_coll) {
		cout << *(link->_coll->searchLess());
		
	} else cout << "Aucune collection sélectionnée" << endl;
	
	return 0;
}

int collec_display_list(LinkCarte *link) {
	string pat, na;
	glob_t glob_result;
	unsigned int i;
	
	pat = PATH_DATA + link->_login + "_*.col";
	glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);

	for(i = 0; i < glob_result.gl_pathc; i++) {
		cout << "[+] Fichier: " << glob_result.gl_pathv[i] << endl;
		
		/* Skip « data/ » and « username » and « _ » */
		na = glob_result.gl_pathv[i] + strlen(PATH_DATA) + link->_login.length() + 1;
		
		/* Removing extension */	
		cout << " -> " << na.substr(0, na.length() - 4) << endl;
	}

	globfree(&glob_result);
	
	return 0;
}

int collec_compare(LinkCarte *link) {
	link = NULL;
	
	cout << "Hé ben non ! Hihi" << endl;
	return 0;
}
