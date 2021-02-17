#ifndef QIUOS_IO_H_
#define QIUOS_IO_H_
#include "const.h"

public void putchar(int c);
public int puts(const char * str);
public void putdec(int n);
public void puthex(int n);
public void putoct(int n);
public void putln();

public int getchar();
public int getline(char * buffer);

#endif // QIUOS_IO_H_