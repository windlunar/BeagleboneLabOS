
#include "task.h"
#include "../klib/queue.h"

SCHED_CONTEXT_t *schedFuncContextSPtr = (SCHED_CONTEXT_t *)0x9df31000 ; 
TASK_t task_origin ;
uint32_t task_origin_stack[TASK_STACK_SIZE] ;
TASK_t *task_ready_queue_head = NULL;
TASK_t *curr_running_task = NULL ;

int32_t taskid = -1 ;

//每個陣列皆為一個指標, 指向該函式起始位置
QUEUE_TASK_t taskReadyQ ;



// 如果 _yield ,timer interrupt ,都從sched開始執行的話?
// 原來在執行的 user proccess要在返回之前設定成 TASK_READY ,然後應該要實作個ready queue ,放到queue最後面
void sched(void)
{
	//Return from user mode(context switch through timer irq) 
	//,and set the task status to READY
	kprintf("In kernel\r\n\r\n") ;

	//choose a task to run
	for(;;)
	{
		//判斷 head是否 Ready
		if(task_ready_queue_head->task_status == TASK_READY)
		{
			TASK_t *origin_head = task_ready_queue_head ;

			//dequeue task ,回傳值目前不需要用到
			TASK_t *r = task_dequeue() ;

			//And the put it to back ,and set it to running
			task_enqueue(origin_head) ;
			origin_head->task_status = TASK_RUNNING ;

			//設定 現在正在 running 的 task結構
			curr_running_task = origin_head ;

			//Switch to user mode and run the task
			TaskRun((uint32_t *)origin_head->task_context_sp) ;
		}
	}
}


void schedFuncContextPrepare(void)
{
	schedFuncContextSPtr->lr = (uint32_t)sched ;	//跳轉address為 task.c的函式shed()
}


//
// arg2 : task 結構體
// arg3 : user task function pointer
// arg4 : user task的 stack起始位址(low addr)
uint32_t taskCreate(TASK_t *task_ptr ,void (*taskFunc)() ,uint32_t *task_stack){
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
	//task_enqueue(task_ptr) ;
	taskid++ ;
	task_ptr->task_id = taskid;

	//設定task stack的起始位址(low address開始)
	task_ptr->task_stack_ptr = task_stack ;

	//因為 task是從高位址往下增長 ,所以找 stack top(stack的最高位址)
	uint32_t *task_stack_top = task_ptr->task_stack_ptr + TASK_STACK_SIZE;

	//設定sp
    task_ptr->task_context_sp = (USR_TASK_CONTEXT_t *)(task_stack_top - 16);

	//設定task的跳轉address
	task_ptr->task_context_sp->r9_return_lr = (uint32_t) taskFunc;
	task_ptr->taskCallBack = taskFunc ;

	//設定task的狀態為ready
    task_ptr->task_status = TASK_READY ;

	//回傳task id
	return task_ptr->task_id ;

}

void task_enqueue(TASK_t *task_node)
{
	if(task_ready_queue_head == NULL){
		//create the first node
		task_node->next_ptr = NULL ;
		task_node->prev_ptr = NULL ;
		task_ready_queue_head = task_node ;
		return ;
	}
	// 不是第一個node
	// 從head找最後一個node
	TASK_t *head = task_ready_queue_head ;
	while(head->next_ptr != NULL){
		head = head->next_ptr ;
	}
	TASK_t *end = head ;

	end->next_ptr = task_node ;
	task_node->prev_ptr = end ;
	task_node->next_ptr = NULL ;
	
	
}

//從頭部取出返回原來的 TASK_t
TASK_t *task_dequeue()
{
	if(task_ready_queue_head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return NULL;
	}

	if(task_ready_queue_head->next_ptr == NULL){
		TASK_t *head = task_ready_queue_head ;
		task_ready_queue_head = NULL ;
		return head;
	}
	TASK_t *origin_head = task_ready_queue_head ;

	//next node becoome the new node
	TASK_t *next = origin_head->next_ptr ;
	
	next->prev_ptr = NULL ;
	task_ready_queue_head = next ;

	origin_head->next_ptr = NULL ;

	return origin_head ;
}


void remove_from_readylist(TASK_t *task_node)
{
	if(task_ready_queue_head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return;		
	}
	TASK_t *prev = task_node->prev_ptr ;
	TASK_t *next = task_node->next_ptr ;

	prev->next_ptr = next ;
	next->prev_ptr = prev ;

	task_node->next_ptr = NULL ;
	task_node->prev_ptr = NULL ;
}


void print_task_id_from_head()
{
	if(task_ready_queue_head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return;		
	}
	TASK_t *head = task_ready_queue_head ;
	while(head->next_ptr != NULL)
	{
		kprintf("task id = %d\r\n" ,head->task_id) ;
		head = head->next_ptr ;
	}
	kprintf("task id = %d\r\n\r\n" ,head->task_id) ;
}