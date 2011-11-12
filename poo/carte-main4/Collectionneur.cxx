#include "Collectionneur.hxx"
#include <stdio.h>
#include <string.h>

using namespace std;

#define MAX_QUANTITY		9999999
#define MAX_QUANTITY_LEN	7

Collectionneur::Collectionneur() {
	*_name = '\0';
	_nb_collection = 0;
	_id = NULL;
}

Collectionneur::Collectionneur(Collectionneur &original) : AuthorisedPerson(original) {
	// setNomCollection(original.getNomCollection());
	setNbCollection(original.getNbCollection());
	_id = NULL;
}

Collectionneur::Collectionneur(const char *name, const char *surname, const char *born, const char *nation, const char *login, int nb_collection) : AuthorisedPerson(name, surname, born, nation, login) {
	setNbCollection(nb_collection);
	_id = NULL;
}

Collectionneur::~Collectionneur() {
	if(_id != NULL)
		delete _id;
}

char * Collectionneur::getIdentification() {
	if(_id != NULL)
		delete _id;
		
	_id = new char[strlen(getNom()) + MAX_QUANTITY_LEN + 1];
	
	sprintf(_id, "%s#%d", getNom(), _nb_collection);
	return _id;
}

/* Setters */
/* void Collectionneur::setNomCollection(const char *name) {
	strcpy(_name, name);
} */

void Collectionneur::setNbCollection(int collection) {
	_nb_collection = collection;
}

/* Getters */
/* char * Collectionneur::getNomCollection() {
	
} */

int Collectionneur::getNbCollection() {
	return _nb_collection;
}

/* Display */
void Collectionneur::Affiche() {
	// Redirect to cout
	Affiche(cout);
}

/* Overload */
void Collectionneur::Affiche(ostream &stream) const {
	Person::Affiche(stream);
	
	// stream << "Nom     : " << _name << endl;
	stream << "Nb Coll : " << _nb_collection << endl;
}

ostream & operator << (ostream &stream, Collectionneur const &source) {
	source.Affiche(stream);
	return stream;
}


void Collectionneur::Encode(istream &stream) {
	/* cout << "Nom     : ";
	stream >> _name; */
	Person::Encode(stream);
	
	cout << "Nb Coll : ";
	stream >> _nb_collection;
}

istream & operator >> (istream &stream, Collectionneur &source) {
	source.Encode(stream);
	return stream;
}
