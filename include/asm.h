#ifndef QIUOS_ASM_H_
#define QIUOS_ASM_H_


#define nop() __asm__ volatile("nop"::)
#define sti() __asm__ volatile("sti"::)
#define cli() __asm__ volatile("cli"::)
#define cld() __asm__ volatile("cld"::)
#define hlt() __asm__ volatile("hlt"::)
#define ud2() __asm__ volatile("ud2"::)
#define iret() __asm__ volatile("iret"::)
#define iretd() __asm__ volatile("iretd"::)
#define in_byte(port) ({                                        \
    unsigned char _v;                                           \
    __asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port));    \
    _v;                                                         \
})
#define out_byte(value,port) __asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

#define in_word(port) ({                                        \
    unsigned short _v;                                          \
    __asm__ volatile ("inw %%dx,%%ax":"=a" (_v):"d" (port));    \
    _v;                                                         \
})
#define out_word(value,port) __asm__ ("outw %%ax,%%dx"::"a" (value),"d" (port))

#define lgdt(gdt_ptr) __asm__ volatile("lgdt (%0)"::"g"(gdt_ptr))
#define lidt(idt_ptr) __asm__ volatile("lidt (%0)"::"g"(gdt_ptr))
#define ltr(tss_ptr)  __asm__ volatile("ltr  (%0)"::"r"(tss_ptr))
#define lldt(ldt_ptr) __asm__ volatile("lldt (%0)"::"r"(ldt_ptr))

#endif // QIUOS_ASM_H_