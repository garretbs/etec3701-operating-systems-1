#include "util.h"

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