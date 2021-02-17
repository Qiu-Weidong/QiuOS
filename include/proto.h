#ifndef QIUOS_PROTO_H_
#define QIUOS_PROTO_H_
#include "const.h"
#include "type.h"

public void putchar(int c);
public int puts(const char * str);

public void set_seg_desc(u64 * descriptor, u32 base, u32 limit,u16 attr);
public void set_gate(u64 * gate, u32 offset, u16 selector, u8 dcount, u8 attr);

public void init_idt();
public void init_kernel();

#endif // QIUOS_PROTO_H_