
#include "usrtasks.h"
#include "../klib/print.h"
#include "../driver/uart.h"
#include "../kernel/debug.h"
#include "../klib/usyscall.h"


#define LOOP_NUM	50

TASK_INFO_t Task[5] ;


void usertask0(void){
	kprintf("Starting User Task 0 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		kprintf("Task 0 #%d ,task id=%d\r\n" ,k ,tid);
		//_yield() ;	
		k++ ;
	}
	__exit();
}


void usertask1(void){
	kprintf("Starting User Task 1 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		kprintf("Task 1 #%d ,task id=%d\r\n" ,k ,tid);
    	//__print_hello(&k);
		//__yield() ;			
		k++ ;
	}
	__exit();
}

void usertask2(void){
	kprintf("Starting User Task 2 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		kprintf("Task 2 #%d ,task id=%d\r\n" ,k ,tid);
		//__yield() ;			
		k++ ;
	}
	__exit();
}

void usertask3(void){
	kprintf("Starting User Task 3 \r\n");

	int k = 0 ;
	int tid = -1 ;

	while(k<LOOP_NUM)
	{
		tid = __gettid() ;	
		kprintf("Task 3 #%d ,task id=%d\r\n" ,k ,tid);
		//__yield() ;	
		k++ ;
	}
	__exit();
}


void usertask4(void){

	kprintf("Starting User Task 4 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		kprintf("Task 4 #%d ,task id=%d\r\n" ,k ,tid);
		//__yield() ;		
		k++ ;
	}
	__exit();
}


int multitasking_test(void)
{
	for(int32_t i=0 ;i<200000;i++);
	kprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test multitasking.\r\n") ;

	MEM_PART_INFO_t *stack_0 = memPartAlloc();
	MEM_PART_INFO_t *stack_1 = memPartAlloc();
	MEM_PART_INFO_t *stack_2 = memPartAlloc();
	MEM_PART_INFO_t *stack_3 = memPartAlloc();
	MEM_PART_INFO_t *stack_4 = memPartAlloc();

	stack_0->part_status = INUSE_FULL ;
	stack_1->part_status = INUSE_FULL ;
	stack_2->part_status = INUSE_FULL ;
	stack_3->part_status = INUSE_FULL ;
	stack_4->part_status = INUSE_FULL ;

	taskCreate(&Task[0] ,&usertask0 ,stktop2bottom(stack_0->m_top));
	taskCreate(&Task[1] ,&usertask1 ,stktop2bottom(stack_1->m_top));
	taskCreate(&Task[2] ,&usertask2 ,stktop2bottom(stack_2->m_top));
	taskCreate(&Task[3] ,&usertask3 ,stktop2bottom(stack_3->m_top));
	taskCreate(&Task[4] ,&usertask4 ,stktop2bottom(stack_4->m_top));

	// 將tasks加入ready list中
	for(int32_t i =0 ;i<5 ;i++) task_enqueue(&Task[i]) ; 

	for(int32_t i=0 ;i<1000000;i++);

	return 0 ;
}