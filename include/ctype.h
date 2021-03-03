#ifndef QIUOS_CTYPE_H_
#define QIUOS_CTYPE_H_

#include "type.h"

static inline bool_t isalnum(char c)
{
    return isalpha(c) || isdigit(c);
}

static inline bool_t isalpha(char c)
{
    return islower(c) || isupper(c);
}

static inline bool_t iscntrl(char c)
{
    return c >= 0 && c < 32 || c == 127;
}

static inline bool_t isdigit(char c)
{
    return c >= '0' && c <= '9';
}

static inline bool_t isgraph(char c)
{
    return c > 32 && c < 127;
}

static inline bool_t islower(char c)
{
    return c >= 'a' && c <= 'z';
}

static inline bool_t isupper(char c)
{
    return c >= 'A' && c <= 'Z';
}

static inline bool_t isprint(char c)
{
    return c >= 32 && c < 127;
}

static inline bool_t ispunct(char c)
{
    return isprint(c) && !isalnum(c) && !isspace(c);
}

static inline bool_t isspace(char c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' ||
           c == '\t' || c == '\v';
}

static inline bool_t isascii(char c)
{
    return c >= 0 && c < 128;
}

static inline char toascii(char c)
{
    return c & 0x7f;
}

static inline char tolower(char c)
{
    return isupper(c) ? c - 'A' + 'a' : c;
}

static inline char toupper(char c)
{
    return islower(c) ? c - 'a' + 'A' : c;
}

#endif // QIUOS_CTYPE_H_