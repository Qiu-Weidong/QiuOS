#ifndef QIUX_PROTO_H_
#define QIUX_PROTO_H_
#include "type.h"

public
uint64_t make_desc(uint32_t base, uint32_t limit, uint16_t attr);
public
uint64_t make_seg_desc(uint32_t base, uint32_t limit, uint16_t attr);
public
uint64_t make_tss_desc(uint32_t base, uint32_t limit, uint16_t attr);
public
uint64_t make_ldt_desc(uint32_t base, uint32_t limit, uint16_t attr);

public
uint64_t make_gate(intr_stub func, selector_t selector, uint8_t dcount, uint8_t attr);
public
uint64_t make_call_gate(intr_stub func, selector_t selector, uint8_t dcount, uint8_t dpl);
public
uint64_t make_trap_gate(intr_stub func, selector_t selector, uint8_t dpl);
public
uint64_t make_intr_gate(intr_stub func, selector_t selector, uint8_t dpl);
public
uint64_t make_task_gate(selector_t selector, uint8_t dpl);

#endif // QIUX_PROTO_H_