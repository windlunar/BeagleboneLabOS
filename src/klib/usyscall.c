
#include "usyscall.h"


void __print_hello(uint32_t *input)
{
    syscall_print_hello(input) ;
}


void __yield(void)
{
    syscall_yield();
}


int __gettid(void)
{
    return syscall_get_tid();
}


void __exit(void)
{
    syscall_exit();
}


int __fork(void)
{
    return syscall_fork();
}


int __do_taskCreate(void (*taskFunc)())
{
    return syscall_do_taskCreate(taskFunc) ;
}
