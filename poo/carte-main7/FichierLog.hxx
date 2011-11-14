#ifndef __FICHIER_LOG_H
	#define __FICHIER_LOG_H
	
	#include <iostream>
	#include <fstream>
	
	using namespace std;
	
	class FichierLog {
		public:
			FichierLog(const char *filename);
			~FichierLog();
			
			/* Input/Output */
			void Write(char *data);
			
			/* Overload */
			friend FichierLog & operator << (FichierLog &stream, const char *data);
			
		private:
			ofstream _out;
			char _filename[64];
			
	};
#endif
