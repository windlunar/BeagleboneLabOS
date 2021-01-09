
#ifndef __TASK_H_
#define __TASK_H_

#include "../common.h"

#define TASK_RUNNING    0
#define TASK_READY      1

#define TASK_STACK_SIZE 128
#define TASK_NUM 3

typedef struct
{
    int userTaskStackSize ;

    //userTaskStack最後一個空間
    uint32 *userTaskStack_end;  

    //userTaskStack起始空間
    uint32 *userTaskStack_start;   

    //原來的userTaskStack_start 
    uint32 *userTaskStackPtr;

    void (*taskFUNC)() ;
    int32 taskID;
    int32 taskStatus ;


}USERTASK_t;

extern uint32 task_stack[TASK_NUM][TASK_STACK_SIZE] ;

uint32 *userTaskInit(uint32 *userTaskStack ,int stackSize ,void (*taskFunc)() ) ;

//輸入參數 stack(Process stack pointer)會存到r0
uint32 *userTaskRun(uint32 *stack); 

#endif

