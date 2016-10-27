#include "file.h"
#include "errno.h"
#include "util.h"

#define MAX_FILES 16
#define BYTES_PER_INODE 128
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
		return -ENOENT; //file not found
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
	return -ENOSYS;
}

int file_write(int fd, const void* buffer, int count){
	return -ENOSYS;
}

int file_seek(int fd, int offset, int whence){
	return -ENOSYS;
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
	kmemcpy( &(file_table[fd].inode), buffer + inodestoskip*BYTES_PER_INODE, BYTES_PER_INODE);
}
