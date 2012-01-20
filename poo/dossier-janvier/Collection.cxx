#include <iostream>
#include <fstream>
#include <string.h>
#include "Carte.hxx"
#include "Collection.hxx"
#include "UserInterface.hxx"

using namespace std;

Collection::Collection() {
	
}

Collection::~Collection() {
	
}

int Collection::Load(const char *name, LinkCarte *link) {
	fstream newer;
	string filename;
	vector <CarteCollectionnee *> :: iterator it;
	CarteCollectionnee *temp;
	
	_name = name;
	
	filename = PATH_DATA + _user + "_" + _name + ".col";
	
	/* Creating user file */
	newer.open(filename.c_str(), fstream::in);
	
	if(newer.is_open()) {
		/* Loading */
		while(1) {
			temp = new CarteCollectionnee;
			newer >> *temp;
			
			temp->_card = link->_alb->GetCarte(temp->getCardID());
			
			if(!newer.eof()) {
				_cards.insert(_cards.begin(), temp);
				
			} else break;
		}
		
		newer.close();
	} else return 0;
	
	cout << "[+] Loaded Unique Cards: " << _cards.size() << endl;
	return 1;
}

void Collection::Save() {
	fstream newer;
	string filename;
	vector <CarteCollectionnee *> :: iterator it;
	
	filename = PATH_DATA + _user + "_" + _name + ".col";
	
	/* Creating user file */
	newer.open(filename.c_str(), fstream::out);
	
	/* Saving data */
	it = _cards.begin();
	while(it != _cards.end())
		newer << **(it++);
	
	newer.close();
}

void Collection::setName(const char *name) {
	_name = name;
}

void Collection::setUsername(const char *name) {
	_user = name;
}

int Collection::size() {
	return _cards.size();
}

void Collection::AddCarte(int id, LinkCarte *link) {
	vector <CarteCollectionnee *> :: iterator it;
	CarteCollectionnee *n;
	Carte *c;
	
	c = link->_alb->GetCarte(id);
	
	if(!c) {
		cout << "Carte introuvable" << endl;
		return;
	}
		
	if(GetCarte(id, &it)) {
		(*it)->increment();
		cout << "Vous avez maintenant " << (*it)->getCount() << "x la carte " << id << endl;
		
		return;
	}
	
	n = new CarteCollectionnee;
	n->_card = c;
	n->setID(c->getNumero());
	n->increment();
	
	cout << "Ajout de la carte: " << id << endl;
	_cards.insert(_cards.end(), n);
}

int Collection::GetCarte(int id, vector <CarteCollectionnee *> :: iterator *it) {
	*it = _cards.begin();
	
	while(*it != _cards.end()) {
		if((**it)->_card->getNumero() == id)
			return 1;
			
		(*it)++;
	}
	
	return 0;
}

Carte * Collection::searchBest() {
	vector <CarteCollectionnee *> :: iterator it;
	Carte *max;
	
	it = _cards.begin();
	max = (*it)->_card;
	
	while(it != _cards.end()) {
		if((*it)->_card->getCaractScore().getScore() > (max)->getCaractScore().getScore())
			max = (*it)->_card;
		
		it++;
	}
	
	return max;
}

Carte * Collection::searchLess() {
	vector <CarteCollectionnee *> :: iterator it;
	Carte *min;
	
	it = _cards.begin();
	min = (*it)->_card;
	
	while(it != _cards.end()) {
		if((*it)->_card->getCaractScore().getScore() < (min)->getCaractScore().getScore())
			min = (*it)->_card;
		
		it++;
	}
	
	return min;
}

/* Overload */
void Collection::Affiche(ostream &stream) const {
	vector <CarteCollectionnee *> :: const_iterator it;
	
	it = _cards.begin();
	while(it != _cards.end()) {
		(*it)->Affiche(stream);
		it++;
	}
}

ostream & operator << (ostream &stream, Collection const &source) {	
	source.Affiche(stream);
	return stream;
}


void Collection::Encode(istream &stream) {
	vector <CarteCollectionnee *> :: iterator it;
	
	it = _cards.begin();
	while(it != _cards.end()) {
		(*it)->Encode(stream);
		it++;
	}
}

istream & operator >> (istream &stream, Collection &source) {
	source.Encode(stream);
	return stream;
}
