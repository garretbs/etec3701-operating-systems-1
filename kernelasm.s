
.section .text

//Test value
ldr r1,=#2016

//Current Bit
ldr r3,=#0

loop:
	mov r2,r1 //Get bit value
	lsr r2,r3 //Right shift until it is bit 0
	and r2,r2,#1 //AND with #1 to check if it is 1
	add r0,r0,r2 //ADD to the counter in r0

	add r3,#1 //Increment counter

	cmp r3,#64 //Return if max reached
	bne loop

forever:
    b forever

	