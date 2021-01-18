
#include "usrtasks.h"
#include "../klib/print.h"
#include "../driver/uart.h"
#include "../kernel/debug.h"

#define LOOP_NUM	50

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




