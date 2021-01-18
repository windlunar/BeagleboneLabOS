#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"

TASK_t Task[6] ;

void main_origin()
{
/************************************************************************************************/
// Test Fork function
/************************************************************************************************/

	MEM_PART_INFO_t *mempart = alloc_one_mem_part();
	mempart->part_status = INUSE_FULL ;
	taskCreate(&Task[0] ,&fork_test_main ,mempart->mempart_top_ptr-TASK_STACK_SIZE);
	task_enqueue(&Task[0]) ; 

/************************************************************************************************/


/************************************************************************************************/
// 測試 multitasking
// Time slice base. Round Robin Multitasking
/************************************************************************************************/
	kprintf("Wait the fork test terminate.Otherwise it's hard to idetified what console print.\r\n") ;
	for(int32_t i=0 ;i<200000;i++);
	kprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test multitasking.\r\n") ;

	MEM_PART_INFO_t *stack_0 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_1 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_2 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_3 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_4 = alloc_one_mem_part();

	stack_0->part_status = INUSE_FULL ;
	stack_1->part_status = INUSE_FULL ;
	stack_2->part_status = INUSE_FULL ;
	stack_3->part_status = INUSE_FULL ;
	stack_4->part_status = INUSE_FULL ;

	taskCreate(&Task[0] ,&usertask0 ,stack_0->mempart_top_ptr-TASK_STACK_SIZE);
	taskCreate(&Task[1] ,&usertask1 ,stack_1->mempart_top_ptr-TASK_STACK_SIZE);
	taskCreate(&Task[2] ,&usertask2 ,stack_2->mempart_top_ptr-TASK_STACK_SIZE);
	taskCreate(&Task[3] ,&usertask3 ,stack_3->mempart_top_ptr-TASK_STACK_SIZE);
	taskCreate(&Task[4] ,&usertask4 ,stack_4->mempart_top_ptr-TASK_STACK_SIZE);

	//Init Task queue
	for(int32_t i =0 ;i<5 ;i++) task_enqueue(&Task[i]) ; 

/************************************************************************************************/
	for(;;) ;
}