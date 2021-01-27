
#include "task.h"
#include "../klib/queue.h"

SCHED_CONTEXT_t *schedFuncContextSPtr = (SCHED_CONTEXT_t *)0x9df31000 ; 
TASK_INFO_t task_origin ;
uint32_t task_origin_stack[TASK_STACK_SIZE/4] ;
TASK_INFO_t *task_ready_queue_head = NULL;
TASK_INFO_t *curr_running_task = NULL ;

int32_t taskid = -1 ;


// 原來在執行的 user proccess在返回之前設定成 TASK_READY ,然後放入ready queue 最後面
void sched(void)
{
	// 從IRQ handler跟SVC handler返回到user mode會直接進到這個sched
	//choose a task to run
	for(;;)
	{
		//將task 從ready list 的 head 拿出 ,然後執行 TaskRun,切換到user mode跑task
		if(task_ready_queue_head->task_status == TASK_READY){
			TASK_INFO_t *_head = task_ready_queue_head ;

			//dequeue task ,回傳值目前不需要用到
			TASK_INFO_t *r = task_dequeue() ;

			//And the put it to back ,and set it to running
			task_enqueue(_head) ;
			_head->task_status = TASK_RUNNING ;

			//設定 現在正在 running 的 task結構
			curr_running_task = _head ;

			//Switch to user mode and run the task
			TaskRun((uint32_t *)_head->task_context) ;
		}
	}
}


void schedFuncContextPrepare(void)
{
	//跳轉address為 task.c的函式shed()
	schedFuncContextSPtr->lr = (uint32_t)sched ;	
}


//
// arg1 : task 結構體
// arg2 : user task function pointer
// arg3 : user task的 stack起始位址(low addr開始)
// return : task id
//
// task的context結構體(USR_TASK_CONTEXT_t)如下
// r0 ,r1 ,r2 ,r3 ,r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
// 將 sp指向 該結構起始位址 r0 ,那麼在pop的時候, 就會將其內容依序pop回暫存器中
// 預設 r9 存放 user task 返回位址
// lr 存放該 user自己本身的 lr值(如返回其他函數用)
// 所以要把 task 的 entry位址放在 USR_TASK_CONTEXT_t 的 r9_return_lr上
//
int32_t taskCreate(TASK_INFO_t *task ,void (*taskFunc)() ,void *stack)
{
	uint32_t *task_stack = (uint32_t *)stack ;

	taskid++ ;
	task->task_id = taskid;

	//設定task stack的起始位址(low address開始)
	task->stk_bottom = task_stack ;

	//因為 task是從高位址往下增長 ,所以找 stack top(stack的最高位址)
	task->stk_top = stkbottom2top(task->stk_bottom) ;

	//設定sp
	task->task_context = (USR_TASK_CONTEXT_t *)(task->stk_top - 15);

	//設定task的跳轉address
	task->task_context->r9_return_lr = (uint32_t) taskFunc;
	task->taskCallBack = taskFunc ;

	//設定task的狀態為ready
    task->task_status = TASK_READY ;

	//回傳task id
	return task->task_id ;

}

void task_enqueue(TASK_INFO_t *task_node)
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
	TASK_INFO_t *head = task_ready_queue_head ;
	while(head->next_ptr != NULL){
		head = head->next_ptr ;
	}
	TASK_INFO_t *end = head ;

	end->next_ptr = task_node ;
	task_node->prev_ptr = end ;
	task_node->next_ptr = NULL ;
	
	
}

//從頭部取出返回原來的 TASK_INFO_t
TASK_INFO_t *task_dequeue()
{
	if(task_ready_queue_head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return NULL;
	}

	if(task_ready_queue_head->next_ptr == NULL){
		TASK_INFO_t *head = task_ready_queue_head ;
		task_ready_queue_head = NULL ;
		return head;
	}
	TASK_INFO_t *origin_head = task_ready_queue_head ;

	//next node becoome the new node
	TASK_INFO_t *next = origin_head->next_ptr ;
	
	next->prev_ptr = NULL ;
	task_ready_queue_head = next ;

	origin_head->next_ptr = NULL ;

	return origin_head ;
}


void remove_from_readylist(TASK_INFO_t *task_node)
{
	if(task_node ==NULL){
		return ;
	}

	//list沒有node
	if(task_ready_queue_head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return;		
	}

	//list中只有自己
	if(task_ready_queue_head->next_ptr == NULL){
		task_ready_queue_head = NULL ;
	}

	TASK_INFO_t *prev = task_node->prev_ptr ;
	TASK_INFO_t *next = task_node->next_ptr ;

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
	TASK_INFO_t *head = task_ready_queue_head ;
	while(head->next_ptr != NULL)
	{
		kprintf("task id = %d\r\n" ,head->task_id) ;
		head = head->next_ptr ;
	}
	kprintf("task id = %d\r\n\r\n" ,head->task_id) ;
}