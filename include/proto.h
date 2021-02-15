#ifndef QIUOS_PROTO_H_
#define QIUOS_PROTO_H_
#include "const.h"

PUBLIC void setColor(u8 color);
PUBLIC void putchar(int c);
PUBLIC int puts(const char * str);

PUBLIC u8 read_port(u16 port);
PUBLIC void write_port(u16 port, u8 value);


#endif // QIUOS_PROTO_H_