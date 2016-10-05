
ldr sp,=stack
b kmain

forever:
    b forever


.section .data

.global stack
    .rept 4096
    .word 0
    .endr
stack:
    
