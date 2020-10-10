
#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "common.h"

void svc_handler(void) ;
uint32 *activate(uint32 *stack); //輸入參數 stack(Process stack pointer)會存到r0

void sys_call() ;


#endif