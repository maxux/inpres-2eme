#ifndef __DATE_HEADER_H
	#define __DATE_HEADER_H
	
	using namespace std;

	class Date {
		public:
			Date();
			Date(Date &original);
			Date(char *date);
		
		private:
			unsigned char _day;
			unsigned char _month;
			unsigned char _year;
	};
#endif
