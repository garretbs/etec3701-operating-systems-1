#include "console.h"
#include "disk.c"
#include "file.c"
#include "disktest.h"

void kmain(){
	console_init();
	disk_init();
	
	disk_test();
	
	//this is just so we know we've gotten here
    asm("ldr r0,=0x1234");
    while(1){
    }
}
