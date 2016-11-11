#include "kprintf.h"

void handler_reset_c(){
	kprintf("reset\n");
}

void handler_undefined_c(){
	kprintf("undefined\n");
}

void handler_svc_c(){
	kprintf("svc\n");
}

void handler_prefetchabort_c(){
	kprintf("prefetch abort\n");
}

void handler_dataabort_c(){
	kprintf("data abort\n");
}

void handler_reserved_c(){
kprintf("reserved\n");
}

void handler_irq_c(){
	kprintf("irq\n");
}

void handler_fiq_c(){
	kprintf("fiq\n");
}