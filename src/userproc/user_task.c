
#include "user_task.h"
#include "../lib/print.h"



void usertask1(void){
	kprintf("Starting User Task 1 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 1 #%d\r\n" ,k);
    	sys_call();
		k++ ;
	}
}

void usertask2(void){
	kprintf("Starting User Task 2 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 2 #%d\r\n" ,k);
    	sys_call();
		k++ ;
	}
}

void usertask3(void){
	kprintf("Starting User Task 3 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 3 #%d\r\n" ,k);
    	sys_call();
		k++ ;
	}
}




