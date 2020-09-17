
#ifndef __PRINT_H_
#define __PRINT_H_

#include "common.h"
#include "stdarg.h"


void terminal_printC(int character) ;
static void print_char(int chartr ,int *count);

int kprintf(const char *fmt, ...);
int vkprintf(const char *fmt, va_list args);
int vprintfmt(void(*put_char)(int, int*) ,int *put_count ,const char *fmt ,va_list args);



#endif