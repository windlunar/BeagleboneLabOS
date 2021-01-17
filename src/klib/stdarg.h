#ifndef __STDARG_H
#define	__STDARG_H

//__builtin_va_list is built in to the compiler ,not a macro
typedef __builtin_va_list   va_list;

/** ap point to the first var of variable-arguments(可變參數)
 * last point to the previous argument
 * ex printf(const char * ,...)
 * ap point to const char * ,last point to ...
 * */
#define va_start(ap, last)  __builtin_va_start(ap, last)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)
#define va_end(ap)          __builtin_va_end(ap)

#endif
