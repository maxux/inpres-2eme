#ifndef __DATE_HEADER_H
	#define __DATE_HEADER_H
	
	using namespace std;

	class Date {
		public:
			Date();
			Date(Date &original);
			Date(char *date);
			virtual ~Date();
			
			/* Setters */
			void setDate(const char *date);
			
			/* Getters */
			const char * getDate() const;
			unsigned short getDay();
			unsigned short getMonth();
			unsigned short getYear();
		
		private:
			unsigned short _day;
			unsigned short _month;
			unsigned short _year;
			char *_to_char;
	};
#endif
