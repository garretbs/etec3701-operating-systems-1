#include "console.h"
#include "disk.c"
#include "file.h"
#include "syscalls.h"

void kmain(){
	console_init();
	disk_init();
	file_init();
	
	int rv;
	rv = exec("print.bin");
	if(rv < 0)
		kprintf("No print for you.\n");
	
    while(1){
    }
}