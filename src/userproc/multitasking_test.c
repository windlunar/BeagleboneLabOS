
#include "usrtasks.h"
#include "../klib/print.h"
#include "../driver/uart.h"
#include "../kernel/debug.h"

#define LOOP_NUM	50

TASK_INFO_t Task[5] ;


void usertask0(void){
	kprintf("Starting User Task 0 \r\n");

	int k = 0 ;
	int tid = 0 ;
	while(k<LOOP_NUM)
	{	
		syscall_get_tid(&tid) ;	
		kprintf("Task 0 #%d ,task id=%d\r\n" ,k ,tid);
		//syscall_yield() ;	
		k++ ;
	}
	syscall_exit();
}


void usertask1(void){
	kprintf("Starting User Task 1 \r\n");

	int k = 0 ;
	int tid = 0 ;
	while(k<LOOP_NUM)
	{
		syscall_get_tid(&tid) ;	
		kprintf("Task 1 #%d ,task id=%d\r\n" ,k ,tid);
    	//syscall_print_hello(&k);
		//syscall_yield() ;			
		k++ ;
	}
	syscall_exit();
}

void usertask2(void){
	kprintf("Starting User Task 2 \r\n");

	int k = 0 ;
	int tid = 0 ;
	while(k<LOOP_NUM)
	{
		syscall_get_tid(&tid) ;	
		kprintf("Task 2 #%d ,task id=%d\r\n" ,k ,tid);
		//syscall_yield() ;			
		k++ ;
	}
	syscall_exit();
}

void usertask3(void){
	kprintf("Starting User Task 3 \r\n");

	int k = 0 ;
	int tid = 0 ;

	while(k<LOOP_NUM)
	{
		syscall_get_tid(&tid) ;	
		kprintf("Task 3 #%d ,task id=%d\r\n" ,k ,tid);
		//syscall_yield() ;	
		k++ ;
	}
	syscall_exit();
}


void usertask4(void){

	kprintf("Starting User Task 4 \r\n");

	int k = 0 ;
	int tid = 0 ;
	while(k<LOOP_NUM)
	{
		syscall_get_tid(&tid) ;	
		kprintf("Task 4 #%d ,task id=%d\r\n" ,k ,tid);
		//syscall_yield() ;		
		k++ ;
	}
	syscall_exit();
}


int multitasking_test(void)
{
	for(int32_t i=0 ;i<200000;i++);
	kprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test multitasking.\r\n") ;

	MEM_PART_INFO_t *mempart = alloc_one_mem_part();
	uint32_t *stack_0 = demand_a_blk(1) ;
	uint32_t *stack_1 = demand_a_blk(1) ;
	uint32_t *stack_2 = demand_a_blk(1) ;
	uint32_t *stack_3 = demand_a_blk(1) ;
	uint32_t *stack_4 = demand_a_blk(1) ;
	mempart->part_status = INUSE_FULL ;

	taskCreate(&Task[0] ,&usertask0 ,stack_0-1);
	taskCreate(&Task[1] ,&usertask1 ,stack_1-1);
	taskCreate(&Task[2] ,&usertask2 ,stack_2-1);
	taskCreate(&Task[3] ,&usertask3 ,stack_3-1);
	taskCreate(&Task[4] ,&usertask4 ,stack_4-1);

	// 將tasks加入ready list中
	for(int32_t i =0 ;i<5 ;i++) task_enqueue(&Task[i]) ; 

	for(int32_t i=0 ;i<1000000;i++);

	return 0 ;
}