#ifndef QIUNIX_ASM_H_
#define QIUNIX_ASM_H_

// 常用的内嵌汇编语句
// 注意，为避免与int关键字同名，int指令声明为intr
#define nop() __asm__ volatile("nop" ::)
#define sti() __asm__ volatile("sti" ::)
#define cli() __asm__ volatile("cli" ::)
#define cld() __asm__ volatile("cld" ::)
#define std() __asm__ volatile("std" ::)
#define hlt() __asm__ volatile("hlt" ::)
#define ud2() __asm__ volatile("ud2" ::)
#define ret() __asm__ volatile("ret" ::)
#define iret() __asm__ volatile("iret" ::)
#define into() __asm__ volatile("into" ::)
#define bound(dest, src) __asm__ volatile("boundl %0, (%1)" ::"r"(dest), "g"(src))
#define intr(n) __asm__ volatile("int %0" ::"g"(n))
#define int3() __asm__ volatile("int3" ::)
#define wait() __asm__ volatile("wait" ::)
#define fwait() __asm__ volatile("fwait" ::)
#define pushf() __asm__ volatile("pushfl" ::)
#define popf() __asm__ volatile("popfl" ::)
#define pushb(value) __asm__ volatile("pushb %0" ::"g"(value))
#define pushw(value) __asm__ volatile("pushw %0" ::"g"(value))
#define pushl(value) __asm__ volatile("pushl %0" ::"g"(value))
#define popb(value) __asm__ volatile("popb %0"     \
                                     : "=g"(value) \
                                     :)
#define popw(value) __asm__ volatile("popw %0"     \
                                     : "=g"(value) \
                                     :)
#define popl(value) __asm__ volatile("popl %0"     \
                                     : "=g"(value) \
                                     :)

#define in_byte(port) ({             \
    unsigned char _v;                \
    __asm__ volatile("inb %%dx,%%al" \
                     : "=a"(_v)      \
                     : "d"(port));   \
    _v;                              \
})
#define out_byte(port, value) __asm__("outb %%al,%%dx" ::"a"(value), "d"(port))

#define in_word(port) ({             \
    unsigned short _v;               \
    __asm__ volatile("inw %%dx,%%ax" \
                     : "=a"(_v)      \
                     : "d"(port));   \
    _v;                              \
})
#define out_word(port, value) __asm__("outw %%ax,%%dx" ::"a"(value), "d"(port))

#define lgdt(gdt_ptr) __asm__ volatile("lgdt (%0)" ::"g"(gdt_ptr))
#define lidt(idt_ptr) __asm__ volatile("lidt (%0)" ::"g"(idt_ptr))
#define ltr(tss_sel) __asm__ volatile("ltr  %w0" ::"r"(tss_sel))
#define lldt(ldt_sel) __asm__ volatile("lldt %w0" ::"r"(ldt_sel))

#define sgdt(gdt_ptr) __asm__ volatile("sgdt (%0)" ::"g"(gdt_ptr))
#define sidt(idt_ptr) __asm__ volatile("sidt (%0)" ::"g"(idt_ptr))
#define str(tss_sel) __asm__ volatile("str %w0" ::"g"(tss_sel))
#define sldt(ldt_sel) __asm__ volatile("sldt %w0" ::"g"(ldt_sel))

#define get_ds() ({                    \
    unsigned short _v;                 \
    __asm__ volatile("movw %%ds, %%ax" \
                     : "=a"(_v)        \
                     :);               \
    _v;                                \
})

#define get_es() ({                    \
    unsigned short _v;                 \
    __asm__ volatile("movw %%es, %%ax" \
                     : "=a"(_v)        \
                     :);               \
    _v;                                \
})

#define get_fs() ({                    \
    unsigned short _v;                 \
    __asm__ volatile("movw %%fs, %%ax" \
                     : "=a"(_v)        \
                     :);               \
    _v;                                \
})

#define get_gs() ({                    \
    unsigned short _v;                 \
    __asm__ volatile("movw %%gs, %%ax" \
                     : "=a"(_v)        \
                     :);               \
    _v;                                \
})

#define get_ss() ({                    \
    unsigned short _v;                 \
    __asm__ volatile("movw %%ss, %%ax" \
                     : "=a"(_v)        \
                     :);               \
    _v;                                \
})

#define set_ds(value) __asm__ volatile("movw %w0, %%ds" ::"a"(value))
#define set_es(value) __asm__ volatile("movw %w0, %%es" ::"a"(value))
#define set_fs(value) __asm__ volatile("movw %w0, %%fs" ::"a"(value))
#define set_gs(value) __asm__ volatile("movw %w0, %%gs" ::"a"(value))
#define set_ss(value) __asm__ volatile("movw %w0, %%ss" ::"a"(value))
#define set_esp(value) __asm__ volatile("movl %0, %%esp" ::"a"(value))

#define get_eflags() ({                \
    unsigned int _v;                   \
    __asm__ volatile("pushfl; popl %0" \
                     : "=g"(_v)        \
                     :);               \
    _v;                                \
})

#define set_eflags(flags) __asm__ volatile("pushl %0; popf" ::"g"(flags))

#define ljmp(fptr) __asm__ volatile("ljmp *(%0)" ::"g"(fptr))

#endif // QIUNIX_ASM_H_