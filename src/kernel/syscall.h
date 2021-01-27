
#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../common.h"

// Define System call id
#define SYSCALL_ID_print_hello      1
#define SYSCALL_ID_yield            2        
#define SYSCALL_ID_get_tid          3
#define SYSCALL_ID_exit             4
#define SYSCALL_ID_fork             5

void svc_handler(void) ;
extern void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args);

//svc syscall call by syscall_handler
void __print_hello(uint32_t input) ;
void __yield(uint32_t *usrTaskContextOld);
void __get_tid(uint32_t *usrTaskContextOld);
void __exit();
void __fork(uint32_t *usrTaskContextOld ,uint32_t *args);

//syscall call by user task ,define in syscall_asm.s
extern void syscall_print_hello(uint32_t *input) ;
extern void syscall_yield();
extern int syscall_get_tid();
extern void syscall_exit();
extern int syscall_fork(uint32_t *args);





#endif