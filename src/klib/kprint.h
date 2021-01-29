
#ifndef __PRINT_H_
#define __PRINT_H_

#include "../common.h"


void terminal_printC(int32_t character) ;
void print_char(int32_t chartr);

int32_t kprintf(const char *fmt, ...);
int kprint(const char *fmt ,__builtin_va_list args);

static void print_va(uint32_t va_val, uint32_t base, int32_t width, int32_t padc);

#endif