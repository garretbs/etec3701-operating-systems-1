#include "console.h"
#include "disk.c"

void kmain(){
	console_init();
	disk_init();
	
	load_bitmap("flower1.bmp");
	
	//this is just so we know we've gotten here
    asm("ldr r0,=0x1234");
    while(1){
    }
}
