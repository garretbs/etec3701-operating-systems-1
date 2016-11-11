#include "console.h"
#include "disk.c"

void kmain(){
	console_init();
	disk_init();
	
	
	if(0){
        kprintf("This should give an undefined opcode\n");
        asm volatile(".word 0xf7f0a000");
    }else if(0){
        kprintf("This should give a breakpoint (prefetch abort)\n");
        asm volatile("bkpt");
    }else{
        //transition to user mode
        asm volatile("msr cpsr,0x10");
    
        //cause svc interrupt
        asm volatile("svc #1" );
    }
	
    while(1){
    }
}
