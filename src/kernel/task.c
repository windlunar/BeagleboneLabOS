
#include "task.h"
#include "../klib/mem.h"
#include "mmu.h"


struct SCHED_CONTEXT *schedFuncContextSPtr = (struct SCHED_CONTEXT *)KSTACK_SCHED_CONTEXT_SP ; 


struct TASK_READY_LIST_HEAD
{
	struct TASK_INFO *head ;
} ;

struct TASK_READY_LIST_HEAD task_ready_list[MAXNUM_PRIORITY] ;
struct TASK_INFO *curr_running_task = NULL ;


int32_t taskid ;
int32_t prio ;


/**
 * arg : void
 * return : TASK_INFO structure
 * 
 * Choose a task from ready queue ,dequeue from ready list and put it to the tail of list
 * By default ,it has 5 ready list(queue) ,uses an array of pointers ,each pointer in array
 * point to a ready list's head.
 * The index of array represent the priority of the ready list which it points to.
 * 
 * This function will check the the ready list array of pointers from index 0 which has the
 * largest priority.Hence ,the larger priority whill first been executed. If the head is 
 * not empty(means at least one task in ready list) then  deque from the list ,put it to 
 * the end of the list ,set the task status to RUNNING ,and return the TASK_INFO structure. 
 */ 
struct TASK_INFO *choose_task(void)
{
	for (int i=0 ; i<MAXNUM_PRIORITY; i++) {

		// 如果該prio的ready list不為空時
		if ((task_ready_list[i].head != NULL) 
			&& (task_ready_list[i].head->task_status == TASK_READY)) {
								
			struct TASK_INFO *_head = task_ready_list[i].head ;
			struct TASK_INFO *r = task_dequeue(i) ;

			/** Put it to back ,and set it to running */
			_head->task_status = TASK_RUNNING ;
			task_enqueue(_head) ;

			/** 回傳選定的task結構 */
			return _head ;	
		}
	}	
}



void run_first_sched (void)
{
	/** choose a task to run */
	curr_running_task = choose_task() ;

	/** Switch page table base */
	switch_mm(curr_running_task->pgtbase) ;
	
	/** Run task */
	first_run((uint32_t *)curr_running_task->task_context) ;
}



/**
 * arg : void
 * return : void
 * 
 * Choose a task from ready queue ,switch the page table ,and then run the task by 
 * passing the context(the context structure will be stored at task's stack ,and the 
 * stack pointer will point to the start address of this context structure.)
 */ 
void sched (void)
{
	/** choose a task to run */
	curr_running_task = choose_task() ;

	/** Switch page table base */
	switch_mm(curr_running_task->pgtbase) ;
	
	/** Run task */
	switch_task((uint32_t *)curr_running_task->task_context) ;
}


/**
 * arg : void
 * return : void
 * 
 * lr(R14) register stores the return address ,the program will return to address which 
 * stored in R14 by execute "bx	lr" instruction.
 * So just make lr var as the start address of function. Then plase the context sturcture
 * to the stack ,make stack pointer points to the struct's start address ,and pop the 
 * context from stack.Finally ,after execute "bx lr" the program will jump to the function.
 * In this case ,it will jump to sched function by calling "switch_task". 
 * 
 */ 
void set_sched_context(void)
{
	schedFuncContextSPtr->lr = (uint32_t)sched ;	
}



void set_first_sched(void)
{
	schedFuncContextSPtr->lr = (uint32_t)run_first_sched ;	
}



void task_init()
{
	for (int i=0 ; i<MAXNUM_PRIORITY; i++) {
		task_ready_list[i].head = NULL ;
	}

	taskid = -1 ;
	prio = -1 ;
}


/**
 * arg1 : TASK_INFO structure.
 * arg2 : The function pointer that points to a specific task.
 * arg3 : The stack bottom of task.
 * arg4 : Task priority
 * 
 * return : task id
 * 
 * The context struct of task(struct TASK_CONTEXT) is defined in task.h.
 * Starts from:
 * r0 ,r1 ,r2 ,r3 ,r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr
 * 
 * For running or switching the task ,this context structure will be place inside task's 
 * stack. Then make user mode's SP points to the context struct ,and restore(pop) the 
 * context from stack.
 * 
 * By default R9 stores the return address from kernal mode.(lr stores the return address
 * from the function called by task itself.)
 * Hence ,make R9(r9_return_lr) points to the entry of target function.
 * Then , asign a an id to task ,setting the priority ,stack top ,place context struct to
 * stack.
 */
