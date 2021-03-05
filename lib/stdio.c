#include "../include/stdio.h"

int printf(const char *format, ...)
{
    size_t size;
    char buf[256];
    va_list args;
    va_start(args, format);
    size = vsnprintf(buf, 256, format, args);
    va_end(args);

    return size;
}

int vsnprintf(char *buffer, size_t size, const char *format, va_list list)
{
    char *p = buffer;
    char tmp[256];

    va_list next_arg = list;
    int n;

    for (; *format; format++)
    {
        if(p - buffer >= size) return size;
        if (*format != '%')
        {
            *p++ = *format;
            continue;
        }

        format++;
        switch (*format)
        {
        case '%':
            *p++ = '%';
            break;
        case 'x':
            n = va_arg(next_arg, int);
            break;
        case 'd':
            break;
        case 'c':
            break;
        case 's':
            break;
        case 'f':
            break;
        case 'l':
            break;
        default:
            break;
        }
    }
}
