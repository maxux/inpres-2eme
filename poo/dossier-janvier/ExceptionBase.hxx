#ifndef __EX_BASE_HEADER_H
	#define __EX_BASE_HEADER_H
	
	#include <iostream>
	using namespace std;
	
	class ExceptionBase {
		protected:
			char *_message;
		
		public:
			ExceptionBase();
			ExceptionBase(const char *message);
			ExceptionBase(const ExceptionBase &original);
			virtual ~ExceptionBase();
			
			friend ostream & operator << (ostream &stream, const ExceptionBase &eb);
			void setMessage(const char *message);
			const char * getMessage() const;
	};
#endif
