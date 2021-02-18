#ifndef QIUOS_PROTO_H_
#define QIUOS_PROTO_H_
#include "const.h"
#include "type.h"

public uint64_t make_desc(uint32_t base, uint32_t limit, uint16_t attr);
public uint64_t make_seg_desc(uint32_t base, uint32_t limit, uint16_t attr);
public uint64_t make_tss_desc(uint32_t base, uint32_t limit, uint16_t attr);
public uint64_t make_ldt_desc(uint32_t base, uint32_t limit, uint16_t attr);

public uint64_t make_gate(function func, uint16_t selector, uint8_t dcount, uint8_t attr);
public uint64_t make_call_gate(function func, uint16_t selector,uint8_t dcount, uint8_t dpl);
public uint64_t make_trap_gate(function func, uint16_t selector,uint8_t dpl);
public uint64_t make_intr_gate(function func, uint16_t selector,uint8_t dpl);
public uint64_t make_task_gate(uint16_t selector, uint8_t dpl);


public void init_idt();
public void init_kernel();
public bool is_intr_on();

#endif // QIUOS_PROTO_H_