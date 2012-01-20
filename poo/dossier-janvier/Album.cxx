#include "MyContainers.hxx"
#include "Album.hxx"
#include "UserInterface.hxx"
#include <fstream>

Album::Album() {
	
}

Album::~Album() {
	Save();
}


void Album::setName(string name) {
	_name = name;
}
	
string Album::getName() {
	return _name;
}

void Album::AddCarte(Carte* card) {
	_c.insert(_c.end(), card);
}

void Album::displayCards() {
	MonVecteur <Carte *> :: iterator it;
	
	it = _c.begin();
	
	while(it != _c.end()) {
		cout << **(it) << endl;
		it++;
	}
}

void Album::Save() {
	fstream newer;
	string filename;
	MonVecteur <Carte *> :: iterator it;
	
	filename = PATH_DATA + _name + ".alb";
	
	/* Creating user file */
	newer.open(filename.c_str(), fstream::out);
	
	/* Saving data */
	it = _c.begin();
	while(it != _c.end())
		newer << **(it++);
	
	newer.close();
}

int Album::Load(const char *name) {
	fstream newer;
	string filename;
	Carte *temp;
	
	_name = name;
	
	filename = PATH_DATA + _name + ".alb";
	newer.open(filename.c_str(), fstream::in);
	
	if(newer.is_open()) {
		/* Loading */
		while(1) {
			temp = new Carte;
			newer >> *temp;
			
			if(!newer.eof()) {
				_c.insert(_c.begin(), temp);
				
			} else break;
		}
		
		newer.close();
	} else return 0;
	
	cout << "[+] Loaded Cards: " << _c.size() << endl;
	
	return 1;
}

int Album::size() {
	return _c.size();
}

Carte * Album::GetCarte(int id) {
	MonVecteur <Carte *> :: iterator it;
	
	it = _c.begin();
	
	while(it != _c.end()) {
		if((*(it))->getNumero() == id)
			return *it;
			
		it++;
	}
	
	return NULL;
}
