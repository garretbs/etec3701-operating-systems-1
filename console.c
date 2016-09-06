#include "console.h"

void lab3(){
	drawrectangle(15, 15, COLOR16(0, 127, 255), 374, 241);
}

void console_init(){
	haxis = 0x3f1f3f00 | (WIDTH/4-4);
	vaxis = 0x80b6000 | (HEIGHT-1);
	polarity = 0x067f3800;
	baseaddr1 = (int) framebuffer;
	params = 0x1829;
}

void setpixel(int x, int y, unsigned short color ){
    framebuffer[ y*WIDTH+x ] = color;
}

void drawrectangle(int x, int y, unsigned short color, int length, int width){
	for(int i=0; i< length;i++){
		setpixel(x+i, y, color);
		setpixel(x+i, y+width, color);
	}
	for(int i=0;i<width;i++){
		setpixel(x, y+i, color);
		setpixel(x+length, y+i, color);
	}
	setpixel(length+x, width+y, color);
}
