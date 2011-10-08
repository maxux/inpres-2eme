#include <iostream>
#include <fstream>
#include <string.h>
#include "Carte.hxx"

using namespace std;

Carte::Carte() {
	Carte("New", "New", 0, "New", 0);
}

Carte::Carte(const char *name, const char *serie, int id) {
	Carte(name, serie, id, "New", 0);
}
			
Carte::Carte(const char *name, const char *serie, int id, const char *caract_name, int caract_score) {
	strcpy(_name, name);
	
	_serie = new char[strlen(serie) + 1];
	strcpy(_serie, serie);
	
	Carte::setCaractScore(caract_name, caract_score);
	
	_id = id;
}

Carte::Carte(const char *name, const char *serie, int id, CaractScore &original) {
	Carte(name, serie, id, NULL, 0);
	setCaractScore(original);
}

Carte::Carte(Carte &original) {
	strcpy(_name, original._name);
	
	_serie = new char[strlen(original._serie) + 1];
	strcpy(_serie, original._serie);
	
	_id = original._id;
}

void Carte::Affiche() const {
	if(_id > 0)
		cout << "ID   : " << _id << endl;
	else cout << "ID   : <unknown>" << endl;
		
	if(*_name)
		cout << "Nom  : " << _name << endl;	
	else cout << "Nom  : <unknown>" << endl;
	
	if(_serie)
		cout << "Serie: " << _serie << endl;
	else cout << "Serie: <unknown>" << endl;
}

void Carte::setNumero(int id) {
	_id = id;
}

void Carte::setNomCarte(const char *name) {
	strncpy(_name, name, sizeof(_name));
	_name[sizeof(_name) - 1] = '\0';
}

void Carte::setNomSerie(const char *serie) {
	if(_serie != NULL)
		delete _serie;
	
	_serie = new char [strlen(serie)];
	strcpy(_serie, serie);
}

/* Return Values */
char* Carte::getNomCarte() {
	return _name;
}

char* Carte::getNomSerie() {
	return _serie;
}

int Carte::getNumero() {
	return _id;
}

/* Overload */
void Carte::Affiche(ostream &stream) const {
	if(_id > 0)
		stream << "ID   : " << _id << endl;
	else stream << "ID   : <unknown>" << endl;
		
	if(*_name)
		stream << "Nom  : " << _name << endl;	
	else stream << "Nom  : <unknown>" << endl;
	
	if(_serie)
		stream << "Serie: " << _serie << endl;
	else stream << "Serie: <unknown>" << endl;
}

ostream & operator << (ostream &stream, Carte const &carte) {
	carte.Affiche(stream);
	return stream;
}


void Carte::Encode(istream &stream) {
	cout << "Nom: ";
	stream >> _name;
	
	cout << "Serie: ";
	stream >> _serie;
	
	cout << "ID: ";
	stream >> _id;
}

istream & operator >> (istream &stream, Carte &carte) {
	carte.Encode(stream);
	return stream;
}

/* Aggrégation */
void Carte::setCaractScore(const char *name, int score) {
	//caract.setNomCaract(name);
	//caract.setScore(score);
}

void Carte::setCaractScore(CaractScore &original) {
	//caract.setNomCaract(original.getNomCaract);
	//caract.setScore(original.getScore);
}

CaractScore Carte::getCaractScore() {
	
	return caract;
}
