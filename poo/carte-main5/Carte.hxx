#ifndef __CARTE_H
	#define __CARTE_H
	
	#include <iostream>
	#include "CaractScore.hxx"
	
	class Carte {
		private:
			char _name[30];
			char *_serie;
			int _id;
			CaractScore caract;
		
		public:
			/* Construct */
			Carte();
			Carte(const char *name, const char *serie, int id);
			Carte(const char *name, const char *serie, int id, const char *caract_name, int caract_score);
			Carte(const char *name, const char *serie, int id, CaractScore &original);
			Carte(Carte &original);
			
			/* Real Init */
			void __construct(const char *name, const char *serie, int id, const char *caract_name, int caract_score);
			
			/* Affecting */
			void setNumero(int id);
			void setNomCarte(const char *name);
			void setNomSerie(const char *serie);
			
			void setCaractScore(const char *name, int score);
			void setCaractScore(CaractScore &original);
			
			/* Return */
			char* getNomCarte();
			char* getNomSerie();
			int getNumero();
			CaractScore getCaractScore();
			
			/* Compare */
			bool IdentNomCaract(Carte &c);
			
			/* Display */
			void Affiche() const;
			void Affiche(std::ostream &stream) const;
			
			/* Encoding */
			void Encode(std::istream &stream);
			
			/* Overload */
			friend std::ostream & operator << (std::ostream &flux, Carte const &carte);
			friend std::istream & operator >> (std::istream &flux, Carte &carte);
			
			Carte & operator = (Carte &rval);
			
			Carte & operator + (int value);
			Carte & operator - (int value);
			int operator - (Carte &rval);
			
			bool operator < (Carte &rval);

			Carte & operator ++ ();
			Carte & operator ++ (int);
	};
#endif
