#include "disk.h"
#include "util.h"
#include "kprintf.h"

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

void read_block(unsigned blocknum, void* p){
	for(int i=0;i<8;i++){//8 sectors per block (4kB/512B = 8)
		disk_read_sector(blocknum*8 + i, p+i*512);
	}
}

void print_directories(unsigned inode_num, unsigned depth){
	if(depth == 0){
		char sb[1024];
		disk_read_sector(2, sb);
		struct Superblock* supablock = (struct Superblock*) sb;	
		kprintf("Volume label: %s\n", supablock->volname);
	}
	
	char inode_buffer[4096];
	char dir_buffer[4096];
	read_block(4, inode_buffer);
	int ro = 0; //root offset
	
	struct Inode* I = (struct Inode*) inode_buffer;
	struct Inode root_inode;
	kmemcpy(&root_inode, &(I[inode_num]), sizeof(struct Inode));
	
	
	//read first 4kB of current directory
	read_block(I[inode_num].direct[0], dir_buffer);	
	//root_inode.size tells how many bytes are in the current directory
	
	struct DirEntry *d = (struct DirEntry*) dir_buffer;
	while(ro < root_inode.size && d->rec_len > 0){
		if((depth == 0) || (d->name[0] != '.')){
			for(int i=0;i<depth;++i){
				kprintf("\t");
			}
			kprintf("Inode: %i %*s\n", d->inode, d->name_len, d->name);
		}
		if(((((struct Inode*) I)[d->inode - 1].mode >> 12) == 4) && (d->name[0] != '.')){
			print_directories(d->inode - 1, depth + 1);
		}
		
		ro += d->rec_len;
		d = (struct DirEntry*) (dir_buffer + ro);
	}
	if(depth == 0)
		kprintf("End of root directory.\n");
}
