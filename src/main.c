/**
 * U-Boot 2017.01 (Sep 13 2020 - 04:52:37 +0800)   
 * 
 * Need to close the watchdog ,or the BBB will reset itself after 50 seconds
 */ 


#include "usr_led.h"
#include "common.h"
#include "uart.h"
#include "print.h"
#include "syscall.h"
#include "debug.h"



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


int kernal_entry (void)
{
	/* Initialization of process stack.
	 * r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
     * 
     * push : 將data放到sp-4的位址
     * pop : 將sp的data取出,然後sp+4
     * 所以sp指向 stack中最上面的那一個data
     * 
     * usertask_stack_start[0] ~ [9]依次存放 :
     * r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr
     * 所以要把 usertask的entry位址放在 usertask_stack_start[13] 的位址上
     */
	uint32 user_task1[256] ;
	uint32 *usertask1_stack_top = user_task1 + 256 ;
	uint32 *usertask1_stack_start = usertask1_stack_top - 16;
	usertask1_stack_start[9] = (uint32) &usertask1;

	uint32 user_task2[256] ;
	uint32 *usertask2_stack_top = user_task2 + 256 ;
	uint32 *usertask2_stack_start = usertask2_stack_top - 16;
	usertask2_stack_start[9] = (uint32) &usertask2;

	
	kprintf("\r\nWelcome to my very first Beaglebone's mini toy OS Kernal!\r\n") ;

	kprintf("\nInitialize user leds...\r\n") ;
	usrLed_Init() ;
	

	kprintf("cpsr : %x ---sp : %x ---CP15 : %x ---Exception Vector Base : %x \r\n"
			,READ_CPSR() ,READ_SP() ,READ_CP15_c1() ,READ_VECTOR_BASE());


	kprintf("Multitask Starting...\r\n");
	kprintf("\nStart to blinking user leds...\r\n") ;

	while(1){
		usertask1_stack_start = task_activate(usertask1_stack_start);
    	kprintf("Back to the kernal. #1\r\n") ;

		usrLed_blink() ;

    	usertask2_stack_start = task_activate(usertask2_stack_start);
    	kprintf("Back to the kernal. #2\r\n") ;

		usrLed_blink() ;
	}
   	return 0;
}

