#include "irq.h"

void irq_init(){
	*IRQ_ENABLE = 0x1fe; //(this enables interrupts 1-8)
	asm volatile(
		"mrs r0,cpsr\n"
		"and r0,r0,#0xffffff7f\n"
		"msr cpsr,r0" 
		: : : "r0" 
	);
	timer_init();
}

void timer_init(){
    TIMER1[0] = 1000000; //initial value
    //note: TIMER[6] == base address + 24 
    //since each integer is 4 bytes
    TIMER1[6] = 1000000; //reload value
     
    //clear pending interrupts
    TIMER1[3] = 1;
     
    //enable timer, cycle value (ie, loop it), enable interrupts,
    //set prescale to 1, and use 32 bit timer
    TIMER1[2] = (1<<7) | (1<<6) | (1<<5) | (0 << 2) | (1<<1) ;
	
	//set interrupt enable bit on CPU
	asm volatile(
		"mrs r0,cpsr\n"
		"and r0,r0,#0xffffff7f\n"
		"msr cpsr,r0" 
		: : : "r0" 
	);
}