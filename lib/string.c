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

public
size_t strlen(const char *str)
{
    size_t size = 0;
    while(*str++) size++;
    return size;
}

public
char *strcpy(char *dest, const char *src)
{
    while(*src) *dest++ = *src++;
    return dest;
}
