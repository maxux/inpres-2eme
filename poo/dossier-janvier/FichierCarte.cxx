#include <cstring>
#include "FichierCarte.hxx"

using namespace std;

FichierCarte::FichierCarte() {
	__construct("Default.txt");
}

FichierCarte::FichierCarte(const char *filename) {
	__construct(filename);
}

void FichierCarte::__construct(const char *filename) {
	strncpy(_filename, filename, sizeof(_filename));
	_fp.open(_filename, ios::out | ios::in | ios::app);
}

FichierCarte::~FichierCarte() {
	_fp.close();
}

void FichierCarte::save(Carte &carte) {
	carte.Export(_fp);
}

void FichierCarte::list() {
	Carte temp;
	_fp.clear();
	_fp.seekg(0, ios::beg);
	
	while(!_fp.eof()) {
		_fp >> temp;
		cout << temp << endl;
	}
}

bool FichierCarte::load(Carte &carte, const char *name) {
	Carte temp;
	_fp.clear();
	_fp.seekg(0, ios::beg);
	
	while(!_fp.eof()) {
		_fp >> temp;
		
		if(strcmp(temp.getNomCarte(), name) == 0) {
			carte = temp;
			return true;
		}
	}
	
	return false;
}
