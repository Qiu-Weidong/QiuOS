#ifndef QIUOS_TYPE_H_
#define QIUOS_TYPE_H_

#define true 1
#define false 0

typedef unsigned int        bool;
typedef unsigned int        size_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short        int16_t;
typedef unsigned int        uint32_t;
typedef signed int          int32_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;

typedef	void (*function)	();

typedef struct 
{
    uint16_t back_link;                      // 上一个任务链接
    uint32_t esp0;                           // 0特权级的栈指针
    uint16_t ss0;                            // 0特权级的栈段描述符
    uint32_t esp1;
    uint16_t ss1;
    uint32_t esp2;
    uint16_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint16_t es;
    uint16_t cs;
    uint16_t ss;
    uint16_t ds;
    uint16_t fs;
    uint16_t gs;
    uint16_t ldt;
    uint16_t trace, bitmap;
}TSS;

#endif // QIUOS_TYPE_H_