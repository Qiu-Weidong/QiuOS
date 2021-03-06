#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/stdlib.h"
#include "const.h"


size_t write(filedesc_t , void *, size_t);

public
size_t printf(const char *format, ...)
{
    size_t size;
    char buf[256];
    va_list args;
    va_start(args, format);
    size = vsnprintf(buf, 256, format, args);
    va_end(args);

    size = write(STDOUT_FILENO, buf,size);
    return size;
}

public
size_t vsnprintf(char *buffer, size_t size, const char *format, va_list list)
{
    char *p = buffer;
    char tmp[256];

    va_list next_arg = list;
    union 
    {
        signed char s8;
        signed short s16;
        signed int s32;
        signed long long s64;
        unsigned char u8;
        unsigned short u16;
        unsigned int u32;
        unsigned long long u64;
        float f32;
        double f64;
        char * ptr;
    }type;
    

    for (; *format; format++)
    {
        if(p - buffer >= size) return size;
        if (*format != '%')
        {
            *p++ = *format;
            continue;
        }

        format++;
        // 已经读入了一个%
        switch (*format)
        {
        case '%':
            *p++ = '%';
            break;
        case 'x':
            type.s32 = va_arg(next_arg, int);
            p = itoa(p,type.s32,16);
            break;
        case 'd':
            type.s32 = va_arg(next_arg, int);
            p = itoa(p,type.s32,10);
            break;
        case 'o':
            type.s32 = va_arg(next_arg, int);
            p = itoa(p,type.s32,8);
            break;
        case 'c':
            type.s8 = va_arg(next_arg, int);
            *p++ = type.s8;
            break;
        case 's':
            type.ptr = va_arg(next_arg, char *);
            p = strcpy(p,type.ptr);
            break;
        case 'f':
            break;
        
        case 'u':
        case 'l':
            break;
        default:
            // error
            break;
        }
    }
    *p = '\0';
    return p - buffer;
}

public
size_t snprintf(char * buffer, size_t size, const char * fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args,fmt);
    size = vsnprintf(buf,size,fmt,args);
    va_end(args);

    size = write(STDOUT_FILENO, buf, size);
    return size;
}
public
void panic(const char *fmt,...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    size_t size = vsnprintf(buf, 256, fmt, args);
    va_end(args);
    buf[size] = '\0';
    printf("%c >>panic<< %s", MAG_CH_PANIC, buf);
    for(;;);
}

