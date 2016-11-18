
#include "syscalls.h"

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
    syscall2(SYSCALL_SET_COLOR,0x010101,0x0ff0);
    syscall0(SYSCALL_CLEAR);
    
    //open( about.txt, 0 )
    const char* fname = "article4.txt";
    char buf[32];
    int fd = syscall2( SYSCALL_OPEN, (unsigned)fname, 0 );
    if( fd >= 0 ){
        while(1){
            //read(fd,buf,32)
            int rv = syscall3(SYSCALL_READ,fd,(unsigned)buf,32);
            if( rv <= 0 )
                break;
            syscall3(SYSCALL_WRITE,2,(unsigned)buf,rv);
        }
    }
    else{
        syscall2(SYSCALL_SET_COLOR,0xf800,0x001f);
        syscall0(SYSCALL_CLEAR);
    }
    
    while(1){
    }
    return 0;
    
}

