.section .text
.global start

start:

//clear the bss
//similar to the kernel's code...
ldr r0, =sbss
ldr r1, =ebss
and r2,r2,#0
wiper:
    cmp r0,r1
    beq endwiper
    str r2,[r0]
    add r0,r0,#1
    b wiper
endwiper:

b main