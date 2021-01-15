
#ifndef __TASK_H_
#define __TASK_H_

#include "../common.h"
#include "../lib/print.h"
#include "debug.h"


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

/***********************************************************************************************/
#define TASK_RUNNING    0
#define TASK_READY      1

#define TASK_STACK_SIZE 256
#define TASK_NUM 5


/***********************************************************************************************/





// -----------------
// ......
// lr
// r12
// r11
// r10
// r9
// r8
// r7
// r6
// r5
// r4
// r3
// r2
// r1 -----------------0x9df31000 + 4*1
// r0 -----------------kernel init stack top = 0x9df31000 (set up in start.s)
//                      sched sp (start addr of SCHED_CONTEXT_t schedFuncContextSPtr)
typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t lr;

}SCHED_CONTEXT_t;
extern SCHED_CONTEXT_t *schedFuncContextSPtr ; 




// *******************************************************
//
//                 SP------>lr
//                          r12
// push=STMFD SP!           r11
//        |                 r10
//        |                 r9      ^
//        |                 r8      |
//        |                 r7      |
//        |                 r6      |
//        |                 r5      |
//        |                 r4      |
//        V                 r3      |
//                          r2  pop=LDMFD SP!      
//                          r1              
//                          r0 <------SP
//
// *******************************************************
typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9_return_lr;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t lr;

}USR_TASK_CONTEXT_t;

typedef struct
{
    uint32_t task_stack[TASK_STACK_SIZE] ;
    //uint32_t *usrTaskContextSPtr; //指向task_stack的某個位址

    void (*taskCallBack)() ;

    int32_t taskID;
    int32_t taskStatus ;

    USR_TASK_CONTEXT_t *usrTaskContextSPtr ; 

    //int32_t priority ;

}USERTASK_t;
/***********************************************************************************************/

extern USERTASK_t userTask[TASK_NUM] ;

//user tasks的函式指標
extern void (*userTaskFuncsVector[TASK_NUM])(void);
extern void _call_sched(uint32_t schedContext) ;
/***********************************************************************************************/

void sched(void);
void userTasksInit(int32_t taskid, USERTASK_t *userTaskStructPtr ,void (*taskFunc)()) ;

//輸入參數 stack(Process stack pointer)會存到r0
//USR_TASK_CONTEXT_t *userTaskRun(uint32_t sp); 
void userTaskRun(uint32_t *sp); 
void schedFuncContextPrepare(void);

/***********************************************************************************************/
#endif

