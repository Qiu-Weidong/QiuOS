#include "../include/string.h"

public
void *memcpy(void *dest, const void *src, size_t size)
{
    unsigned char * _dst = dest;
    while(size--)
        *_dst++ = *(unsigned char *)src++;
    return dest;
}



