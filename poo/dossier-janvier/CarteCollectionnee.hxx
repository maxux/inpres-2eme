#ifndef __CARTE_COLL_H
	#define __CARTE_COLL_H
	
	#include <iostream>
	#include "Carte.hxx"
	
	class CarteCollectionnee {
		public:
			/* Construct */
			CarteCollectionnee();
			virtual ~CarteCollectionnee();
			
			Carte *_card;
			
			/* Overload */
			friend std::ostream & operator << (ostream &flux, CarteCollectionnee const &source);
			friend std::istream & operator >> (istream &flux, CarteCollectionnee &source);
			
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;
			
			void increment();
			int getCount();
			
			void setID(int id);
			int getCardID();
			
		private:
			int _cid;
			int _count;
	};
#endif
