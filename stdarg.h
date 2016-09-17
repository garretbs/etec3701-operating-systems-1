#pragma once

typedef unsigned short u16;

typedef struct x{
	void* current;
}y;

typedef struct va_list_{
	char* current;
}va_list;

#define va_start(v, arg)\
	v.current = (((char*)&arg)+sizeof(arg))
#define va_end(v)
#define va_arg(v, t) *((t*)va_arg_(&v, sizeof(t)))
	
	
void* va_arg_(va_list *v, int sz){
	//helper function for va_arg macro
	
	void *temp = v->current;
	v->current+=sz;
	return temp;
	//return v;
}

//int kprintf(char *fmt, ...){
	//va_list v;
	//va_start(v, fmt);
	
	// while(1){
		// if(percent.d){
			// int x;
			// x = va_arg(v, int);
			//break;
		// }else if(percent.f){
			// double f;
			// f = va_arg(v, double);
			//break;
		// }
	// }
	//va_end(v);
	//return 0;
//}



