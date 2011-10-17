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
	__construct(name, score);
}

void CaractScore::__construct(const char *name, int score) {
	setNomCaract(name);
	setScore(score);
}

void CaractScore::Affiche() const {
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

unsigned int CaractScore::getScore() {
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

/* Advanced Overload */
CaractScore & CaractScore::operator = (CaractScore &rval) {
	__construct(rval.getNomCaract(), rval.getScore());
	return *this;
}

CaractScore & CaractScore::operator + (int value) {
	_score += value;
	return *this;
}

CaractScore & CaractScore::operator - (int value) {
	_score -= value;
	return *this;
}

int CaractScore::operator - (CaractScore &rval) {
	/* if(strcmp(rval.getNomCaract(), _name) != 0)
		return 0; */
	
	return rval.getScore() - _score;
}

bool CaractScore::operator < (CaractScore &rval) {
	return _score < rval.getScore();
}

CaractScore & CaractScore::operator ++ () {
	_score += 5;
	return *this;
}

CaractScore CaractScore::operator ++ (int) {
	CaractScore copy = *this;
	
	// ADD: Check MAX Value
	_score += 5;		
	return copy;
}

/* Static Declarations */
const CaractScore CaractScore::BEST_AMITIE("Amitié", 100);
CaractScore CaractScore::BEST_QUALITE_STAR;
