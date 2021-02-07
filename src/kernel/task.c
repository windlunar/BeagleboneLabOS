
#include "task.h"
#include "../klib/mem.h"



struct SCHED_CONTEXT *schedFuncContextSPtr = (struct SCHED_CONTEXT *)KSTACK_SCHED_CONTEXT_SP ; 

// 沒有static會出錯
struct TASK_READY_LIST_HEAD{

	struct TASK_INFO *head ;

} ;

struct TASK_READY_LIST_HEAD task_ready_list[MAXNUM_PRIORITY] ;
struct TASK_INFO *curr_running_task = NULL ;

int32_t taskid = -1 ;
int32_t prio = -1 ;

/****************************************************************************************/

void sched (void)
{
	//choose a task to run
	curr_running_task = choose_task() ;
	TaskRun((uint32_t *)curr_running_task->task_context) ;
}


struct TASK_INFO *choose_task(void)
{
	for (int i=0 ; i<MAXNUM_PRIORITY; i++) {

		// 如果該prio的ready list不為空時
		if ((task_ready_list[i].head != NULL) && (task_ready_list[i].head->task_status == TASK_READY)) {
								
			struct TASK_INFO *_head = task_ready_list[i].head ;

			//dequeue task ,回傳值目前不需要用到
			struct TASK_INFO *r = task_dequeue(i) ;

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
	for (int i=0 ; i<MAXNUM_PRIORITY; i++) {
		task_ready_list[i].head = NULL ;
	}	
}


//
// arg1 : task 結構體
// arg2 : user task function pointer
// arg3 : user task的 stack起始位址(low addr開始)
// return : task id
//
// task的context結構體(struct TASK_CONTEXT)如下
// r0 ,r1 ,r2 ,r3 ,r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
// 將 sp指向 該結構起始位址 r0 ,那麼在pop的時候, 就會將其內容依序pop回暫存器中
// 預設 r9 存放 user task 返回位址
// lr 存放該 user自己本身的 lr值(如返回其他函數用)
// 所以要把 task 的 entry位址放在 struct TASK_CONTEXT 的 r9_return_lr上
//
int32_t taskCreate(struct TASK_INFO *task ,void (*taskFunc)() ,void *stack ,int32_t prio)
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
	task->task_context = (struct TASK_CONTEXT *)(task->stk_top - 15);

	//設定task的跳轉address
	task->task_context->r9_return_lr = (uint32_t) taskFunc;
	task->taskCallBack = taskFunc ;

	//設定task的狀態為ready
    task->task_status = TASK_READY ;

	//Init open file
	for (int i=0 ; i<MAX_FD; i++) {
		task->openfiles[i] = NULL ;
	}

	// 初始dir為空
	task->cwdn = NULL ;
	
	//回傳task id
	return task->task_id ;

}

// Create user task by kernel
// ----------------------- offset = 0x100000 (1M)
//	Free area for task
// 
// ----------------------- offset = task stack size
//	task stack
// ----------------------- offset = sizeof(struct TASK_INFO)
// struct TASK_INFO
// ----------------------- offset = 0
int32_t do_ktaskCreate(int32_t prio ,void (*taskFunc)())
{
    struct PAGE_INFO *pg = page_alloc();
    pg->page_status = PAGE_FOR_TASK ; 
	_memset((void *)pg->pgstart, 0, TASK_STACK_SIZE) ;

    struct TASK_INFO *ntask = (struct TASK_INFO *)(pg->pgstart) ;


    taskCreate(ntask ,taskFunc ,(void *)pg->pgstart ,prio);

    blks_init(pg) ;

    open_console_in_out(ntask) ;

    //設定路徑
    ntask->cwdn = root ;

    task_enqueue(ntask) ; 
}


void set_page_free_start(uint32_t mv_bytes ,struct PAGE_INFO *pg)
{
    pg->free_start = (uint32_t *)((uint32_t)pg->free_start + mv_bytes) ;  
    pg->blk_list_head = pg->free_start ;	
}

void open_console_in_out(struct TASK_INFO *task)
{
// open console_in and console_out
	if (file_open(FILE_CONSOLE_IN ,(void *)task) < 0) printk("Failed to open 'console_in'\r\n") ;
	if (file_open(FILE_CONSOLE_OUT ,(void *)task) < 0) printk("Failed to open 'console_out'\r\n") ;
}



void task_enqueue(struct TASK_INFO *task)
{
	if (task_ready_list[task->priority].head == NULL) {
		//create the first node
		task->next_ptr = NULL ;
		task->prev_ptr = NULL ;
		task_ready_list[task->priority].head = task ;
		return ;
	}

	// 不是第一個node
	// 從head找最後一個node
	struct TASK_INFO *head = task_ready_list[task->priority].head ;

	while (head->next_ptr != NULL) {
		head = head->next_ptr ;
	}
	struct TASK_INFO *end = head ;

	end->next_ptr = task ;
	task->prev_ptr = end ;
	task->next_ptr = NULL ;
	
	
}

//從頭部取出返回原來的 struct TASK_INFO
struct TASK_INFO *task_dequeue(int32_t prio)
{
	if (task_ready_list[prio].head == NULL) {
		printk("Task queue is empty\r\n") ;
		return NULL;
	}

	if (task_ready_list[prio].head->next_ptr == NULL) {
		struct TASK_INFO *head = task_ready_list[prio].head ;
		task_ready_list[prio].head = NULL ;
		return head;
	}
	struct TASK_INFO *origin_head = task_ready_list[prio].head ;

	//next node becoome the new node
	struct TASK_INFO *next = origin_head->next_ptr ;
	
	next->prev_ptr = NULL ;
	task_ready_list[prio].head = next ;

	origin_head->next_ptr = NULL ;

	return origin_head ;
}


// 從尾部取出
void task_pop(struct TASK_INFO *task)
{
	if ((task == NULL) || (task->next_ptr != NULL)) {
		return ;
	}

	//list沒有node
	if (task_ready_list[task->priority].head == NULL) {
		printk("Task queue is empty\r\n") ;
		return;		
	}

	//list中只有自己
	if (task_ready_list[task->priority].head->next_ptr == NULL) {
		printk("Only itself\r\n") ;
		task_ready_list[task->priority].head = NULL ;
		return ;
	}

	struct TASK_INFO *prev = task->prev_ptr ;

	prev->next_ptr = NULL ;

	task->next_ptr = NULL ;
	task->prev_ptr = NULL ;
}

/****************************************************************************************/
void print_task_id_from_head(int32_t prio)
{
	if (task_ready_list[prio].head == NULL) {
		printk("Task queue is empty\r\n") ;
		return;		
	}
	struct TASK_INFO *head = task_ready_list[prio].head ;
	while (head->next_ptr != NULL) {
		printk("task id = %d\r\n" ,head->task_id) ;
		head = head->next_ptr ;
	}
	printk("task id = %d\r\n\r\n" ,head->task_id) ;
}


void print_task_addr_from_head(int32_t prio)
{
	if (task_ready_list[prio].head == NULL) {
		printk("Task queue is empty\r\n") ;
		return;		
	}

	struct TASK_INFO *head = task_ready_list[prio].head ;
	printk("task_ready_list[prio].head addr =%p\r\n" ,&task_ready_list[prio].head) ;
	while (head->next_ptr != NULL) {
		printk("task addr = %p\r\n" ,head) ;
		head = head->next_ptr ;
	}
	printk("task addr = %p\r\n\r\n" ,head) ;
}