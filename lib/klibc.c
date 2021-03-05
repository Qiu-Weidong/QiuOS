#include "proto.h"
#include "type.h"
#include "gdt.h"
#include "interrupt.h"
#include "const.h"
#include "proc.h"
#include "../include/stdio.h"

public
uint64_t make_desc(uint32_t base, uint32_t limit, uint16_t attr)
{
    uint32_t desc_low, desc_high;
    desc_low = (base << 16) | (limit & 0xffff);
    desc_high = ((base >> 16) & 0xff) | (((attr & 0xf0ff) | ((limit >> 8) & 0x0f00)) << 8) |
                (base & 0xff000000);
    return ((uint64_t)desc_high << 32) | desc_low;
}
public
uint64_t make_seg_desc(uint32_t base, uint32_t limit, uint16_t attr)
{
    attr |= 0x90;
    return make_desc(base, limit, attr);
}
public
uint64_t make_ldt_desc(uint32_t base, uint32_t limit, uint16_t attr)
{
    attr = (attr & 0xffe0) | DA_LDT;
    return make_desc(base, limit, attr);
}
public
uint64_t make_tss_desc(uint32_t base, uint32_t limit, uint16_t attr)
{
    attr = (attr & 0xf0e0) | DA_386TSS;
    return make_desc(base, limit, attr);
}

public
uint64_t make_gate(intr_stub func, selector_t selector, uint8_t dcount, uint8_t attr)
{
    uint32_t gate_low, gate_high;
    gate_low = ((uint32_t)selector << 16) | ((uint32_t)func & 0xffff);
    gate_high = (uint32_t)func & 0xffff0000 | ((uint16_t)attr << 8) | (dcount & 0x1f);
    return ((uint64_t)gate_high << 32) | gate_low;
}

public
uint64_t make_call_gate(intr_stub func, selector_t selector, uint8_t dcount, uint8_t dpl)
{
    uint8_t attr = ((dpl % 4) << 5) | DA_386CGate;
    return make_gate(func, selector, dcount, attr);
}
public
uint64_t make_trap_gate(intr_stub func, selector_t selector, uint8_t dpl)
{
    uint8_t attr = ((dpl % 4) << 5) | DA_386TGate;
    return make_gate(func, selector, 0, attr);
}
public
uint64_t make_intr_gate(intr_stub func, selector_t selector, uint8_t dpl)
{
    uint8_t attr = ((dpl % 4) << 5) | DA_386IGate;
    return make_gate(func, selector, 0, attr);
}
public
uint64_t make_task_gate(selector_t selector, uint8_t dpl)
{
    uint8_t attr = ((dpl % 4) << 5) | DA_TaskGate;
    return make_gate(nullptr, selector, 0, attr);
}

void assertion_failure(char *exp, char *file, char *base_file, int line)
{
    printf("%c  assert(%s) failed: file: %s, base_file: %s, ln%d",
	       MAG_CH_ASSERT,
	       exp, file, base_file, line);
    for(;;);
}
