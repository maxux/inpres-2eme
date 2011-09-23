#include <iostream>
#include <string.h>
#include "carte.hxx"

using namespace std;

Carte::Carte() {
	*_name = '\0';
	_serie = NULL;
	_id = -1;
}

Carte::Carte(const char *name, const char *serie, int id) {
	strcpy(_name, name);
	
	_serie = new char[strlen(serie) + 1];
	strcpy(_serie, serie);
	
	_id = id;
}

Carte::Carte(Carte &original) {
	strcpy(_name, original._name);
	
	_serie = new char[strlen(original._serie) + 1];
	strcpy(_serie, original._serie);
	
	_id = original._id;
}

void Carte::set_id(int id) {
	_id = id;
}

void Carte::set_name(const char *name) {
	strncpy(_name, name, sizeof(_name));
	_name[sizeof(_name) - 1] = '\0';
}

void Carte::set_serie(const char *serie) {
	if(_serie != NULL)
		delete _serie;
	
	_serie = new char [strlen(serie)];
	strcpy(_serie, serie);
}

void Carte::display() {
	if(_id > -1) {
		cout << "ID   : " << _id << endl;
		
		if(*_name)
			cout << "Nom  : " << _name << endl;	
		else cout << "Nom  : <unknown>" << endl;
		
		if(_serie)
			cout << "Serie: " << _serie << endl;
		else cout << "Serie: <unknown>" << endl;
		
	} else cerr << "Invalid ID" << endl;
}
