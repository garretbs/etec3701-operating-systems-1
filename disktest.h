#include "errno.h"

void disk_test(){
    int i;
    char buf[32];
    int fd;

    fd = file_open("article6.txt",0);
    if( fd < 0 ){
        kprintf("Could not open\n");
        return;
    }
    
    kprintf("Printing all of article 6:\n");
    int nr;
    while(1){
        nr = file_read(fd,buf,23);
        if( nr < 0 || nr > 23){
            kprintf("Error: nr=%d\n",nr);
            return;
        }
        if( nr == 0 )
            break;
        for(i=0;i<nr;++i){
            kprintf("%c",buf[i]);
        }
    }

    file_close(fd);
    kprintf("~~~[EOF]~~~\n");
    fd = file_open("article5.txt",0);
    if(fd<0){
        kprintf("Could not open\n");
        return;
    }
    
    int rv;
    rv = file_seek(fd,17,SEEK_SET);
    if( rv < 0 ){
        kprintf("Seek 0 failed\n");
        return;
    }
    
    rv = file_read(fd,buf,32);
    if( rv <= 0 || rv > 32 ){
        kprintf("Read failed\n");
        return;
    }
    kprintf("Expected: Congress, whenever two thirds of\n");
    kprintf("Actual:   %.*s\n",rv,buf);


    rv = file_seek(fd,-100000,SEEK_END);
    if( rv == 0 ){
        kprintf("Seek succeeded, but it should not have\n");
        return;
    }
    rv = file_seek(fd,-100000,SEEK_CUR);
    if( rv >= 0 ){
        kprintf("Seek succeeded, but it should not have\n");
        return;
    }
    
    rv = file_seek(fd,-8,SEEK_CUR);
    if( rv < 0 ){
        kprintf("Seek 1 failed\n");
        return;
    }
    
    rv = file_read(fd,buf,13);
    if( rv <= 0 || rv > 13 ){
        kprintf("Read failed\n");
        return;
    }
    kprintf("Expected: hirds of both\n");
    kprintf("Actual:   %.*s\n",rv,buf);
    
    rv = file_seek(fd,-11,SEEK_END);
    if( rv < 0 ){
        kprintf("Seek failed, but it should not have\n");
        return;
    }
    
    rv = file_read(fd,buf,32);
    if( rv <= 0 || rv > 11 ){
        kprintf("Read failed: Got %d\n",rv);
        return;
    }

    kprintf("Expected: the Senate.\n");
    kprintf("Actual:   %.*s\n",rv,buf);
    
    rv = file_seek(fd,0,SEEK_END);
    if( rv < 0 ){
        kprintf("Seek failed, but it should not have\n");
        return;
    }
    rv = file_read(fd,buf,32);
    if( rv != 0 ){
        kprintf("Read at EOF should have returned zero.\n");
        return;
    }

    rv = file_seek(fd,100,SEEK_END);
    if( rv < 0 ){
        kprintf("Seek 2 failed\n");
        return;
    }
    rv = file_read(fd,buf,32);
    if( rv != 0 ){
        kprintf("EOF failed\n");
        return;
    }

    
    if( 0 != file_close(fd) ){
        kprintf("Could not close\n");
        return;
    }
        
    kprintf("All OK");
}
