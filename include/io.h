#ifndef QIUOS_IO_H_
#define QIUOS_IO_H_
#include "const.h"
#include "type.h"

public uint8_t set_color(unsigned char color);
public void putchar(int c);
public int puts(const char * str);
public void putdec(unsigned int n);
public void puthex(int n);
public void putoct(int n);
public void putln();

public int getchar();
public int getline(char * buffer);

#endif // QIUOS_IO_H_