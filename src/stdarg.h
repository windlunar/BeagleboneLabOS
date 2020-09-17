// Ported from JOS
#ifndef __STDARG_H
#define	__STDARG_H

typedef __builtin_va_list va_list;

/** ap指向可變參數的第一個參數 ,last指向前一個參數*/
#define va_start(ap, last) __builtin_va_start(ap, last)

#define va_arg(ap, type) __builtin_va_arg(ap, type)

#define va_end(ap) __builtin_va_end(ap)

#endif
