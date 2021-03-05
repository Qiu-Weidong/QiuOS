#ifndef QIUOS_TYPE_H_
#define QIUOS_TYPE_H_

#define public
#define private static
#define NULL ((void *)0)
#define nullptr ((void *)0)

#define NO_RETURN __attribute__ ((noreturn))
#define UNUSED __attribute__ ((unused))
#define NO_OPTIMIZE __attribute__((optimize("O0")))
#define OPTIMIZE __attribute__((optimize("O2")))
#define ALIGNED(n) __attribute__((aligned(n)))
#define PRINTF_FORMAT(FMT, FIRST) __attribute__ ((format (printf, FMT, FIRST)))

#define true 1
#define false 0

typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short        int16_t;
typedef unsigned int        uint32_t;
typedef signed int          int32_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;

typedef unsigned int        bool_t;
typedef unsigned int        size_t;
typedef unsigned short      selector_t;
typedef unsigned int        pid_t;
typedef unsigned int        tid_t;
typedef unsigned int        filedesc_t;
typedef __builtin_va_list   va_list;

typedef	void (*intr_stub)	();
typedef int (*compare) (const void *,const void *);

#endif // QIUOS_TYPE_H_