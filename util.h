void kmemcpy(void *d, void *s, int n);

void kmemset(void *p, char c, int n);

void debugout(char x);

void kdivmod(unsigned numerator, unsigned denominator, unsigned *qp, unsigned *rp);

unsigned kdiv(unsigned numerator, unsigned denominator);

unsigned kmod(unsigned numerator, unsigned denominator);

int kstrcmp(char* str1, char* str2);

int kstrlen(char* str);

int fnamecmp(char* dir, char* fname);