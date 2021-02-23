#ifndef QIUOS_CTYPE_H_
#define QIUOS_CTYPE_H_

#include "type.h"
public 
bool_t isalnum(char c);

public 
bool_t isalpha(char c);

public
bool_t iscntrl(char c);

public 
bool_t isdigit(char c);

public 
bool_t isgraph(char c);

public 
bool_t islower(char c);

public
bool_t isupper(char c);

public 
bool_t isprint(char c);

public 
bool_t ispunct(char c);

public 
bool_t isascii(char c);

public
bool_t isspace(char c);

public 
char toascii(char c);

public 
char tolower(char c);

public 
char toupper(char c);

#endif // QIUOS_CTYPE_H_