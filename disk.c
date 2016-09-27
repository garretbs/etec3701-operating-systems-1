#include "disk.h"

void disk_init(){
	*POWER = 3; //00 = off, 11 = on
	*CLOCK = 8;
	*CMD = 8 | (1<<10);
	
	do{
		*CMD = 55 | (1<<10) | (1<<6);
		*ARG = 0xffffffff;
		*CMD = 41 | (1<<10) | (1<<6);
	}while(isBusy());
	
	*CMD = 2 | (1<<10) | (1<<6) | (1<<7);
	*CMD = 3 | (1<<10) | (1<<6);
	unsigned relative_address = *RESPONSE;
	*ARG = relative_address;
	*CMD = 7 | (1<<10) | (1<<6);
}

int isBusy(){
    //return busy bit
    return *STATUS & (1<<24); 
}

void disk_read_sector(unsigned sector, void* datablock){
	*DATA_LENGTH = 512;
	*DATA_TIMER = 100;
	*DATA_CONTROL = 1 | (1<<1) | (9<<4);
	
	*ARG = 512*sector;
	*CLEAR=0x3ff;
	*CMD = 17 | (1<<10) | (1<<6);
	
	unsigned* p = datablock;
	
	int k;
	for(k=0;k<128;++k){
		while( !(*STATUS & (1<<21)) )
			;	
		*CLEAR = 0x3ff;
		unsigned v = *DATA_FIFO;
		*p++ = v;
	}
}

void disk_write_sector(unsigned sector, void* datablock){
	*DATA_LENGTH = 512; 
	*DATA_TIMER = 100;
	*DATA_CONTROL = (1<<0) | (9<<4);     
	*ARG = 512*sector;     
	*CLEAR=0x3ff; //clear status flags
	//do the write
	*CMD = 24 | (1<<10) | (1<<6);
	
	unsigned* p = datablock;  //data to write
	int k;
	for(k=0;k<128;++k){
		//wait until buffer is empty
		while( (*STATUS & (1<<20)) )
			;
		*CLEAR = 0x3ff;
		*DATA_FIFO = *p;
		p++;
	}
}


