#define IRQ_STATUS ((volatile unsigned*) 0x14000000)
#define IRQ_ACKNOWLEDGE ((volatile unsigned*) 0x14000004)
#define IRQ_ENABLE ((volatile unsigned*) 0x14000008)
#define IRQ_DISABLE ((volatile unsigned*) 0x1400000c)

#define TIMER0 ((volatile unsigned*) 0x13000000)
#define TIMER1 ((volatile unsigned*) 0x13000100)
#define TIMER2 ((volatile unsigned*) 0x13000200)

#define KEYBOARD ((volatile unsigned*) 0x18000000)
#define LINEBUF_SIZE 89

void irq_init();

void timer_init();

void keyboard_init();

char get_scancode_char(int sc);

void shift();
