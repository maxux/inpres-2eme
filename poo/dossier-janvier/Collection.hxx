#ifndef __COLLECTION_H
	#define __COLLECTION_H
	
	#include "Carte.hxx"
	#include "CarteCollectionnee.hxx"
	#include "MyContainers.hxx"
	
	#include <iostream>
	#include <string>
	
	class LinkCarte;
	
	class Collection {
		public:
			/* Construct */
			Collection();
			virtual ~Collection();
			
			void setName(const char *name);
			void setUsername(const char *name);
			
			int Load(const char *name, LinkCarte *link);
			void Save();
			
			/* Overload */
			friend std::ostream & operator << (ostream &flux, Collection const &source);
			friend std::istream & operator >> (istream &flux, Collection &source);
			
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;
			
			int size();
			void AddCarte(int id, LinkCarte *link);
			
			int GetCarte(int id, MonVecteur <CarteCollectionnee *> :: iterator *it);
			
			Carte * searchBest();
			Carte * searchLess();
			
		private:
			MonVecteur <CarteCollectionnee *> _cards;
			MonVecteur <CarteCollectionnee *> :: iterator _cit;
			string _name;
			string _user;
	};
#endif
