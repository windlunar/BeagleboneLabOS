
#ifndef __TASK_H_
#define __TASK_H_

#include "../common.h"



/*
typedef struct
{
    uint32_t r0 ;
    uint32_t lr_usr_own ;       //store user proc's originak lr reg
    uint32_t r1 ;
    uint32_t r2 ;
    uint32_t r3 ;
    uint32_t r4 ;
    uint32_t r5 ;
    uint32_t r6 ;
    uint32_t r7 ;
    uint32_t r8 ;
    uint32_t r9_return_lr ;     
    uint32_t r10 ;
    uint32_t fp ;
    uint32_t ip_store_spsr ;    //store previos mode's cpsr
    uint32_t lr ;               //store user proc's return addr  
}CONTEXT_t;
*/


#define TASK_RUNNING    0
#define TASK_READY      1

#define TASK_STACK_SIZE 256
#define TASK_NUM 5


typedef struct
{
    uint32_t task_stack[TASK_STACK_SIZE] ;
    uint32_t *userTaskStackPtr; //指向task_stack的某個位址

    void (*taskCallBack)() ;

    int32_t taskID;
    int32_t taskStatus ;

    //int32_t priority ;


}USERTASK_t;

extern USERTASK_t userTask[TASK_NUM] ;

//user tasks的函式指標
extern void (*userTaskVector[TASK_NUM])(void);



void sched(void);
void userTasksInit(int32_t taskid, USERTASK_t *userTaskStructPtr ,void (*taskFunc)()) ;

//輸入參數 stack(Process stack pointer)會存到r0
uint32_t *userTaskRun(uint32_t *stack); 

#endif

