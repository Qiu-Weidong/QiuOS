#ifndef QIUOS_GDT_H_
#define QIUOS_GDT_H_
#include "type.h"

#define GDT_SIZE 128
// 描述符的索引
#define	INDEX_DUMMY			0	
#define	INDEX_FLAT_C		1	
#define	INDEX_FLAT_RW		2	
#define	INDEX_VIDEO			3	
#define INDEX_KERNEL_TSS    4

// 选择子
#define	SELECTOR_DUMMY		0x0		
#define	SELECTOR_FLAT_C		0x08		
#define	SELECTOR_FLAT_RW	0x10		
#define	SELECTOR_VIDEO		(0x18|0x3)
#define SELECTOR_KERNEL_TSS 0x20

#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW

//////////////////////////////////////////////////
/// @brief 向gdt中添加描述符，并返回添加的选择子，其RPL等于添加的描述符的dpl
//////////////////////////////////////////////////
public 
selector_t push_desc(uint64_t * gdt,uint64_t desc);

public 
void gdt_init();


#endif // QIUOS_GDT_H_