#ifndef __PETRA_H
#define __PETRA_H
	union binary {
		unsigned short word;
		
		struct byte {
			unsigned char lsb;
			unsigned char msb;
		} byte;
		
		struct {
			unsigned bit0 :1;
			unsigned bit1 :1;
			unsigned bit2 :1;
			unsigned bit3 :1;
			unsigned bit4 :1;
			unsigned bit5 :1;
			unsigned bit6 :1;
			unsigned bit7 :1;
			unsigned bit8 :1;
			unsigned bit9 :1;
			unsigned bit10 :1;
			unsigned bit11 :1;
			unsigned bit12 :1;
			unsigned bit13 :1;
			unsigned bit14 :1;
			unsigned bit15 :1;
		} bits;
	};
	
	typedef struct capteurs {
		unsigned int L1 :1;
		unsigned int L2 :1;
		unsigned int T  :1;
		unsigned int S  :1;
		unsigned int CS :1;
		unsigned int AP :1;
		unsigned int PP :1;
		unsigned int DE :1;
		
	} capteurs;

	typedef union u_capt {
		struct capteurs capt;
		unsigned char byte;
		
	} u_capt;

	
#endif
