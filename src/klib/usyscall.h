
#ifndef __USYSCALL_H_
#define __USYSCALL_H_

#include "../common.h"
#include "../kernel/syscall.h"


void __print_hello(uint32_t *input) ;
void __yield(void);
int __gettid(void);
void __exit(void);
int __fork(void);
int __do_taskCreate(void (*taskFunc)());
void *__malloc_blk() ;

#endif