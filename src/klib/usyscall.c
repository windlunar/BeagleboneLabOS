
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


int __do_taskCreate(void (*taskFunc)() ,int32_t prio)
{
    TASK_CONFIG config ;
    config.taskCallBack = taskFunc ;
    config.prio = prio ;
    return syscall_do_taskCreate(&config) ;
}

// Allocate one block(60 bytes aval)
void *__malloc_blk()
{
    return syscall_malloc_blk() ;
}


// 如果輸入參數不是blk起始位址的話 ,目前還沒有偵錯機制
void __mfree_blk(void *blk_aval_start)
{
    syscall_mfree_blk(blk_aval_start) ;
}


void __get_mblk_list()
{
    syscall_get_mblk_list() ;
}



int __get_task_priority()
{
    return syscall_get_task_priority() ;
}



int __write(int fd ,char *buf ,uint32_t n_bytes)
{
    FILE_WRITE_SETUP_t args ;
    args.fd = fd ;
    args.wrbuf =buf ;
    args.n_bytes = n_bytes ;

    return syscall_write(&args) ;    
}