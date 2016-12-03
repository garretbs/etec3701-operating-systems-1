#include "irq.h"

unsigned short capslock = 0;

char linebuff[4096];
int linesize=0;

const char CHAR_ARRAY[] = {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
						'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0',
						'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 39, '`', '\0', '\\',
						'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' ', '\0',
						'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
						'7', '8', '9', '-',
						'4', '5', '6', '+',
						'1', '2', '3',
						'0', '.',
						'\0', '\0', '\0', '\0', '\0', '\0'};
						
const char SHIFT_ARRAY[] = {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
						'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0',
						'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 34, '~', '\0', '|',
						'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0', '\0', '\0', ' ', '\0',
						'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
						'7', '8', '9', '-',
						'4', '5', '6', '+',
						'1', '2', '3',
						'0', '.',
						'\0', '\0', '\0', '\0', '\0', '\0'};

void irq_init(){
	*IRQ_ENABLE = 0x1fe; //(this enables interrupts 1-8)
	asm volatile(
		"mrs r0,cpsr\n"
		"and r0,r0,#0xffffff7f\n"
		"msr cpsr,r0" 
		: : : "r0" 
	);
	timer_init();
	keyboard_init();
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
    TIMER1[2] = (1<<7) | (1<<6) | (1<<5) | (0<<2) | (1<<1) ;
	
	//set interrupt enable bit on CPU
	asm volatile(
		"mrs r0,cpsr\n"
		"and r0,r0,#0xffffff7f\n"
		"msr cpsr,r0" 
		: : : "r0" 
	);
}

void keyboard_init(){ //enable interface and interrupts
	KEYBOARD[0] = (1<<2) | (1<<4);
}

char get_scancode_char(int sc){
	return (capslock) ? SHIFT_ARRAY[sc] : CHAR_ARRAY[sc];
}

void shift(){
	capslock ^= 1;
}
