#ifndef USPRINTF_H
#define USPRINTF_H

//implementation of printf

//the attribute stuff allows gcc to do compile time checks
//to ensure we don't do something dumb like:
//kprintf( "%s" , 10 );

//writes to the string s. Does not check for buffer overflow.
int sprintf(char* s, char* fmt, ... ) __attribute__((format (printf , 2, 3 ) )); 

#endif
