

#ifndef __DEBUG_H_
#define __DEBUG_H_

#include "../common.h"
#include "../driver/uart.h"
#include "kprint.h"
#include "memory.h"
#include "task.h"



extern unsigned int READ_CPSR ();
extern unsigned int READ_SP ();
extern unsigned int READ_CP15_c1 ();
extern unsigned int READ_VECTOR_BASE ();

void print_here();
void print_LR(uint32_t data);
void print_SP(uint32_t data);
void stackDisplay(uint32_t stackTop ,int num);
int32_t readCpsrMode() ;

void print_R1_R2_R3(uint32_t r1 ,uint32_t r2 ,uint32_t r3) ;

void print_free_area_list_from_head();
void print_free_area_list_from_end();
void print_inuse_area_list_from_head();
void mem_area_alloc_free_test();
void print_from_blk_head(struct PAGE_INFO *mem_area);

#endif