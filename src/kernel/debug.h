

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "../common.h"
#include "../driver/uart.h"
#include "../klib/print.h"
#include "memory.h"



extern unsigned int READ_CPSR ();
extern unsigned int READ_SP ();
extern unsigned int READ_CP15_c1 ();
extern unsigned int READ_VECTOR_BASE ();

void print_here();
void print_LR(uint32_t data);
void print_SP(uint32_t data);
void stackDisplay(uint32_t stackTop ,int num);
int32_t readCpsr() ;
int32_t readCpsrMode() ;

void print_R1_R2_R3(uint32_t r1 ,uint32_t r2 ,uint32_t r3) ;

void print_free_part_list_from_head();
void print_free_part_list_from_end();
void print_inuse_part_list_from_head();
void mem_part_alloc_free_test();
void mem_part_blk_init_test();
void print_from_blk_head(MEM_PART_INFO_t *mem_part);

#endif