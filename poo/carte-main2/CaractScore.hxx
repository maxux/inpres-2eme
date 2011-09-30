#ifndef __CARACT_HEADER_H
	#define __CARACT_HEADER_H

	class CaractScore {
		public:
			CaractScore (const char *name = "<unknown>", int score = 0);
			virtual ~CaractScore ();

			static const char AMITIE[];
			static const char AUDACE[];
			static const char AUTORITE[];
			static const char QUALITE_STAR[];

			static const CaractScore BEST_AMITIE;
			// static const CaractScore BEST_QUALITE;
			
			void Affiche();

			void setNomCaract(const char *name);
			void setScore(int score);

			char * getNomCaract();
			int getScore();



		private:
			unsigned int _score;
			char _name;
	};
#endif
