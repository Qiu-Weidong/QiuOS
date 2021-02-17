#include "proto.h"
#include "type.h"
#include "asm.h"
#include "const.h"

extern uint64_t gdt[GDT_SIZE];
extern uint64_t idt[IDT_SIZE];

public void set_seg_desc(uint64_t * descriptor, uint32_t base, uint32_t limit,uint16_t attr)
{
    *((uint32_t *)descriptor) = (base << 16) | (limit & 0xffff);
    *((uint32_t *)descriptor+1) = ((base >> 16) & 0xff) 
        | (((attr & 0xf0ff) |((limit >> 8) & 0x0f00)) << 8)
        |(base & 0xff000000);
}

public void set_gate(uint64_t * gate, uint32_t offset, uint16_t selector, uint8_t dcount, uint8_t attr)
{
    *((uint32_t *)gate) = ((uint32_t)selector << 16) | (offset & 0xffff) ;
    *((uint32_t *)gate+1) = offset & 0xffff0000 | ((uint16_t)attr << 8) | (dcount & 0x1f);
}

private void init_gdt()
{
    set_seg_desc(gdt+0,0,0,0); 
    set_seg_desc(gdt+1,0,0xfffff,DA_DPL0|DA_CR|DA_32|DA_LIMIT_4K);     // 可执行代码段
    set_seg_desc(gdt+2,0,0xfffff,DA_32|DA_DPL0|DA_DRW|DA_LIMIT_4K);    // 内核数据段
    set_seg_desc(gdt+3,0xb8000,0xffff,DA_DRW|DA_DPL3);                 // 显存段
    uint8_t gdt_ptr[6];
    *((uint16_t *)gdt_ptr) = 4*8 - 1;
    *((uint32_t *)(gdt_ptr+2)) = (uint32_t)gdt;
    lgdt(gdt_ptr);
}

public void init_kernel()
{
    init_gdt();
    init_idt();

    // uint16_t selector_dummy = (0 << 3) + SA_RPL0 + SA_TIG;
    // uint16_t selector_code  = (1 << 3) + SA_RPL0 + SA_TIG;
    // uint16_t selector_data  = (2 << 3) + SA_RPL0 + SA_TIG;
    // uint16_t selector_video = (3 << 3) + SA_RPL0 + SA_TIG;
    

}
