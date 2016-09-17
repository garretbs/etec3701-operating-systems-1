

#use these for Windows
CC=/cygdrive/c/gcc-arm-none-eabi-5_3-2016q1-20160330-win32/bin/arm-none-eabi-gcc.exe
LD=/cygdrive/c/gcc-arm-none-eabi-5_3-2016q1-20160330-win32/bin/arm-none-eabi-ld.exe
AS=$(CC)
OBJCOPY=/cygdrive/c/gcc-arm-none-eabi-5_3-2016q1-20160330-win32/bin/arm-none-eabi-objcopy.exe
QEMU=/cygdrive/c/users/garret/documents/os_tools/qemu-windows-2.4.1/qemu-system-arm.exe


#these should be cross-platform...
CC+= -Wall -c -mcpu=arm926ej-s -marm -Werror
LD+=-Map kernelmap.txt -T linkerscript.txt
AS+= -c -x assembler-with-cpp -mcpu=arm926ej-s
QEMUARGS=-machine integratorcp -kernel kernel.bin -serial stdio
DISPLAY?=:0
export DISPLAY
SDL_STDIO_REDIRECT=no
export SDL_STDIO_REDIRECT

all: 
	$(AS) kernelasm.s
	$(CC) kernelc.c
	$(CC) console.c
	$(CC) kprintf.c
	$(CC) util.c
	$(LD) -o kernel.tmp kernelasm.o kernelc.o console.o util.o kprintf.o
	$(OBJCOPY) -Obinary kernel.tmp kernel.bin
	$(QEMU) $(QEMUARGS) kernel.bin
	
	
clean:
	-/bin/rm *.o *.exe *.bin *.img *.tmp

	
