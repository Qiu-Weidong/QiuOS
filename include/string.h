#ifndef QIUOS_STRING_H_
#define QIUOS_STRING_H_
#include "type.h"

//////////////////////////////////////////////////////
/// @brief 从src开始复制size个字节到dest，
///        如果内存重叠则不保证正确性。
/// @param dest 目标地址
/// @param src 源地址
/// @param size 复制的字节数
/// @return dest
//////////////////////////////////////////////////////
public
void *memcpy(void *dest, const void *src, size_t size);

//////////////////////////////////////////////////////
/// @brief 从src开始复制size个字节到dest，
///        如果内存重叠将src覆盖。
/// @param dest 目标地址
/// @param src 源地址
/// @param size 复制的字节数
/// @return dest
//////////////////////////////////////////////////////
public
void *memmove(void *dest, const void *src, size_t size);

public
int memcmp(const void *a, const void *b, size_t size);

//////////////////////////////////////////////////////
/// @brief 将dest指向的内存中的size字节设置为value
/// @param dest 目标地址
/// @param value 要设置的值
/// @param size 设置字节数
/// @return dest
//////////////////////////////////////////////////////
public
void *memset(void *dest, int value, size_t size);

//////////////////////////////////////////////////////
/// @brief 计算字符串的长度
/// @param str 输入字符串
/// @return str的长度
//////////////////////////////////////////////////////
public
size_t strlen(char *str);

//////////////////////////////////////////////////////
/// @brief 计算字符串的长度，如果小于maxlen则返回实际
///        长度，否则返回maxlen
/// @param str 输入字符串
/// @param maxlen 最大长度
/// @return str的长度或maxlen
//////////////////////////////////////////////////////
public
size_t strnlen(char *str, size_t maxlen);

//////////////////////////////////////////////////////
/// @brief 将src指向的字符串复制到dest
/// @param dest 目标地址
/// @param src 源字符串
/// @return dest
//////////////////////////////////////////////////////
public
char *strcpy(char *dest, const char *src);

//////////////////////////////////////////////////////
/// @brief 将src指向的字符串复制到dest,最多复制size个字节
/// @param dest 目标地址
/// @param src 源字符串
/// @param size 最多复制size个字节
/// @return dest
//////////////////////////////////////////////////////
public
char *strncpy(char *dest, const char *src, size_t size);

//////////////////////////////////////////////////////
/// @brief 将src指向的字符串追加到dest
/// @param dest 目标地址
/// @param src 源字符串
/// @return dest
//////////////////////////////////////////////////////
public
char *strcat(char *dest, const char *src);

//////////////////////////////////////////////////////
/// @brief 将src指向的字符串追加到dest,最多追加size个字节
/// @param dest 目标地址
/// @param src 源字符串
/// @param size 最多追加size个字节
/// @return dest
//////////////////////////////////////////////////////
public
char *strncat(char *dest, const char *src, size_t size);

//////////////////////////////////////////////////////
/// @brief 字符串比较函数，找到str1和str2第一个不同的字符
///         如果str1中的字符更大，则返回正数，如果str2中的
///         字符更大则返回负数，如果str1和str2完全相同则返回0
/// @param str1 字符串1
/// @param str2 字符串2
/// @return 比较结果
//////////////////////////////////////////////////////
public
int strcmp(const char *str1, const char *str2);

//////////////////////////////////////////////////////
/// @brief 字符串比较函数，找到str1和str2第一个不同的字符
///         如果str1中的字符更大，则返回正数，如果str2中的
///         字符更大则返回负数，如果str1和str2完全相同则
///         返回0。如果两个字符串长度都大于size，则只比较
///         前size个字符
/// @param str1
/// @param str2 源字符串
/// @param size 最多比较size个字符
/// @return 比较结果
//////////////////////////////////////////////////////
public
int strncmp(const char *str1, const char *str2, size_t size);

//////////////////////////////////////////////////////
/// @brief 寻找字符在字符串中首先出现的位置
/// @param str 输入字符串
/// @param c   待寻找字符
/// @return 字符出现的位置，没有则为NULL
//////////////////////////////////////////////////////
public
char *strchr(char *str, char c);

//////////////////////////////////////////////////////
/// @brief 寻找字符在字符串中最后出现的位置
/// @param str 输入字符串
/// @param c   待寻找字符
/// @return 字符出现的位置，没有则为NULL
//////////////////////////////////////////////////////
public
char *strrchr(char *str, char c);

//////////////////////////////////////////////////////
/// @brief 寻找字符在字符串中首先出现的位置,最多访问前size个字符
/// @param str 输入字符串
/// @param c   待寻找字符
/// @param size 最多访问的字符
/// @return 字符出现的位置，没有则为NULL
//////////////////////////////////////////////////////
public
char *strnchr(char *str, char c, size_t size);

//////////////////////////////////////////////////////
/// @brief 寻找字串在字符串中首先出现的位置
/// @param str 输入字符串
/// @param substr  子串
/// @return 子串出现的位置，没有则为NULL
//////////////////////////////////////////////////////
public
char *strstr(char *str, const char *substr);

public
char *strtok_r(char *str, const char *delim, char **saveptr);

#endif // QIUOS_STRING_H_