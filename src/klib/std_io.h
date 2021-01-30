
#ifndef __STD_IO_H_
#define __STD_IO_H_

#include "../common.h"


void put_c(char chartr);
int32_t uprintf(const char *fmt, ...);
int uprint(const char *fmt ,__builtin_va_list args);
static void put_va(uint32_t va_val, uint32_t base, int32_t width, int32_t padc);

#endif