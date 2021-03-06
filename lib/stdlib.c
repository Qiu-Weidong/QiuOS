#include "../include/stdlib.h"

public 
char * itoa(char * buffer, int num, int scale)
{
    char * q = buffer;
    if(scale == 2) *q++ = '0', *q++ = 'b';
    else if(scale == 8) *q++ = '0';
    else if(scale == 16) *q++ = '0', *q++ = 'x';
    else if(scale == 10 && num < 0) *q++ = '-', num = -num;

    char tmp[16];
    char *p = tmp;
    while(num)
    {
        int t = num % scale;
        if(t > 10) *p++ = t - 10 +'A';
        else *p++ = t + '0';
        num /= scale;
    }
    

    while(p!=tmp) *q++ = *--p;
    *q = '\0';
    return q;
}


