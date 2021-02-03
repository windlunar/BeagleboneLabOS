
#include "task.h"
#include "../klib/mem.h"



SCHED_CONTEXT_t *schedFuncContextSPtr = (SCHED_CONTEXT_t *)KSTACK_SCHED_CONTEXT_SP ; 

// 沒有static會出錯
typedef struct{
	TASK_INFO_t *head
}TASK_READY_LIST_HEAD_t ;

TASK_READY_LIST_HEAD_t task_ready_list[MAXNUM_PRIORITY] ;
//static TASK_INFO_t *task_ready_list_head[MAXNUM_PRIORITY] ;
TASK_INFO_t *curr_running_task = NULL ;

int32_t taskid = -1 ;
int32_t prio = -1 ;

/****************************************************************************************/
// 原來在執行的 user proccess在返回之前設定成 TASK_READY ,然後放入ready queue 最後面
/*
void sched(void)
{
	// 從IRQ handler跟SVC handler返回到user mode會直接進到這個sched
	//choose a task to run
	//uart_tx_str(CONSOLE ,"Here #1\r\n" ,9) ;
	//print_task_id_from_head(HIGHEST_PRIORITY) ;
	for(;;)
	{
		// 從priority 0開始
		//uart_tx_str(CONSOLE ,"#2\r\n\0" ,5) ;
		for(int i=0 ; i<MAXNUM_PRIORITY; i++){

			// 如果該prio的ready list不為空時
			if(task_ready_list[i].head != NULL){
				prio = i ;
				
				//將task 從ready list 的 head 拿出 ,然後執行 TaskRun,切換到user mode跑task
				if(task_ready_list[prio].head->task_status == TASK_READY){	
									
					TASK_INFO_t *_head = task_ready_list[prio].head ;

					//dequeue task ,回傳值目前不需要用到
					TASK_INFO_t *r = task_dequeue(prio) ;

					//And the put it to back ,and set it to running
					_head->task_status = TASK_RUNNING ;
					task_enqueue(_head) ;

					//設定 現在正在 running 的 task結構
					curr_running_task = _head ;
					if(_head->task_id != 0){
					}
					
					//Switch to user mode and run the task
					TaskRun((uint32_t *)_head->task_context) ;
				}else{
					kprintf("Error :The head in ready queue is not READY.\r\n") ;
				}
			}
		}	
	}
}
*/


void sched (void)
{
	//choose a task to run
	curr_running_task = choose_task() ;
	TaskRun((uint32_t *)curr_running_task->task_context) ;
}


TASK_INFO_t *choose_task(void)
{
	for(int i=0 ; i<MAXNUM_PRIORITY; i++){

		// 如果該prio的ready list不為空時
		if((task_ready_list[i].head != NULL) && (task_ready_list[i].head->task_status == TASK_READY)){
								
			TASK_INFO_t *_head = task_ready_list[i].head ;

			//dequeue task ,回傳值目前不需要用到
			TASK_INFO_t *r = task_dequeue(i) ;

			//And the put it to back ,and set it to running
			_head->task_status = TASK_RUNNING ;
			task_enqueue(_head) ;


			//設定 現在正在 running 的 task結構
			return _head ;	
		}
	}	
}




void schedFuncContextPrepare(void)
{
	//跳轉address為 task.c的函式shed()
	schedFuncContextSPtr->lr = (uint32_t)sched ;	
}

/****************************************************************************************/

