#include "console.h"
#include "disk.c"
#include "file.h"

void kmain(){
	console_init();
	disk_init();
	
	int rv;
	rv = exec("blink.bin");
	if(rv < 0)
		kprintf("No blink for you.\n");
	
	
    while(1){
    }
}