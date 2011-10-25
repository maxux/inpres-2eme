#include "Person.hxx"
#include <string.h>

using namespace std;

Person::Person() {
	*_name	  = '\0';
	*_surname = '\0';
	*_nation  = '\0';
	
	*_born    = '\0';
	_born[10] = '\0';
}

Person::Person(Person &original) {
	setNom(original.getNom());
	setPrenom(original.getPrenom());
	setDateNais(original.getDateNais());
	setNationalite(original.getNationalite());
}

Person::Person(const char *name, const char *surname, const char *born, const char *nation) {
	setNom(name);
	setPrenom(surname);
	setDateNais(born);
	setNationalite(nation);
}

char * Person::getIdentification() {
	return _name;
}

/* Display */
void Person::Affiche() {
	// Redirect to stream cout
	Affiche(cout);
}

/* Setters */
void Person::setNom(const char *name) {
	strcpy(_name, name);
}

void Person::setPrenom(const char *surname) {
	strcpy(_surname, surname);
}

void Person::setDateNais(const char *date) {
	// _born.setDate(date);
	strncpy(_born, date, 10);
	_born[10] = '\0';
}

void Person::setNationalite(const char *nation) {
	strcpy(_nation, nation);
}

/* Getters */
const char * Person::getNom() {
	return _name;
}

const char * Person::getPrenom() {
	return _surname;
}

const char * Person::getDateNais() {
	return _born;
}

const char * Person::getNationalite() {
	return _nation;
}

/* Overload */
void Person::Affiche(ostream &stream) const {
	stream << "Nom     : " << _name << endl;
	stream << "Prénom  : " << _surname << endl;
	stream << "Nationa : " << _nation << endl;
	stream << "Born on : " << _born << endl << endl;
}

ostream & operator << (ostream &stream, Person const &source) {
	source.Affiche(stream);
	return stream;
}

void Person::Encode(istream &stream) {
	cout << "Nom     : ";
	stream >> _name;
	
	cout << "Prénom  : ";
	stream >> _surname;
	
	cout << "Born on : ";
	stream >> _born;
	
	cout << "Nationa : ";
	stream >> _nation;
	
	cout << endl;
}

istream & operator >> (istream &stream, Person &source) {
	source.Encode(stream);
	return stream;
}
