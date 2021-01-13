
#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../common.h"

// Define System call id
#define SYSCALL_ID_print_hello    1



void svc_handler(void) ;
extern void syscall_handler(uint32_t syscall_id);


//user syscall
extern void syscall_print_hello() ;




//Individual svc syscall
void __print_hello(void) ;

#endif