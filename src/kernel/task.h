
#ifndef __TASK_H_
#define __TASK_H_

#include "../common.h"

#define TASK_RUNNING    0
#define TASK_READY      1

#define TASK_STACK_SIZE 256
#define TASK_NUM 5


typedef struct
{
    uint32_t sp ;
    uint32_t r0 ;
    uint32_t r1 ;
    uint32_t r2 ;
    uint32_t r3 ;
    uint32_t r4 ;
    uint32_t r5 ;
    uint32_t r6 ;
    uint32_t r7 ;
    uint32_t r8 ;
    uint32_t r9_return_lr ;     //store user proc's return addr
    uint32_t r10 ;
    uint32_t fp ;
    uint32_t ip_store_spsr ;    //store previos mode's cpsr
    uint32_t lr ;               //store user proc's originak lr reg
}CONTEXT_t;



typedef struct
{
    int userTaskStackSize ;

    //userTaskStack最後一個空間
    uint32_t *userTaskStack_end;  

    //userTaskStack起始空間
    uint32_t *userTaskStack_start;   

    //原來的userTaskStack_start 
    uint32_t *userTaskStackPtr;

    void (*taskFUNC)() ;
    int32_t taskID;
    int32_t taskStatus ;


}USERTASK_t;



extern uint32_t task_stack[TASK_NUM][TASK_STACK_SIZE] ;
extern CONTEXT_t task_context[TASK_NUM] ;



uint32_t *userTaskInit(uint32_t *userTaskStack ,int stackSize ,void (*taskFunc)() ) ;

//輸入參數 stack(Process stack pointer)會存到r0
uint32_t *userTaskRun(uint32_t *stack); 

#endif

