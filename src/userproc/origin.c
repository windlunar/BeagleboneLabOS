#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"

TASK_t Task[5] ;
//uint32_t task_stack[5][TASK_STACK_SIZE] ;

void main_origin()
{
	MEM_PART_INFO_t *stack_0 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_1 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_2 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_3 = alloc_one_mem_part();
	MEM_PART_INFO_t *stack_4 = alloc_one_mem_part();

	taskCreate(&Task[0] ,&usertask0 ,stack_0->mempart_top_ptr);
	taskCreate(&Task[1] ,&usertask1 ,stack_1->mempart_top_ptr);
	taskCreate(&Task[2] ,&usertask2 ,stack_2->mempart_top_ptr);
	taskCreate(&Task[3] ,&usertask3 ,stack_3->mempart_top_ptr);
	taskCreate(&Task[4] ,&usertask4 ,stack_4->mempart_top_ptr);

	//Init Task queue
	for(int32_t i =0 ;i<5 ;i++) task_enqueue(&Task[i]) ; 

	for(;;) ;
}