void task_init()
{
	for(int i=0 ; i<MAXNUM_PRIORITY; i++)
	{
		task_ready_list[i].head = NULL ;
	}	
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
int32_t taskCreate(TASK_INFO_t *task ,void (*taskFunc)() ,void *stack ,int32_t prio)
{
	uint32_t *task_stack = (uint32_t *)stack ;

	taskid++ ;
	task->task_id = taskid;
	task->priority = prio ;

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

	//Init open file
	for(int i=0 ; i<MAX_FD; i++)
	{
		task->openfiles[i] = NULL ;
	}

	// 初始dir為空
	task->cwdn = NULL ;
	
	//回傳task id
	return task->task_id ;

}

// Create user task by kernel
int32_t do_ktaskCreate(int32_t prio ,void (*taskFunc)())
{
    MEM_AREA_INFO_t *n_ma = alloc_mem_area();
    n_ma->area_status = TASK_AREA ; 
	_memset((void *)n_ma->m_start, 0, AREA_SIZE) ;

    TASK_INFO_t *ntask = (TASK_INFO_t *)(n_ma->m_start) ;
    n_ma->m_aval_start = (uint32_t *)((uint32_t)n_ma->m_start + sizeof(TASK_INFO_t)) ;  
    n_ma->blk_head_ptr = n_ma->m_aval_start ;


    taskCreate(ntask ,taskFunc ,stktop2bottom(n_ma->m_top) ,prio);

    // init blocks
    // 總共應該會有56個blks = (4096-512)/64
    memblks_init(n_ma 
                ,DEFAULT_AVAL_BLK_SIZE 
                ,DEFAULT_TASK_MA_BLKNUM) ;

    //
    open_console_in_out(ntask) ;

    //設定路徑
    ntask->cwdn = root ;

    task_enqueue(ntask) ; 
}



void open_console_in_out(TASK_INFO_t *task)
{
// open console_in and console_out
	if(file_open(FILE_CONSOLE_IN ,(void *)task) < 0) kprintf("Failed to open 'console_in'\r\n") ;
	if(file_open(FILE_CONSOLE_OUT ,(void *)task) < 0) kprintf("Failed to open 'console_out'\r\n") ;
}



void task_enqueue(TASK_INFO_t *task)
{
	if(task_ready_list[task->priority].head == NULL){
		//create the first node
		task->next_ptr = NULL ;
		task->prev_ptr = NULL ;
		task_ready_list[task->priority].head = task ;
		return ;
	}
	// 不是第一個node
	// 從head找最後一個node
	TASK_INFO_t *head = task_ready_list[task->priority].head ;
	while(head->next_ptr != NULL){
		head = head->next_ptr ;
	}
	TASK_INFO_t *end = head ;

	end->next_ptr = task ;
	task->prev_ptr = end ;
	task->next_ptr = NULL ;
	
	
}

//從頭部取出返回原來的 TASK_INFO_t
TASK_INFO_t *task_dequeue(int32_t prio)
{
	if(task_ready_list[prio].head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return NULL;
	}

	if(task_ready_list[prio].head->next_ptr == NULL){
		TASK_INFO_t *head = task_ready_list[prio].head ;
		task_ready_list[prio].head = NULL ;
		return head;
	}
	TASK_INFO_t *origin_head = task_ready_list[prio].head ;

	//next node becoome the new node
	TASK_INFO_t *next = origin_head->next_ptr ;
	
	next->prev_ptr = NULL ;
	task_ready_list[prio].head = next ;

	origin_head->next_ptr = NULL ;

	return origin_head ;
}


// 從尾部取出
void task_pop(TASK_INFO_t *task)
{
	if((task == NULL) || (task->next_ptr != NULL)){
		return ;
	}

	//list沒有node
	if(task_ready_list[task->priority].head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return;		
	}

	//list中只有自己
	if(task_ready_list[task->priority].head->next_ptr == NULL){
		kprintf("Only itself\r\n") ;
		task_ready_list[task->priority].head = NULL ;
		return ;
	}

	TASK_INFO_t *prev = task->prev_ptr ;

	prev->next_ptr = NULL ;

	task->next_ptr = NULL ;
	task->prev_ptr = NULL ;
}

/****************************************************************************************/
void print_task_id_from_head(int32_t prio)
{
	if(task_ready_list[prio].head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return;		
	}
	TASK_INFO_t *head = task_ready_list[prio].head ;
	while(head->next_ptr != NULL)
	{
		kprintf("task id = %d\r\n" ,head->task_id) ;
		head = head->next_ptr ;
	}
	kprintf("task id = %d\r\n\r\n" ,head->task_id) ;
}


void print_task_addr_from_head(int32_t prio)
{
	if(task_ready_list[prio].head == NULL){
		kprintf("Task queue is empty\r\n") ;
		return;		
	}

	TASK_INFO_t *head = task_ready_list[prio].head ;
	kprintf("task_ready_list[prio].head addr =%p\r\n" ,&task_ready_list[prio].head) ;
	while(head->next_ptr != NULL)
	{
		kprintf("task addr = %p\r\n" ,head) ;
		head = head->next_ptr ;
	}
	kprintf("task addr = %p\r\n\r\n" ,head) ;
}