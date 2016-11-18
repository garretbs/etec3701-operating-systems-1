ldr r0, =sbss
ldr r1, =ebss
and r2,r2,#0
wiper:
    cmp r0,r1
    beq endwiper
    str r2,[r0]
    add r0,r0,#4
    b wiper
endwiper:

ldr sp,=stack

and r0,r0,#0
ldr r1,=itable
copyloop:
    ldr r2,[r1]
    str r2,[r0]
    add r1,r1,#4
    add r0,r0,#4
    cmp r0,#64
    blt copyloop

b kmain

.macro interrupt func
    ldr sp,=interrupt_stack 
    sub lr,lr,#4
    push {lr} 
    push {r0-r12} 
    bl \func 
    pop {r0-r12} 
    ldm sp!,{pc}^
.endm

.extern handler_reset_c
handler_reset:
    interrupt(handler_reset_c)
 
.extern handler_undefined_c
handler_undefined:
    interrupt(handler_undefined_c)
 
.extern handler_svc_c
handler_svc:
    ldr sp,=svc_stack 
    //don't subtract 4 from lr! svc is special...
    push {lr} 
    push {r0-r12} 
    mov r0,sp
    bl handler_svc_c
    pop {r0-r12} 
    ldm sp!,{pc}^
	
.extern handler_prefetchabort_c
handler_prefetchabort:
    interrupt(handler_prefetchabort_c)
	
.extern handler_dataabort_c
handler_dataabort:
    interrupt(handler_dataabort_c)
	
.extern handler_reserved_c
handler_reserved:
    interrupt(handler_reserved_c)
	
.extern handler_irq_c
handler_irq:
    interrupt(handler_irq_c)
	
.extern handler_fiq_c
handler_fiq:
    interrupt(handler_fiq_c)


itable:
    ldr pc, handler_reset_
    ldr pc, handler_undefined_
    ldr pc, handler_svc_
    ldr pc, handler_prefetchabort_
    ldr pc, handler_dataabort_
    ldr pc, handler_reserved_
    ldr pc, handler_irq_
    ldr pc, handler_fiq_
handler_reset_: 
    .word handler_reset
handler_undefined_:
    .word handler_undefined
handler_svc_:
    .word handler_svc
handler_prefetchabort_:
    .word handler_prefetchabort
handler_dataabort_:
    .word handler_dataabort
handler_reserved_:
    .word handler_reserved
handler_irq_:
    .word handler_irq
handler_fiq_:
    .word handler_fiq
	
	
	
	
forever:
    b forever


.section .data

.rept 1024
.word 0
.endr




.global stack
    .rept 4096
    .word 0
    .endr
stack:
    
interrupt_stack:

svc_stack:
