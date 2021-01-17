
#include "user_task.h"
#include "../klib/print.h"
#include "../driver/uart.h"
#include "../kernel/debug.h"


void usertask0(void){
	kprintf("Starting User Task 0 \r\n");

	int k = 0 ;

	while(1)
	{	
		kprintf("Task 0 #%d\r\n" ,k);
		//readCpsrMode();
    	//syscall_print_hello();	
		//syscall_yield() ;	
		//readCpsrMode();
		//for(int i =0 ;i<100000;i++) ;
		k++ ;
	}
}


void usertask1(void){
	kprintf("Starting User Task 1 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("Task 1 #%d\r\n" ,k);
		//readCpsrMode();
    	syscall_print_hello(&k);
		//syscall_yield() ;			
		//readCpsrMode();
		//for(int i =0 ;i<100000;i++) ;
		k++ ;
	}
}

void usertask2(void){
	kprintf("Starting User Task 2 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("Task 2 #%d\r\n" ,k);
		//readCpsrMode();
    	//syscall_print_hello();	
		syscall_yield() ;			
		//readCpsrMode();
		//for(int i =0 ;i<100000;i++) ;
		k++ ;
	}
}

void usertask3(void){
	kprintf("Starting User Task 3 \r\n");

	int k = 0 ;
	int tid = 0 ;

	while(1)
	{
		syscall_get_tid(&tid) ;	
		kprintf("Task 3 #%d ,task id=%d\r\n" ,k ,tid);
		//readCpsrMode();
    	//syscall_print_hello();	
		//syscall_yield() ;	
		//readCpsrMode();
		//for(int i =0 ;i<100000;i++) ;
		k++ ;
	}
}


void usertask4(void){
	kprintf("Starting User Task 4 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("Task 4 #%d\r\n" ,k);
		//readCpsrMode();
    	//syscall_print_hello();	
		syscall_yield() ;		
		//readCpsrMode();
		//for(int i =0 ;i<100000;i++) ;
		k++ ;
	}
}




