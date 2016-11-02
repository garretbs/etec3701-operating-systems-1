
#include "kprintf.h"

#define MMCP_START ((volatile unsigned*) 0x1c000000 ) 
#define POWER ( MMCP_START)
#define CLOCK (MMCP_START+1)
#define ARG (MMCP_START+2)
#define CMD (MMCP_START+3)
#define RESPONSE_CMD (MMCP_START+4)
#define RESPONSE (MMCP_START+5)
#define DATA_TIMER (MMCP_START+9)
#define DATA_LENGTH (MMCP_START+10)
#define DATA_CONTROL (MMCP_START+11)
#define DATA_COUNTER (MMCP_START+12)
#define STATUS (MMCP_START+13)
#define CLEAR (MMCP_START+14)
#define INTERRUPT0_MASK (MMCP_START+15)
#define INTERRUPT1_MASK (MMCP_START+16)
#define SELECT (MMCP_START+17)
#define FIFO_COUNT (MMCP_START+18)
#define DATA_FIFO (MMCP_START+32)

#define BLOCK_SIZE 8

int isBusy(){
    //return busy bit
    return *STATUS & (1<<24) ; 
}

void disk_init(){
    
    //power up the mmc
    *POWER = 3;
    
    //enable clock
    *CLOCK=8;
    
    //reset
    *CMD = (1<<10);

    //the mmc is now in state idle_state, mode card_identification_mode

    //activate 3.3v parts. QEMU doesn't seem to care, but 
    //it does have some logic for handling this.
    *CMD = 8 | (1<<10);

    //we must repeat these until the card is not busy
    do{
        //55 = app specific command is next
        *CMD = 55 | (1<<10) | (1<<6);
        
        if( RESPONSE[0] != 0x120 )
            kprintf("Uh oh.");
            
        //set argument
        *ARG = 0xffffffff; 	//delay. Not relevant, but must be > 0
        *CMD = 41 | (1<<10) | (1<<6);        //41 = send_op_cond 
        
    } while( isBusy() );
    
    //we are now in ready state, card identification mode
    
    //Request the MMC to send its CardID
    //move to identification state. This gives a long response.
    *CMD = 2 | (1<<10) | (1<<6) | (1<<7);
   
    //we are now in identification state
    //we can get to standby state by using cmd3 Send relative address
    *CMD = 3 | (1<<10) | (1<<6) ;

    unsigned relative_address = RESPONSE[0];
    
    //select the card. Docs seem to indicate there
    //is no response from this one. But Qemu
    //seems to think there is.
    *ARG = relative_address;
    *CMD = 7 | (1<<10) | (1<<6); 
    
    //could get interrupts: base+MASK0 = (1<<21) | (1<<18) for rx avail/tx empty
}


void disk_read_sector(unsigned sector, char* datablock){

    //time to read a block
    *DATA_LENGTH = 512;
    *DATA_TIMER = 100;	//timeout. What should this be?
    
    //bit 0: 1=data transfer enabled
    //bit 1: 1=read, 0=write
    //bit 2: 0=block, 1=stream
    //bit 3: 0=no dma, 1=dma
    //bits 4-7: block size: Power of 2. Ex: 11=2**11
    *DATA_CONTROL = (1<<0) | (1<<1) | (9<<4);
    
    //looks like qemu emulates SD, not SDHC.
    //block number. Or is it byte number? Depends on SD vs SDHC.
    //Must be multiple of 512 if byte number.
    *ARG = 512*sector;     
    *CLEAR=0x3ff; //clear status flags
    *CMD = 17 | (1<<10) | (1<<6);  //read!
    
    unsigned* p = (unsigned*) datablock;
    int k;
    
    //get 4 bytes at a time...
    for(k=0;k<128;++k){
        //wait for data to be ready
        while( (*STATUS & (1<<19)) )
            ;
        *CLEAR = 0x3ff;
        unsigned v = DATA_FIFO[0];
        *p++ = v;
    }

}
    
void disk_write_sector(unsigned sector, const char* datablock){
    *DATA_LENGTH = 512; //data length
    *DATA_TIMER = 100;   //timeout. What should this be?
    *DATA_CONTROL = (1<<0) | (0<<1) | (9<<4);
    *ARG = 512*sector;     
    *CLEAR=0x3ff; //clear status flags
    *CMD = 24 | (1<<10) | (1<<6); 	//do the write
    unsigned* p = (unsigned*) datablock;
    int k;
    for(k=0;k<128;++k){
        //wait for transmit fifo to be empty
        while( (*STATUS & (1<<20)) )
            ;
        *CLEAR = 0x3ff;
        DATA_FIFO[0] = *p;
        p++;
    }
}

void read_block(unsigned blocknum, void* p){
	for(int i=0;i<BLOCK_SIZE;i++){//8 sectors per block (4kB/512B = 8)
		disk_read_sector(blocknum*BLOCK_SIZE + i, p+i*512);
	}
}