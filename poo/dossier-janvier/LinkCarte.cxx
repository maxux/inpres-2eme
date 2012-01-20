#include <iostream>
#include <string>
#include <fstream>

#include "LinkCarte.hxx"
#include "Collectionneur.hxx"
#include "ConcepteurAlbum.hxx"	

LinkCarte::LinkCarte() {
	ConcepteurAlbum *temp;
	fstream newer;
	
	_alb  = NULL;
	_coll = NULL;
	
	/* Creating user file */
	newer.open(FILENAME_DESIGNER, fstream::in);
	
	if(newer.is_open()) {
		/* Loading */
		while(1) {
			temp = new ConcepteurAlbum;
			newer >> *temp;
			
			if(!newer.eof()) {
				_concept.insert(_concept.begin(), temp);
				
			} else break;
		}
		
		newer.close();
	}
	
	cout << "[+] Loaded ConcepteurAlbum: " << _concept.size() << endl;
}

LinkCarte::~LinkCarte() {
	fstream newer;
	
	/* Creating designer file */
	newer.open(FILENAME_DESIGNER, fstream::out);
	
	/* Saving data */
	_cit = _concept.begin();
	while(_cit != _concept.end())
		newer << **(_cit++);
	
	newer.close();
	
	if(_alb)
		_alb->Save();
	
	if(_coll)
		_coll->Save();
}

void LinkCarte::setIteratorDesigner(string login) {
	string temp;
	list <ConcepteurAlbum*> :: iterator it;
	
	it = _concept.begin();
	_cit = _concept.begin();
	
	while(it != _concept.end()) {
		temp = (*it)->getLogin();
		
		if(temp == login) {
			_cit = it;
			break;
		}
		
		it++;
	}
}
