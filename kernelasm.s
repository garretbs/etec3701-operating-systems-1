
ldr sp,=stack
b kmain

forever:
    b forever


.section .data

.global stack
    .rept 1024
    .word 0
    .endr
stack:
    
