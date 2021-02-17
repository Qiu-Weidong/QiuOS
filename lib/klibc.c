#include "proto.h"
#include "type.h"
#include "asm.h"
#include "const.h"

extern u64 gdt[GDT_SIZE];
extern u64 idt[IDT_SIZE];

public void set_seg_desc(u64 * descriptor, u32 base, u32 limit,u16 attr)
{
    *((u32 *)descriptor) = (base << 16) | (limit & 0xffff);
    *((u32 *)descriptor+1) = ((base >> 16) & 0xff) 
        | (((attr & 0xf0ff) |((limit >> 8) & 0x0f00)) << 8)
        |(base & 0xff000000);
}

public void set_gate(u64 * gate, u32 offset, u16 selector, u8 dcount, u8 attr)
{
    *((u32 *)gate) = ((u32)selector << 16) | (offset & 0xffff) ;
    *((u32 *)gate+1) = offset & 0xffff0000 | ((u16)attr << 8) | (dcount & 0x1f);
}

private void init_gdt()
{
    set_seg_desc(gdt+0,0,0,0); 
    set_seg_desc(gdt+1,0,0xfffff,DA_DPL0|DA_CR|DA_32|DA_LIMIT_4K);     // 可执行代码段
    set_seg_desc(gdt+2,0,0xfffff,DA_32|DA_DPL0|DA_DRW|DA_LIMIT_4K);    // 内核数据段
    set_seg_desc(gdt+3,0xb8000,0xffff,DA_DRW|DA_DPL3);                 // 显存段
    u8 gdt_ptr[6];
    *((u16 *)gdt_ptr) = 4*8 - 1;
    *((u32 *)(gdt_ptr+2)) = (u32)gdt;
    lgdt(gdt_ptr);
}

public void init_kernel()
{
    init_gdt();
    init_idt();

    // u16 selector_dummy = (0 << 3) + SA_RPL0 + SA_TIG;
    // u16 selector_code  = (1 << 3) + SA_RPL0 + SA_TIG;
    // u16 selector_data  = (2 << 3) + SA_RPL0 + SA_TIG;
    // u16 selector_video = (3 << 3) + SA_RPL0 + SA_TIG;
    

}