int32_t create_task(struct TASK_INFO *task ,void (*taskFunc)() ,void *stack ,int32_t prio)
{
	uint32_t *task_stack = (uint32_t *)stack ;

	taskid++ ;
	task->task_id = taskid;
	task->priority = prio ;

	/** 設定task stack的起始位址(low address開始) */
	task->stk_bottom = task_stack ;

	/** 設定 stack top(stack的最高位址) ,因為 task是從高位址往下增長 */
	task->stk_top = stkbottom2top(task->stk_bottom) ;

	/** 設定sp */
	task->task_context = (struct TASK_CONTEXT *)(task->stk_top - 15);

	/**設定task的跳轉address */
	task->task_context->r9_return_lr = (uint32_t) taskFunc;
	task->taskCallBack = taskFunc ;

	/**設定task的狀態為ready */
	task->task_status = TASK_READY ;

	/** Init open file */
	for (int i=0 ; i<MAX_FD; i++) {
		task->openfiles[i] = NULL ;
	}

	/** 初始dir為空 */
	task->cwdn = NULL ;
	
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
int32_t do_task_create(int32_t prio ,void (*taskFunc)())
{
	struct PAGE_INFO *pg = page_alloc();
	pg->page_status = FOR_TASK ; 
	_memset((void *)pg->pgstart, 0, TASK_STACK_SIZE) ;

	struct TASK_INFO *ntask = (struct TASK_INFO *)(pg->pgstart) ;

	create_task(ntask ,taskFunc ,(void *)pg->pgstart ,prio);

	blks_init(pg) ;

	open_console_in_out(ntask) ;

	//設定路徑
	ntask->cwdn = root ;

	task_enqueue(ntask) ; 

	ntask->pgtbase = (uint32_t *)task_pgt_setup(pg->pgstart ,pg->top) ;
}



void set_page_free_start(uint32_t mv_bytes ,struct PAGE_INFO *pg)
{
	pg->free_start = (uint32_t *)((uint32_t)pg->free_start + mv_bytes) ;  
	pg->blk_list_head = (struct BLK_INFO *)pg->free_start ;	
}



void open_console_in_out(struct TASK_INFO *task)
{
	/** open console_in and console_out */
	if (file_open(FILE_CONSOLE_IN ,(void *)task) < 0) 
		printk("Failed to open 'console_in'\r\n") ;

	if (file_open(FILE_CONSOLE_OUT ,(void *)task) < 0) 
		printk("Failed to open 'console_out'\r\n") ;
}



void task_enqueue(struct TASK_INFO *task)
{
	if (task_ready_list[task->priority].head == NULL) {
		/** create the first node */
		task->next_ptr = NULL ;
		task->prev_ptr = NULL ;
		task_ready_list[task->priority].head = task ;
		return ;
	}

	/** Find end node */
	struct TASK_INFO *head = task_ready_list[task->priority].head ;

	while (head->next_ptr != NULL) {
		head = head->next_ptr ;
	}
	struct TASK_INFO *end = head ;

	end->next_ptr = task ;
	task->prev_ptr = end ;
	task->next_ptr = NULL ;
}

/** 從頭部取出 ,並返回 struct TASK_INFO */
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

	/** next node becoome the new node */
	struct TASK_INFO *next = origin_head->next_ptr ;
	
	next->prev_ptr = NULL ;
	task_ready_list[prio].head = next ;

	origin_head->next_ptr = NULL ;

	return origin_head ;
}


/** 從尾部取出 */
void task_pop(struct TASK_INFO *task)
{
	if ((task == NULL) || (task->next_ptr != NULL)) {
		return ;
	}

	/** list沒有node */
	if (task_ready_list[task->priority].head == NULL) {
		printk("Task queue is empty\r\n") ;
		return;		
	}

	/** list中只有自己 */
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