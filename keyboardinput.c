
#include "syscalls.h"
#include "usprintf.c"

int syscall0(int p0){
    register unsigned r0 asm ("r0");
    r0=p0;
    asm volatile("svc #0" : "+r"(r0) : : "memory","cc");
    return r0;
}

int syscall1(int p0, int p1){
    register unsigned r0 asm  ("r0");
    register unsigned r1 asm  ("r1");
    r0=p0;
    r1=p1;
    asm volatile("svc #0" : "+r"(r0),"+r"(r1) : : "memory","cc");
    return r0;
}

int syscall2(int p0, int p1, int p2){
    register unsigned r0 asm  ("r0");
    register unsigned r1 asm  ("r1");
    register unsigned r2 asm  ("r2");
    r0=p0;
    r1=p1;
    r2=p2;
    asm volatile("svc #0" : "+r"(r0),"+r"(r1), "+r"(r2) : : "memory","cc");
    return r0;
}

int syscall3(int p0, int p1, int p2, int p3){
    register unsigned r0 asm  ("r0");
    register unsigned r1 asm  ("r1");
    register unsigned r2 asm  ("r2");
    register unsigned r3 asm  ("r3");
    r0=p0;
    r1=p1;
    r2=p2;
    r3=p3;
    asm volatile("svc #0" : "+r"(r0),"+r"(r1), "+r"(r2), "+r"(r3) : : "memory","cc");
    return r0;
}

int main(int argc, char* argv[])
{
	char buf[40];
	char buf2[100];
	int num=0;
	int num2;
	num2 = sprintf(buf2, "User Typing program initialized. Type something.\n");
	syscall3(SYSCALL_WRITE,1,(unsigned) buf2, num2);
	while(1){
		syscall0(SYSCALL_HALT);
		num = syscall3(SYSCALL_READ,0,(unsigned)buf,40);
		num2 = sprintf(buf2,"You typed: %.*s\n",num,buf);
		syscall3(SYSCALL_WRITE,1,(unsigned)buf2,num2);
	}
    return 0;
}



