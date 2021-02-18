#include "proto.h"
#include "type.h"
#include "asm.h"
#include "const.h"

extern uint64_t gdt[GDT_SIZE];
extern uint64_t idt[IDT_SIZE];

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
    attr = (attr & 0xffe0) | DA_386TSS;
    return make_desc(base, limit, attr);
}

public
uint64_t make_gate(function func, uint16_t selector, uint8_t dcount, uint8_t attr)
{
    uint32_t gate_low, gate_high;
    gate_low = ((uint32_t)selector << 16) | ((uint32_t)func & 0xffff);
    gate_high = (uint32_t)func & 0xffff0000 | ((uint16_t)attr << 8) | (dcount & 0x1f);
    return ((uint64_t)gate_high << 32) | gate_low;
}

public
uint64_t make_call_gate(function func, uint16_t selector, uint8_t dcount, uint8_t dpl)
{
    uint8_t attr = ((dpl % 3) << 5) | DA_386CGate;
    return make_gate(func, selector, dcount, attr);
}
public
uint64_t make_trap_gate(function func, uint16_t selector, uint8_t dpl)
{
    uint8_t attr = ((dpl % 3) << 5) | DA_386TGate;
    return make_gate(func, selector, 0, attr);
}
public
uint64_t make_intr_gate(function func, uint16_t selector, uint8_t dpl)
{
    uint8_t attr = ((dpl % 3) << 5) | DA_386IGate;
    return make_gate(func, selector, 0, attr);
}
public
uint64_t make_task_gate(uint16_t selector, uint8_t dpl)
{
    uint8_t attr = ((dpl % 3) << 5) | DA_TaskGate;
    return make_gate(NULL, selector, 0, attr);
}

private
void init_gdt()
{
    gdt[0] = make_desc(0,0,0);
    gdt[1] = make_seg_desc(0,0xfffff,DA_DPL0 | DA_CR | DA_32 | DA_LIMIT_4K);
    gdt[2] = make_seg_desc(0,0xfffff,DA_32 | DA_DPL0 | DA_DRW | DA_LIMIT_4K);
    gdt[3] = make_seg_desc(0xb8000, 0xffff,DA_DRW|DA_DPL3);

    uint8_t gdt_ptr[6];
    *((uint16_t *)gdt_ptr) = 4 * 8 - 1;
    *((uint32_t *)(gdt_ptr + 2)) = (uint32_t)gdt;
    lgdt(gdt_ptr);
}

public
void init_kernel()
{
    init_gdt();
    init_idt();
}
