
#ifndef __PRINT_H_
#define __PRINT_H_

#include "../common.h"
#include "stdarg.h"


void terminal_printC(int character) ;
static void print_char(int chartr);

int kprintf(const char *fmt, ...);
int kprintfmt(const char *fmt ,va_list args);

static void print_va(uint32 va_val, uint32 base, int32 width, int32 padc);


#endif