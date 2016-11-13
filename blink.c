int main(int argc, char* argv[]){
    int i,j;
    volatile unsigned short* p = ((volatile unsigned short*) (((0x07ffffff - 0x75300*2))&~0xf));
    while(1){
		for(i=0;i<0x75300;++i){
			*(p+i) ^= 0x007f;
		}
        for(i=0;i<10000;++i){
            for(j=0;j<10000;++j){
            }
        }
    }
    return 0;
}