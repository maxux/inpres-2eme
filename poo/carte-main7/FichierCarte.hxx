#ifndef __FICHIER_CARTE_H
	#define __FICHIER_CARTE_H
	
	#include <iostream>
	#include <fstream>
	#include "Carte.hxx"
	#include "CaractScore.hxx"
	
	class FichierCarte {
		public:
			FichierCarte();
			FichierCarte(const char *filename);
			~FichierCarte();
			void __construct(const char *filename);
			
			void save(Carte &carte);
			void list();
			bool load(Carte &carte, const char *name);
		
		private:
			char _filename[64];
			fstream _fp;
	};
#endif
