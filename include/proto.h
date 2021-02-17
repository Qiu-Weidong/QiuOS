#ifndef QIUOS_PROTO_H_
#define QIUOS_PROTO_H_
#include "const.h"
#include "type.h"

public void putchar(int c);
public int puts(const char * str);

public void set_seg_desc(uint64_t * descriptor, uint32_t base, uint32_t limit,uint16_t attr);
public void set_gate(uint64_t * gate, uint32_t offset, uint16_t selector, uint8_t dcount, uint8_t attr);

public void init_idt();
public void init_kernel();

#endif // QIUOS_PROTO_H_