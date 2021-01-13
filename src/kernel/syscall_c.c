

#include "syscall.h"
#include "../lib/print.h"
#include "debug.h"


void syscall_handler(uint32_t syscall_id) ;
void syscall_handler(uint32_t syscall_id){
    switch (syscall_id)
    {
    case SYSCALL_ID_print_hello:
        __print_hello() ;
        break;

    case SYSCALL_ID_yield:
        __yield() ;
        break;   

    default:
        break;
    }
}


void __print_hello(void){
    
    kprintf("Hello! This is my first system call ,") ;
    readCpsrMode();
}

void __yield(void){
    //Not implement Yet
}