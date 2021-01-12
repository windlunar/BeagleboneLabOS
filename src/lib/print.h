
#ifndef __PRINT_H_
#define __PRINT_H_

#include "../common.h"
#include "stdarg.h"


void terminal_printC(int character) ;
void print_char(int chartr);

int kprintf(const char *fmt, ...);
int kprintfmt(const char *fmt ,va_list args);

static void print_va(uint32_t va_val, uint32_t base, int32_t width, int32_t padc);


#endif