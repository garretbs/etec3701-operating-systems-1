#include "kprintf.h"
#include "console.h"
#include "errno.h"
#include "syscalls.h"
#include "file.h"
#include "irq.h"
#include "util.h"

static char linebuf[LINEBUF_SIZE];
static int linebuf_chars = 0;
static volatile int linebuf_ready = 0;

void halt(){
    asm volatile(
        "mov r0,#0\n"
        "mcr p15,0,r0,c7,c0,4" 
        : : : "r0"
    );
}

void sti(){
    asm volatile(
        "mrs r0,cpsr\n"
        "and r0,r0,#0xffffff7f\n"
        "msr cpsr,r0" : : : "r0" 
    );
}

void keyboard_interrupt(){
	unsigned q = KEYBOARD[2];
	int press = (q&0x80) ? 1 : 0;
	q &= 0x7f;
	char c = get_scancode_char(q);
	
	if(c == '\b'){
		if(linebuf_chars > 0 && press){
			--linebuf_chars;
			kprintf("\b");
		}
	}else if(c == '\n' && press){
		linebuf_ready = 1;
		kprintf("\n");
	}else if((q == 42 || q == 54) || (q == 58 && press)){ //shift keys and capslock
		shift();
	}else if(linebuf_chars >= LINEBUF_SIZE){
		linebuf_ready = 1;
	}else if(q < 90 && press){
		linebuf[linebuf_chars] = c;
		++linebuf_chars;
		kprintf("%c", c);
	}
}

int keyboard_getline(char* buffer,int num){
    while( !linebuf_ready ){
		sti();  //enable interrupts
		halt(); //halt until interrupt
    }
    if( num > linebuf_chars )
        num=linebuf_chars;
    kmemcpy(buffer,linebuf,num);
    linebuf_chars=0;
    linebuf_ready=0;
    return num;
}

void handler_reset_c(){
	kprintf("reset\n");
}

void handler_undefined_c(){
	kprintf("undefined\n");
}

void handler_svc_c(unsigned* ptr){
    switch(ptr[0]){
		case SYSCALL_READ:
		{
			//int fd = ptr[1];
			if(ptr[1] == 0){
				ptr[0] = keyboard_getline((char*) ptr[2], ptr[3]);
			}else if(ptr[1] < 3){
				ptr[0] = -ENOSYS;
			}else{
				unsigned buf = ptr[2];
				if( buf < 0x400000 ){
					ptr[0] = -EFAULT;
					break;
				}
				int count = ptr[3];
				if( count < 0 ){
					ptr[0] = -EINVAL;
					break;
				}
				ptr[0] = file_read(ptr[1], (void*) ptr[2], ptr[3]);
			}
			break;
		}
		case SYSCALL_WRITE:
		{
			//r1 = fd
			//r2 = buffer
			//r3 = count
			if( ptr[1] == 1 || ptr[1] == 2 ){
				int i;
				char* msg = (char*) ptr[2];
				for(i=0;i<ptr[3];++i)
					console_putc(msg[i]);
				ptr[0]=ptr[3];  //wrote all data
			}
			else
				ptr[0] = file_write( ptr[1], (void*) ptr[2], ptr[3] );
			break;
		}
        case SYSCALL_OPEN: //for reading from files
        {
            // r1 = filename
            // r2 = flags			
            ptr[0] = file_open( (void*) ptr[1], ptr[2]);
            break;
        }
        case SYSCALL_CLOSE:
        {
            // r1 = file descriptor
            ptr[0] = file_close(ptr[1]);
            break;
        }
        case SYSCALL_EXIT:
		{
			break;
		}
		case SYSCALL_CLEAR: //clears the console
		{
			console_putc('\f');
			break;
		}
		case SYSCALL_SET_COLOR: //sets the foreground and background colors
		{
			set_foreground_color(ptr[1]);
			set_background_color(ptr[2]);
			break;
		}
		case SYSCALL_HALT:
		{
			halt();
			break;
		}
		default:
			ptr[0] = -ENOSYS;
        }
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
	//see which interrupts are being asserted
    //PIC[0] = status
    //timer #1 = IRQ 6
    //Note: 0100 0000 == 64
    if( *IRQ_STATUS & 64 ){
        TIMER1[3] = 1;    //acknowledge at timer chip
        //*IRQ_ACKNOWLEDGE = ~64;   //acknowledge at PIC
    }else if (*IRQ_STATUS & 8){
		//KEYBOARD[3] = 1; //acknowledge keyboard
		keyboard_interrupt();
	}
}

void handler_fiq_c(){
	kprintf("fiq\n");
}
