#define pl110 ( (volatile unsigned*)0xc0000000 )
#define haxis       (*(pl110+0))    //offset 0: horizontal axis                             
#define vaxis       (*(pl110+1))    //offset 4: vertical axis                               
#define polarity    (*(pl110+2))    //offset 8: clock+polarity
#define lineend     (*(pl110+3))    //offset 12: line end
#define baseaddr1   (*(pl110+4))    //offset 16: upper panel base address                           
#define baseaddr2   (*(pl110+5))    //offset 20: lower panel base address
#define intmask     (*(pl110+6))    //offset 24: interrupt mask
#define params      (*(pl110+7))    //offset 28: panel parameters   

#define WIDTH 800
#define HEIGHT 600
#define COL_MAX 88
#define ROW_MAX 66
#define MAX_ROWS 32

#define framebuffer ((volatile unsigned short*) (((0x07ffffff - WIDTH*HEIGHT*2))&~0xf)) //0x200000; 

#define COLOR16(r,g,b) (((b>>3) << 11 )| ((r>>3)) | ((g>>2) << 5))



void console_init();

void setpixel(int x, int y, unsigned short color);

void drawrectangle(int x, int y, int length, int width, unsigned short color);

void lab3();

void lab4();

void lab5();

void draw_character(int x, int y, char c);

void console_putc(char c);

void print_string(char *string);

void clear_screen();

void variadic_function(char *string, ...);