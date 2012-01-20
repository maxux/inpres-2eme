#ifndef __ALBUM_HEADER_H
	#define __ALBUM_HEADER_H
	
	#include "Carte.hxx"
	#include "MyContainers.hxx"
	#include <iostream>
	
	using namespace std;

	class Album {
		public:
			Album();
			
			virtual ~Album();
			
			void setName(string name);
			string getName();
			
			void AddCarte(Carte* card);
			void displayCards();
			
			void Save();
			int Load(const char *name);
			
			int size();
			Carte * GetCarte(int id);
		
		private:
			string _name;
			MonVecteur <Carte*> _c;
	};
#endif
