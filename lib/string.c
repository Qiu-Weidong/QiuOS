#include "../include/string.h"

public
void *memcpy(void *dest, const void *src, size_t size)
{
    unsigned char * _dst = dest;
    while(size--)
        *_dst++ = *(unsigned char *)src++;
    return dest;
}

public 
void *memset(void *dest, int value, size_t size)
{
    int * _dst = dest;
    while(size--)
        *_dst++ = value;
    return dest;
}

