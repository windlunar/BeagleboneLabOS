
#include "task.h"
#include "../lib/print.h"

CONTEXT_t task_context[TASK_NUM] ;
uint32_t task_stack[TASK_NUM][TASK_STACK_SIZE] ;

uint32_t *userTaskInit(unsigned int *userTaskStack ,int stackSize ,void (*taskFunc)() ){
	/* Initialization of process stack.
	 * r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
     * 
     * push : 將data放到sp-4的位址
     * pop : 將sp的data取出,然後sp+4
     * 所以sp指向 stack中最上面的那一個data
     * 
     * usertask_stack_start[0] ~ [9]依次存放 :
     * r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr
     * 所以要把 usertask的entry位址放在 usertask_stack_start[9] 的位址上
     */
	uint32_t *userTaskStack_top = userTaskStack + stackSize ;
	uint32_t *userTaskStack_start = userTaskStack_top - 16;
	userTaskStack_start[9] = (uint32) taskFunc;

     //userTaskStack_start[8] = (uint32) userTaskStack_start ;

	return userTaskStack_start ;
}



