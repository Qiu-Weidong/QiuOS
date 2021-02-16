#ifndef QIUOS_PROTO_H_
#define QIUOS_PROTO_H_
#include "const.h"
#include "type.h"

PUBLIC void putchar(int c);
PUBLIC int puts(const char * str);

PUBLIC void setDescriptor(u64 * descriptor, u32 base, u32 limit,u16 attr);
PUBLIC void setGate(u64 * gate, u32 offset, u16 selector, u8 dcount, u8 attr);

PUBLIC void init_interupt_vector();
PUBLIC void init_kernel();

#endif // QIUOS_PROTO_H_