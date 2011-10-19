#include "Person.hxx"
#include <string.h>

using namespace std;

Person::Person() {
	*_name	  = '\0';
	*_surname = '\0';
	*_nation  = '\0';
}

Person::Person(Person &original) {
	setNom(original.getNom());
	setPrenom(original.getPrenom());
	setNationalite(original.getNationalite());
}

Person::Person(const char *name, const char *surname, const char *born, const char *nation) {
	setNom(name);
	setPrenom(surname);
	// setDateNais(born);
	setNationalite(nation);
}

char * Person::getIdentification() {
	
}

/* Display */
void Person::Affiche() {
	cout << "Nom     : " << _name << endl;
	cout << "Prénom  : " << _surname << endl;
	cout << "Nationa : " << _nation << endl;
	// cout << "Date N. :" << endl;
}

/* Setters */
void Person::setNom(const char *name) {
	strcpy(_name, name);
}

void Person::setPrenom(const char *surname) {
	strcpy(_surname, surname);
}

void Person::setDateNais(const char *date) {
	
}

void Person::setNationalite(const char *nation) {
	strcpy(_nation, nation);
}

/* Getters */
char * Person::getNom() {
	return _name;
}

char * Person::getPrenom() {
	return _surname;
}

char * Person::getDateNais() {
	
}

char * Person::getNationalite() {
	return _nation;
}

/* Overload */
void Person::Affiche(ostream &stream) const {
	stream << "Nom     : " << _name << endl;
	stream << "Prénom  : " << _surname << endl;
	stream << "Nationa : " << _nation << endl;
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
	
	cout << "Nationa : ";
	stream >> _nation;
}

istream & operator >> (istream &stream, Person &source) {
	source.Encode(stream);
	return stream;
}
