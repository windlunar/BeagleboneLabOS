#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"

TASK_t Task[5] ;
uint32_t task_stack[5][TASK_STACK_SIZE] ;

void main_origin()
{
	taskCreate(&Task[0] ,&usertask0 ,task_stack[0]);
	taskCreate(&Task[1] ,&usertask1 ,task_stack[1]);
	taskCreate(&Task[2] ,&usertask2 ,task_stack[2]);
	taskCreate(&Task[3] ,&usertask3 ,task_stack[3]);
	taskCreate(&Task[4] ,&usertask4 ,task_stack[4]);

	//Init Task queue
	for(int32_t i =0 ;i<5 ;i++) enQueue(&taskReadyQ, &Task[i]);

	for(;;);
}