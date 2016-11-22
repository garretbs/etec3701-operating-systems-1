#include "console.h"
#include "disk.c"
#include "file.h"
#include "irq.c"

void kmain(){
	console_init();
	disk_init();
	file_init();
	irq_init();
	
	int rv;
	rv = exec("displaytime.bin");
	if(rv < 0)
		kprintf("Zero Time Dilemma\n");
	
    while(1){
    }
}