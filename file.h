#include "disk.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define BYTES_PER_INODE 128
#define BYTES_PER_BLOCK 4096

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

int exec(const char* filename);