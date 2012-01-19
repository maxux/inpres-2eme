#include <iostream>
#include <fstream>
#include <string.h>
#include "CarteCollectionnee.hxx"

using namespace std;

CarteCollectionnee::CarteCollectionnee() {
	_card  = NULL;
	_count = 0;
	_cid   = 0;
}

CarteCollectionnee::~CarteCollectionnee() {
	
}

/* Overload */
void CarteCollectionnee::Affiche(ostream &stream) const {
	if(stream == cout) {
		stream << *_card;
		stream << "==> " << _count << "x" << endl;
		
	} else {
		stream << _count << endl;
		stream << _cid << endl;
	}
}

ostream & operator << (ostream &stream, CarteCollectionnee const &source) {
	source.Affiche(stream);
	return stream;
}


void CarteCollectionnee::Encode(istream &stream) {
	stream >> _count;
	stream >> _cid;
}

istream & operator >> (istream &stream, CarteCollectionnee &source) {
	source.Encode(stream);
	return stream;
}

void CarteCollectionnee::increment() {
	_count++;
}

int CarteCollectionnee::getCount() {
	return _count;
}

int CarteCollectionnee::getCardID() {
	return _cid;
}

void CarteCollectionnee::setID(int id) {
	_cid = id;
}
