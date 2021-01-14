
#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../common.h"

// Define System call id
#define SYSCALL_ID_print_hello      1
#define SYSCALL_ID_yield      2        


void svc_handler(void) ;
extern void syscall_handler(uint32_t syscall_id ,uint32_t userTaskSP);


//syscall call by user
extern void syscall_print_hello() ;
extern void syscall_yield();



//Individual svc syscall
void __print_hello(void) ;
void __yield(uint32_t userTaskSP) ;

#endif