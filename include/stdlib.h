#ifndef QIUX_STDLIB_H_
#define QIUX_STDLIB_H_
#include "type.h"

public
int atoi(const char *);

public 
char * itoa(char * , int num, int);

public 
void *qsort(void * array, size_t cnt, size_t size,compare cmp);

public
void * bsearch(const void * key, const void * array, size_t cnt, size_t size,int (*copmare)(const void *,const void *,void *aux),void *aux);



#endif // QIUX_STDLIB_H_