#include "disk.h"

struct File{
    int in_use;
    struct Inode inode;
    int offset;
    int flags;   //0=read, 1=write, 2=r/w
};

int file_open(const char* filename, int open_mode);

int file_close(int descriptor);

int file_read(int fd, void* buffer, int count);

int file_write(int fd, const void* buffer, int count);

int file_seek(int fd, int offset, int whence);

short scan_root_for_file(const char* filename);

void load_inode(int fd, short inode_num);