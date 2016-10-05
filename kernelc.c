#include "kprintf.h"
#include "console.h"
#include "disk.c"
#include "util.h"

void kmain(){
	console_init();
	disk_init();

	print_directories(1, 0);
	

	//this is just so we know we've gotten here
    asm("ldr r0,=0x1234");
    while(1){
    }
}
