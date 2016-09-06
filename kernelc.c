#include "console.h"

void kmain(){
	console_init();
	lab3();
	lab4();
	
	//this is just so we know we've gotten here
    asm("ldr r0,=0x1234");
    while(1){
    }
}