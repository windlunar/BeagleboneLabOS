/**
 * U-Boot 2017.01 (Sep 13 2020 - 04:52:37 +0800)   
 * 
 * Need to close the watchdog ,or the BBB will reset itself after 50 seconds
 */ 


#include "user_task.h"
#include "print.h"



void usertask1(void){
	kprintf("Starting User Task 1 \r\n");

	int k = 0 ;
	while(1){
		kprintf("User Task 1 #%d\r\n" ,k);
		for(int i = 0; i<100000 ; i++) ;
		k++ ;
    	sys_call();
	}
}

void usertask2(void){
	kprintf("Starting User Task 2 \r\n");

	int k = 0 ;
	while(1){
		kprintf("User Task 2 #%d\r\n" ,k);
		for(int i = 0; i<100000 ; i++) ;
		k++ ;
    	sys_call();
	}
}




