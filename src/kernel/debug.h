

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "../common.h"
#include "../driver/uart.h"
#include "../lib/print.h"



extern unsigned int READ_CPSR ();
extern unsigned int READ_SP ();
extern unsigned int READ_CP15_c1 ();
extern unsigned int READ_VECTOR_BASE ();

void print_here();
void print_LR(uint32 data);
void print_SP(uint32 data);
void stackDisplay(uint32 stackTop ,int num);


#endif