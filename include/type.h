#ifndef QIUNIX_TYPE_H_
#define QIUNIX_TYPE_H_

#define public
#define private static
#define NULL ((void *)0)
#define nullptr ((void *)0)

#define NO_RETURN __attribute__((noreturn))
#define UNUSED __attribute__((unused))
#define NO_OPTIMIZE __attribute__((optimize("O0")))
#define OPTIMIZE __attribute__((optimize("O2")))
#define ALIGNED(n) __attribute__((aligned(n)))
#define PRINTF_FORMAT(FMT, FIRST) __attribute__((format(printf, FMT, FIRST)))

void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp) \
        if (exp)    \
                ;   \
        else        \
                assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)

#define true 1
#define false 0

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

typedef unsigned int bool_t;
typedef unsigned int size_t;
typedef unsigned short selector_t;
typedef unsigned int pid_t;
typedef unsigned int tid_t;
typedef unsigned int filedesc_t;
typedef __builtin_va_list va_list;

typedef void (*intr_stub)();
typedef int (*compare)(const void *, const void *);

struct mess1
{
        int m1i1;
        int m1i2;
        int m1i3;
        int m1i4;
};
struct mess2
{
        void *m2p1;
        void *m2p2;
        void *m2p3;
        void *m2p4;
};
struct mess3
{
        int m3i1;
        int m3i2;
        int m3i3;
        int m3i4;
        uint64_t m3l1;
        uint64_t m3l2;
        void *m3p1;
        void *m3p2;
};
typedef struct
{
        int source;
        int type;
        union
        {
                struct mess1 m1;
                struct mess2 m2;
                struct mess3 m3;
        } u;
} message;

#endif // QIUNIX_TYPE_H_