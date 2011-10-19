#include "Collectionneur.hxx"
#include <string.h>

using namespace std;

Collectionneur::Collectionneur() {
	*_name = '\0';
	_nb_collection = 0;
}

Collectionneur::Collectionneur(Collectionneur &original) {
	setNomCollection(original.getNomCollection());
	setNbCollection(original.getNbCollection());
}

Collectionneur::Collectionneur(const char *name, const char *surname, const char *born, const char *nation, const char *login, int nb_collection) {
	
}

char * Collectionneur::getIdentification() {
	
}

/* Setters */
void Collectionneur::setNomCollection(const char *name) {
	strcpy(_name, name);
}

void Collectionneur::setNbCollection(int collection) {
	_nb_collection = collection;
}

/* Getters */
char * Collectionneur::getNomCollection() {
	
}

int Collectionneur::getNbCollection() {
	
}

/* Display */
void Collectionneur::Affiche() {
	cout << "Nom     : " << _name << endl;
	cout << "Nb Coll : " << _nb_collection << endl;
}

/* Overload */
void Collectionneur::Affiche(ostream &stream) const {
	stream << "Nom     : " << _name << endl;
	stream << "Nb Coll : " << _nb_collection << endl;
}

ostream & operator << (ostream &stream, Collectionneur const &source) {
	source.Affiche(stream);
	return stream;
}


void Collectionneur::Encode(istream &stream) {
	cout << "Nom     : ";
	stream >> _name;
	
	cout << "Nb Coll : ";
	stream >> _nb_collection;
}

istream & operator >> (istream &stream, Collectionneur &source) {
	source.Encode(stream);
	return stream;
}
