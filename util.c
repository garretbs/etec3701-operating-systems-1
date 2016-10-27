#include "util.h"
#include "kprintf.h"

volatile unsigned* serialport = (unsigned*) 0x16000000;
volatile unsigned* serialflags = (unsigned*) 0x16000018;

//d=destination, s=source, n=number of bytes
void kmemcpy(void *d, void *s, int n){
	char* dcopy = (char*) d;
	char* scopy = (char*) s;
	while(n--){
		*(dcopy)++ = *(scopy)++;
	}
}

void kmemset(void *p, char c, int n){
	char* pcopy = (char*) p;
	while(n--){
		*(pcopy)++ = c;
	}
}

void debugout(char x){
	while(*(serialflags)&32){
		;
	}
	*serialport = x;
}

//super function for division and modulo
void kdivmod(unsigned numerator, unsigned denominator, unsigned *qp, unsigned *rp){
	unsigned dividend = 0;
	unsigned quotient = 0;
	for(int i=0;i<32;i++){//unsigned is always 32 bits
		dividend <<= 1;//check leftmost (current) bit
		if(numerator & 0x80000000)//check if current bit of numerator is 1
			dividend |= 1;//if so, rightmost bit of dividend = 1
		numerator <<= 1;//check leftmost (current) bit
		quotient <<= 1;//check leftmost (current) bit
		if(dividend >= denominator){//check if dividend >= denominator
			quotient |= 1;//if so, rightmost bit of quotient = 1
			dividend -= denominator;//subtract the denominator from the dividend
		}		
	}
	*qp = quotient;
	*rp = dividend;
}

unsigned kdiv(unsigned numerator, unsigned denominator){
	unsigned q,r;
	kdivmod(numerator, denominator, &q, &r);
	return q;
}

unsigned kmod(unsigned numerator, unsigned denominator){
	unsigned q,r;
	kdivmod(numerator, denominator, &q, &r);
	return r;
}

int kstrcmp(char* str1, char* str2){
	for(int i=0;i<32;++i){
		if(str1[i] != str2[i])
			return 1;//not equal
		if(str1[i] == '\0')//end of string
			break;
	}
	return 0;//equal
}

//fefefe
int fnamecmp(char* dir, char* fname){
	int flen = kstrlen(fname);
	for(int i=0;i<flen;++i){
		if(dir[i] != fname[i])
			return 1;//not equal
	}
	return 0;
}

int kstrlen(char* str){
	int len = 0;
	for(int i=0;i<32;++i){
		if(str[i] == '\0')
			break;
		len++;
	}
	return len;
}