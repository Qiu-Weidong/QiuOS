#ifndef QIUX_GDT_H_
#define QIUX_GDT_H_
#include "type.h"

#define GDT_SIZE 128
// 描述符的索引
#define	INDEX_DUMMY			0	
#define	INDEX_FLAT_C		1	
#define	INDEX_FLAT_RW		2	
#define	INDEX_VIDEO			3	
#define INDEX_TSS           4
#define INDEX_LDT           5

// 选择子
#define	SEL_DUMMY		    0x0		
#define	SEL_FLAT_C		    0x08		
#define	SEL_FLAT_RW	        0x10		
#define	SEL_VIDEO		    (0x18|0x3)
#define SEL_TSS             0x20
#define SEL_LDT             0x28

#define	SEL_KERNEL_CS	SEL_FLAT_C
#define	SEL_KERNEL_DS	SEL_FLAT_RW

//////////////////////////////////////////////////
/// @brief 向gdt中添加描述符，并返回添加的选择子，其RPL等于添加的描述符的dpl
//////////////////////////////////////////////////
public 
selector_t gdt_push_back(uint64_t * gdt,uint64_t desc);

public 
void gdt_remove_back(uint64_t * gdt);

public 
void gdt_init();

public 
size_t gdt_size();

#endif // QIUX_GDT_H_