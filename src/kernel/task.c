
#include "task.h"
#include "../lib/queue.h"

SCHED_CONTEXT_t *schedFuncContextSPtr = (SCHED_CONTEXT_t *)0x9df31000 ; 
USERTASK_t userTask[TASK_NUM] ;

//每個陣列皆為一個指標, 指向該函式起始位置
void (*userTaskFuncsVector[TASK_NUM])(void);
QUEUE_TASK_t taskReadyQ ;



// 如果 _yield ,timer interrupt ,都從sched開始執行的話?
// 原來在執行的 user proccess要在返回之前設定成 TASK_READY ,然後應該要實作個ready queue ,放到queue最後面
void sched(void)
{
	//Return from user mode(context switch through timer irq) 
	//,and set the task status to READY
	kprintf("		Back to kernel ,Run another task\r\n\r\n") ;

	//choose a task to run
	for(;;)
	{
		//判斷 head是否 Ready
		if(taskReadyQ.qDataTaskStructPtr[taskReadyQ.idxHead]->taskStatus == TASK_READY)
		{
			USERTASK_t *TaskStructPtr = taskReadyQ.qDataTaskStructPtr[taskReadyQ.idxHead] ;

			//dequeue task
			deQueue(&taskReadyQ) ;

			//And the put it to back ,and set it to running
			enQueue(&taskReadyQ ,TaskStructPtr) ;
			TaskStructPtr->taskStatus = TASK_RUNNING ;

			//Switch to user mode and run the task
			userTaskRun((uint32_t *)TaskStructPtr->usrTaskContextSPtr) ;
		}
	}
}

void userTasksInit(int32_t taskid, USERTASK_t *userTaskStructPtr ,void (*taskFunc)()){
	/* Initialization of process stack.
	 * r0 ,r1 ,r2 ,r3 ,r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
     * 
     * push : 將data放到sp-4的位址
     * pop : 將sp的data取出,然後sp+4
     * 所以sp指向 stack中最上面的那一個data
     * 
     * usertask_stack_start[0] ~ [9]依次存放 :
     * r0 ,r1 ,r2 ,r3 ,r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
     * 預設 r9 存放 user proc返回位址
     * lr 存放該 user自己本身的 lr值(如返回其他函數用)
     * 所以要把 usertask 的 entry位址放在 usertask_stack_start[9] 的位址上
     */
	uint32_t *userTaskStackTop = userTaskStructPtr->task_stack + TASK_STACK_SIZE;
    userTaskStructPtr->usrTaskContextSPtr = (USR_TASK_CONTEXT_t *)(userTaskStackTop - 16);

	userTaskStructPtr->usrTaskContextSPtr->r9_return_lr = (uint32_t) taskFunc;

     userTaskStructPtr->taskID = taskid ;
     userTaskStructPtr->taskStatus = TASK_READY ;

}


void schedFuncContextPrepare(void)
{
	schedFuncContextSPtr->lr = (uint32_t)sched ;	//跳轉address為 task.c的函式shed()
}