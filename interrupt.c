#include "kprintf.h"
#include "console.h"
#include "errno.h"
#include "syscalls.h"
#include "file.h"


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
	kprintf("irq\n");
}

void handler_fiq_c(){
	kprintf("fiq\n");
}
