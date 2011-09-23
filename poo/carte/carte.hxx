#ifndef __CARTE_H
	#define __CARTE_H
	
	class Carte {
		private:
			char _name[30];
			char *_serie;
			int _id;			
		
		public:
			/* Construct */
			Carte();
			Carte(const char *name, const char *serie, int id);
			Carte(Carte &original);
			
			/* Affecting */
			void set_id(int id);
			void set_name(const char *name);
			void set_serie(const char *serie);
			
			void display();
	};
#endif
