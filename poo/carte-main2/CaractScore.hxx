#ifndef __CARACT_HEADER_H
	#define __CARACT_HEADER_H
	
	#include <iostream>
	
	using namespace std;

	class CaractScore {
		public:
			CaractScore(const char *name = "<unknown>", int score = 0);
			// virtual ~CaractScore();

			static const char AMITIE[];
			static const char AUDACE[];
			static const char AUTORITE[];
			static const char QUALITE_STAR[];

			static const CaractScore BEST_AMITIE;
			static const CaractScore BEST_QUALITE_STAR;
			
			void Affiche();

			void setNomCaract(const char *name);
			void setScore(int score);

			char * getNomCaract();
			int getScore();
			
			/* Overload Methods */
			void Encode(istream &stream);
			void Affiche(ostream &stream) const;

			/* Overload */
			friend std::ostream & operator << (ostream &flux, CaractScore const &caract);
			friend std::istream & operator >> (istream &flux, CaractScore &caract);


		private:
			unsigned int _score;
			char _name[16];
	};
#endif
