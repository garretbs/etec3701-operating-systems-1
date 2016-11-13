#include "file.h"
#include "errno.h"
#include "util.h"

#include "kprintf.h"

#define MAX_FILES 16
static struct File file_table[MAX_FILES];

int file_open(const char* filename, int open_mode){
	int idx = -1;
	for(int i=0;i<MAX_FILES;i++){
		if((file_table)[i].in_use == 0){
			idx = i;
			break;
		}
	}
	
	if(idx == -1)
		return -EMFILE;
	short inode_num = scan_root_for_file(filename);
	if(inode_num < 0){
		return -ENOENT; //File not found
	}
	
	file_table[idx].in_use = 1;
	load_inode(idx, inode_num);
	file_table[idx].offset = 0;
	file_table[idx].flags = open_mode;
	return idx;
}

int file_close(int descriptor){
	if(descriptor < 0 || descriptor >= MAX_FILES)
		return -EINVAL;
	if(file_table[descriptor].in_use == 0)
		return -EINVAL;
	file_table[descriptor].in_use = 0;
	return SUCCESS;
}

int file_read(int fd, void* buffer, int count){
	if(fd < 0 || fd >= MAX_FILES)
		return -EINVAL;
	
	struct File* fp = &file_table[fd];
	
	if(fp->in_use == 0) //file not in use
		return -EINVAL;
	if(count <= 0)
		return SUCCESS;
	if(fp->offset >= fp->inode.size) //end of file
		return SUCCESS;
	
	int bi = kdiv(fp->offset, BYTES_PER_BLOCK); //blocks in file
	char block_buffer[4096];
	//inode.direct[bi] is the block number into the file
	if(bi < 12){
		//direct
		read_block(fp->inode.direct[bi], block_buffer);
	}
	else{
		bi -= 12;
		unsigned int U[1024];
		if(bi < 1024){
			//indirect
			read_block(fp->inode.indirect, U);
			read_block(U[bi], block_buffer);
		}else{
			bi -= 1024;
			int oi;
			int ii;
			if(bi < 0x100000){ //bi < 1024*1024
				//double indirect
				oi = bi >> 10;
				ii = bi & 1023;
				read_block(fp->inode.doubleindirect, U);
				read_block(U[oi], U);
				read_block(U[ii], block_buffer);
			}else{
				bi -= 0x100000; //bi -= 1024*1024
				//triple indirect
				int ro = bi >> 20;
				oi = (bi>>10) & 0x3ff;
				ii = bi & 0x3ff;
				read_block(fp->inode.tripleindirect, U);
				read_block(U[ro], U);
				read_block(U[oi], U);
				read_block(U[ii], block_buffer);
			}
		}
	}
	
	int bo = kmod(fp->offset, BYTES_PER_BLOCK);//buffer offset
	
	unsigned minimum = BYTES_PER_BLOCK - bo; //data remaining in block
	minimum = (minimum > count) ? count : minimum;//data requested
	unsigned remaining_in_f = fp->inode.size - fp->offset;//data remaining in file
	minimum = (minimum > remaining_in_f) ? remaining_in_f : minimum;
	kmemcpy(buffer, block_buffer + bo , minimum);
	
	fp->offset += minimum;
	return minimum;
}

int file_write(int fd, const void* buffer, int count){
	return -ENOSYS;
}

int file_seek(int fd, int offs, int whence){
	//verify fd is valid
	if(fd < 0)
		return -EINVAL;
	
	struct File* fp = &file_table[fd];
	
	if(fp->in_use == 0) //file not in use
		return -EINVAL;
	if(fp->offset >= fp->inode.size) //end of file
		return SUCCESS;
		
	if(whence == SEEK_SET){
		if(offs < 0){
			return -EINVAL;
		}else{
			fp->offset = offs; //Offset is interpreted as relative to start of file
			return SUCCESS;
		}
	}else if(whence == SEEK_CUR){
		if((int) (fp->offset + offs) < 0){
			return -EINVAL;
		}else{
			fp->offset += offs; //Offset is interpreted as relative to current file position
			return SUCCESS;
		}
	}else if(whence == SEEK_END){
		if((int) (fp->inode.size + offs) < 0){//comparators and unsigned don't mix
			return -EINVAL;
		}else{
			fp->offset = fp->inode.size + offs; //Offset is interpreted as relative to file end
			return SUCCESS;
		}
	}
	return -EINVAL;
}

short scan_root_for_file(const char* filename){	
	char inode_buffer[4096];
	char dir_buffer[4096];
	read_block(4, inode_buffer);
	int ro = 0; //root offset
	
	struct Inode* I = (struct Inode*) inode_buffer;
	struct Inode root_inode;
	kmemcpy(&root_inode, &(I[1]), sizeof(struct Inode));
	
	
	read_block(I[1].direct[0], dir_buffer);
	
	struct DirEntry *d = (struct DirEntry*) dir_buffer;
	while(ro < root_inode.size && d->rec_len > 0){
		if(d->name_len == kstrlen((char*) filename)){
			if(fnamecmp(d->name, (char*) filename) == 0)
				return d->inode;
		}
		ro += d->rec_len;
		d = (struct DirEntry*) (dir_buffer + ro);
	}
	return -ENOENT;//file not found
}

void load_inode(int fd, short inode_num){
	char sb[1024];
	disk_read_sector(2, sb);
	struct Superblock* supablock = (struct Superblock*) sb;	
	
	char buffer[4096];
	int igroup = kdiv((inode_num - 1), supablock->inodes_per_group);
	int bstart = igroup * supablock->blocks_per_group;
	int itstart = bstart + 4;
	int iindex = kmod((inode_num-1), supablock->inodes_per_group);
	read_block(itstart + kdiv(iindex, 32), buffer);
	
	int inodestoskip = kmod(iindex, 32);
	kmemcpy(&(file_table[fd].inode), buffer + inodestoskip*BYTES_PER_INODE, BYTES_PER_INODE);
}

int exec(const char* filename){
	int fd = file_open(filename, 0);
	char *buffer = (char*) 0x400000;//load binary to ram at 0x400000
	while(1){
		int rv = file_read(fd, buffer, 1000); //1000 is arbitrary
		if(rv == 0){
			break;
		}else if(rv < 0){
			return -1;
		}else{
			buffer += rv;
		}
	}
	file_close(fd);
	
	asm volatile(
    //get current processor state register
    "mrs r0,cpsr\n"
    //mask off lower bits
    "and r0,#0xffffffe0\n"
    //set lower bits to 0x10 = user mode
    "orr r0,#0x10\n"
    //set current processor state register
    "msr cpsr,r0\n"
    //update stack pointer
    "mov sp,#0x800000\n"
    //jump to user code
    "mov pc,#0x400000\n"
	);
	return 0;
}