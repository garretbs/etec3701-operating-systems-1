#include "console.h"
#include "font.h"

void lab3(){
	drawrectangle(15, 15, 374, 241, COLOR16(0, 127, 255));
}

void lab4(){
	//print a Z in the center of the screen
	draw_character((WIDTH-CHAR_WIDTH)/2, (HEIGHT-CHAR_HEIGHT)/2, 'Z');
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

void drawrectangle(int x, int y, int length, int width, unsigned short color){
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

void draw_character(int x, int y, char character){
	int c = (int) character;
	int fcolor = COLOR16(255,255,255);
	int bgcolor = COLOR16(0,0,0);
	for(int i=0;i<CHAR_HEIGHT;i++){
		for(int j=0;j<CHAR_WIDTH;j++){
			if((font_data[c][i] >> j & 1) == 1){
				setpixel(x+CHAR_WIDTH-j, y+i, fcolor);
			}else{
				setpixel(x+CHAR_WIDTH-j, y+i, bgcolor);
			}
		}
	}
}
