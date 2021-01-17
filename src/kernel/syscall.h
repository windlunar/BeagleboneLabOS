
#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../common.h"

// Define System call id
#define SYSCALL_ID_print_hello      1
#define SYSCALL_ID_yield      2        


void svc_handler(void) ;
extern void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args);



//syscall call by user task
extern void syscall_print_hello(uint32_t *input) ;
extern void syscall_yield();



//svc syscall call bu syscall_handler
void  __print_hello(uint32_t input) ;
void __yield(uint32_t *usrTaskContextOld);

#endif