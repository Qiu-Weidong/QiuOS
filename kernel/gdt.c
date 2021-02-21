#include "gdt.h"
#include "asm.h"
#include "const.h"
#include "proto.h"
#include "tss.h"

uint64_t gdt[GDT_SIZE];
extern task_state_segment kernel_tss;

public
void gdt_init()
{
    gdt[INDEX_DUMMY] = make_desc(0, 0, 0);
    gdt[INDEX_FLAT_C] = make_seg_desc(0, 0xfffff, DA_DPL0 | DA_CR | DA_32 | DA_LIMIT_4K);
    gdt[INDEX_FLAT_RW] = make_seg_desc(0, 0xfffff, DA_32 | DA_DPL0 | DA_DRW | DA_LIMIT_4K);
    gdt[INDEX_VIDEO] = make_seg_desc(0xb8000, 0xffff, DA_DRW | DA_DPL3);
    gdt[INDEX_KERNEL_TSS] = make_tss_desc((uint32_t)(&kernel_tss),sizeof(kernel_tss)-1,DA_DPL0);

    gdt[5] = gdt[INDEX_FLAT_C];
    uint16_t gdt_ptr[3];
    *((uint16_t volatile *)gdt_ptr) = 6 * 8 - 1;
    *((uint32_t volatile *)(gdt_ptr + 1)) = (uint32_t)gdt;
    lgdt(gdt_ptr);

    // 更新除cs外的段寄存器，由于我们新定义的gdt与loader中定义的一样，省略掉似乎也没事
    set_ds(SELECTOR_KERNEL_DS);
    set_es(SELECTOR_KERNEL_DS);
    set_fs(SELECTOR_KERNEL_DS);
    set_ss(SELECTOR_KERNEL_DS);
    set_gs(SELECTOR_VIDEO);
}

public
selector_t gdt_push_back(uint64_t *gdt, uint64_t desc)
{
    uint16_t gdt_ptr[3];
    sgdt(gdt_ptr);
    uint16_t i = gdt_ptr[0] / sizeof(uint64_t);
    gdt[++i] = desc;
    (*(uint16_t volatile *)gdt_ptr) += sizeof(uint64_t);
    lgdt(gdt_ptr);
    uint8_t dpl = (desc >> 45) & 0x3;
    return (i << 3) + dpl + SA_TIG;
}


