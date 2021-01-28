
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

	__do_taskCreate(&usertask0) ;
	__do_taskCreate(&usertask1) ;
	__do_taskCreate(&usertask2) ;
	__do_taskCreate(&usertask3) ;
	__do_taskCreate(&usertask4) ;


	for(int32_t i=0 ;i<1000000;i++);

	return 0 ;
}