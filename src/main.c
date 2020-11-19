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
#include "task.h"
#include "user_task.h"




int kernal_entry (void)
{
	uint32 user_task1[256] ;
	uint32 *usertask1_stack_start = userTaskInit(user_task1 ,256 ,&usertask1) ;

	uint32 user_task2[256] ;
	uint32 *usertask2_stack_start = userTaskInit(user_task2 ,256 ,&usertask2) ;

	
	
	kprintf("\r\nWelcome to my Beaglebone's mini OS Kernal!\r\n") ;

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

