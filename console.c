#include "console.h"
#include "font.h"
#include "util.h"


void lab3(){
	drawrectangle(15, 124, 319, 130, COLOR16(0, 127, 255));
}

void lab4(){
	draw_character((WIDTH-CHAR_WIDTH)/2, (HEIGHT-CHAR_HEIGHT)/2, 'Z', 0);
}

void lab5(){
	//print_string("Fourscore and\tseven years ago, our\tforefathers brought forth upon this continent");
	
	char* bigstring = "When in the course of human. No.\f"
    "We the people of. Oh crud.\f"
    "|       |       |       |       |       |       |       |       |       |       |\n"
    "Fourscore and\tseven years ago, our\tforefathers brought forth upon this continent\n"
    "a new \tNATION conceived in libertarian\b\b\b\b\by and dedicated to\tthe\tNOTION\tthat\n\n"
    "all men\b\b\bpeople are created equal.\n\n\n"
    "Now\n\twe\n\t\tare\n\t\t\tengaged.....................in...............a.........large,"
    "egregious, massive, overbearing " 
    "grate\b\b\beat civil strife, \t\t\t\t\t\ttesting whether that "
    "nation or fooby"
    "any\b\b\b\b\b\b\b\bany "
    "nation can so endure.";
	
	print_string(bigstring);
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

void draw_character(int x, int y, char character, char emphasis){
	int c = (int) character;
	short fcolor = COLOR16(255,255,255);
	short bgcolor = COLOR16(0,0,0);
	short emph_color = COLOR16(255, 0, 0);
	short text_color = ((short) emphasis > 0) ? emph_color : fcolor;
	
	for(int i=0;i<CHAR_HEIGHT;i++){
		for(int j=0;j<CHAR_WIDTH;j++){
			if(font_data[c][i] & (1 << (CHAR_WIDTH - j))){
				setpixel(x+j,y+i, text_color);
			}else{
				setpixel(x+j,y+i, bgcolor);
			}
		}
	}
}

void console_putc(char c){
	static int column = 0;
	static int row = 0;
	static char emphasis = 0;
	if(c == '\001'){
		emphasis = 1;
	}else if(c == '\002'){
		emphasis = 0;
	}else if(c == '\n'){//new line
		column = 0;
		row++;
	}else if(c == '\b'){//backspace
		if(!(row == 0 && column == 0)){
			column--;
			if(column < 0){
				row--;
				column = COL_MAX;
			}
			draw_character(column*CHAR_WIDTH, row*CHAR_HEIGHT, ' ', 0);
		}
		
	}else if (c == '\t'){//tab character
		column >>= 3;
		column <<= 3;
		column += 8;
		if(column * CHAR_WIDTH > WIDTH){
			column = 0;
			row++;
		}
	}else if(c == '\f'){//clear screen
		column = 0;
		row = 0;
		clear_screen();
	}else{
		draw_character(column*CHAR_WIDTH, row*CHAR_HEIGHT, c, emphasis);
		column++;
		if(column*CHAR_WIDTH > WIDTH){
			column = 0;
			row++;
		}
	}
	if(row > MAX_ROWS){
		kmemcpy((void*)framebuffer, (void*)framebuffer+CHAR_HEIGHT*WIDTH*2, WIDTH*HEIGHT*2-WIDTH*CHAR_HEIGHT*2);
		kmemset((void*)framebuffer + (WIDTH*HEIGHT*2) - (WIDTH*CHAR_HEIGHT*2), 0, WIDTH*CHAR_HEIGHT*2);
		row--;
		column = 0;
	}
	debugout(c);
}

void print_string(char *string){
	while (*string){
		console_putc(*string++);
	}
}

void clear_screen(){
	short color = COLOR16(0, 0, 0);
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			setpixel(j, i, color);
		}
	}
}
