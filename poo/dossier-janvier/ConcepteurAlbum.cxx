#include "ConcepteurAlbum.hxx"
#include <cstdio>
#include <cstring>
using namespace std;

ConcepteurAlbum::ConcepteurAlbum() : AuthorisedPerson() {
	*_inc_name = '\0';
	_id = NULL;
}

ConcepteurAlbum::ConcepteurAlbum(ConcepteurAlbum &original) : AuthorisedPerson(original) {
	strcpy(_inc_name, original.getNomSociete());
	
	_id = new char[strlen(original.getIdentification()) + 1];
	strcpy(_id, original.getIdentification());
}

ConcepteurAlbum::ConcepteurAlbum(const char *name, const char *surname, const char *born, const char *nation, const char *login, const char *societe_name) : AuthorisedPerson(name, surname, born, nation, login) {
	strcpy(_inc_name, societe_name);
	_id = NULL;
}

ConcepteurAlbum::~ConcepteurAlbum() {
	/* if(_id != NULL)
		delete _id; */
}

char * ConcepteurAlbum::getIdentification() {
	if(_id != NULL)
		delete _id;
		
	_id = new char[strlen(getNom()) + strlen(_inc_name) + 1];
	
	sprintf(_id, "%s#%s", getNom(), _inc_name);
	return _id;
}

/* Display */
void ConcepteurAlbum::Affiche() {
	// Redirect to cout
	Affiche(cout);
}

/* Setters */
void ConcepteurAlbum::setNomSociete(const char *name) {
	strcpy(_inc_name, name);
}

/* Setters */
const char * ConcepteurAlbum::getNomSociete() {
	return _inc_name;
}

/* Overload */
void ConcepteurAlbum::Affiche(ostream &stream) const {
	Person::Affiche(stream);
	
	if(stream == cout) {
		stream << "Nom Soci: " << _inc_name << endl;
		stream << "Login: " << getLogin() << endl;
	
	} else {
		stream << _inc_name << endl;
		stream << getLogin() << endl;
	}
}

ostream & operator << (ostream &stream, ConcepteurAlbum const &source) {
	source.Affiche(stream);
	return stream;
}


void ConcepteurAlbum::Encode(istream &stream) {
	Person::Encode(stream);
	string temp;
		
	if(stream == cin)
		cout << "Nom Soci: ";
	stream >> _inc_name;
	
	if(stream != cin) {
		stream >> temp;
		setLogin(temp.c_str());
	}
}

istream & operator >> (istream &stream, ConcepteurAlbum &source) {
	source.Encode(stream);
	return stream;
}
