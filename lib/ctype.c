#include "../include/ctype.h"

public 
bool_t isalnum(char c)
{
    return isalpha(c) || isdigit(c);
}

public 
bool_t isalpha(char c)
{
    return islower(c) || isupper(c);
}

public
bool_t iscntrl(char c)
{
    return c >= 0 && c < 32 || c == 127;
}

public 
bool_t isdigit(char c)
{
    return c >= '0' && c <= '9';
}

public 
bool_t isgraph(char c)
{
    return c > 32 && c < 127;
}

public 
bool_t islower(char c)
{
    return c >= 'a' && c <= 'z';
}

public
bool_t isupper(char c)
{
    return c >= 'A' && c <= 'Z';
}

public 
bool_t isprint(char c)
{
    return c >= 32 && c < 127;
}

public 
bool_t ispunct(char c)
{
    return isprint(c) && !isalnum(c) && ! isspace(c);
}

public 
bool_t isspace(char c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' ||
    c == '\t' || c == '\v';
}

public 
bool_t isascii(char c)
{
    return c >=0 && c < 128;
}

public 
char toascii(char c)
{
    return c & 0x7f;
}

public 
char tolower(char c)
{
    return isupper(c) ? c - 'A'+'a':c;
}

public 
char toupper(char c)
{
    return islower(c) ? c - 'a' + 'A' : c;
}

