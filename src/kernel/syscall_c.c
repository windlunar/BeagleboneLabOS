

#include "syscall.h"
#include "../lib/print.h"
#include "debug.h"
#include "task.h"
#include "../lib/queue.h"


void syscall_handler(uint32_t syscall_id ,uint32_t userTaskSP) ;
void syscall_handler(uint32_t syscall_id ,uint32_t userTaskSP){
    switch (syscall_id)
    {
    case SYSCALL_ID_print_hello:
        __print_hello() ;
        break;

    case SYSCALL_ID_yield:
        __yield(userTaskSP) ;
        break;   

    default:
        break;
    }
}


void __print_hello(void){
    
    kprintf("Hello! This is my first system call ,") ;
    readCpsrMode();
}

void __yield(uint32_t userTaskSP){
    // check which task's status is running
    // Then change the task satus back to ready
    //queue.c的back的計算好像有點問題
    /*
    int32_t qBack = (taskReadyQ.idxHead + TASK_NUM-1) % TASK_NUM ;
    if(taskReadyQ.qDataTaskStructPtr[qBack]->taskStatus == TASK_RUNNING)
    {
        kprintf("Here #1\r\n") ;
        taskReadyQ.qDataTaskStructPtr[taskReadyQ.idxBack]->taskStatus = TASK_READY ;

        //update the current saved user stack
        taskReadyQ.qDataTaskStructPtr[taskReadyQ.idxBack]->usrTaskContextSPtr = (uint32_t *)userTaskSP ;
    }
    kprintf("Here #2\r\n") ;
    sched() ;
    */
}