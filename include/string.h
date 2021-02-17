#ifndef QIUOS_STRING_H_
#define QIUOS_STRING_H_
#include "const.h"

public void * memcpy(void * dest, const void * src, unsigned int n);
public void * memset(void * dest, int value, unsigned int n);
public unsigned int strlen(char * str);
public char * strcpy(char * dest, const char * src);
public char * strncpy(char * dest, const char * src, unsigned int n);
public char * strcat(char * dest, const char * src);
public char * strncat(char * dest, const char * src, unsigned int n);
public int strcmp(const char * str1, const char * str2);
public int strncmp(const char * str1, const char * str2);
public char * strchr(char * str, char c);
public char * strstr(char * str, const char * substr);

#endif // QIUOS_STRING_H_