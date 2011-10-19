#include "ConcepteurAlbum.hxx"
using namespace std;

ConcepteurAlbum::ConcepteurAlbum() {
	
}

ConcepteurAlbum::ConcepteurAlbum(ConcepteurAlbum &original) {
	
}

ConcepteurAlbum::ConcepteurAlbum(const char *name, const char *surname, const char *born, const char *nation, const char *login, const char *collection_name) {
	
}

char * ConcepteurAlbum::getIdentification() {
	
}

/* Display */
void ConcepteurAlbum::Affiche() {
	
}

/* Setters */
void ConcepteurAlbum::setNomSociete(const char *name) {
	
}

/* Overload */
void ConcepteurAlbum::Affiche(ostream &stream) const {
	
}

ostream & operator << (ostream &stream, ConcepteurAlbum const &source) {
	source.Affiche(stream);
	return stream;
}


void ConcepteurAlbum::Encode(istream &stream) {
	
}

istream & operator >> (istream &stream, ConcepteurAlbum &source) {
	source.Encode(stream);
	return stream;
}
