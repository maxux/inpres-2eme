#include "CaractScore.hxx"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

const char CaractScore::AMITIE[] 	= "Amitié";
const char CaractScore::AUDACE[] 	= "Audace";
const char CaractScore::AUTORITE[] 	= "Autorité";
const char CaractScore::QUALITE_STAR[] 	= "Qualite Star";

CaractScore::CaractScore(const char *name, int score) {
	setNomCaract(name);
	setScore(score);
}

void CaractScore::Affiche() {
	cout << "Nom   : " << _name << endl;
	cout << "Scrore: " << _score << endl;
}

void CaractScore::setNomCaract(const char *name) {
	strncpy(_name, name, sizeof(_name));
	_name[sizeof(_name) - 1] = '\0';
}

void CaractScore::setScore(int score) {
	_score = score;
}

char * CaractScore::getNomCaract() {
	return _name;
}

int CaractScore::getScore() {
	return _score;
}

/* Overload */
void CaractScore::Affiche(ostream &stream) const {
	if(*_name)
		stream << "Nom  : " << _name << endl;	
	else stream << "Nom  : <unknown>" << endl;
	
	if(_score)
		stream << "Score: " << _score << endl;
	else stream << "Score: <unknown>" << endl;
}

ostream & operator << (ostream &stream, CaractScore const &caract) {
	caract.Affiche(stream);
	return stream;
}


void CaractScore::Encode(istream &stream) {
	cout << "Nom: ";
	stream >> _name;
	
	cout << "Score: ";
	stream >> _score;
}

istream & operator >> (istream &stream, CaractScore &caract) {
	caract.Encode(stream);
	return stream;
}

/* Static Declarations */
const CaractScore CaractScore::BEST_AMITIE("Amitié", 100);
CaractScore CaractScore::BEST_QUALITE_STAR;
