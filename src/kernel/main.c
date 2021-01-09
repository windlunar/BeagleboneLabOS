/**
 * U-Boot version: 2017.01
 * 
 * BBB will reset itself after 50 seconds by watchdog
 */ 


#include "../driver/usr_led.h"
#include "../common.h"
#include "../driver/uart.h"
#include "../lib/print.h"
#include "../userproc/user_task.h"
#include "syscall.h"
#include "debug.h"
#include "task.h"
#include "../driver/timer.h"





int kernal_entry (void)
{
	uint32 *usertask1_stack_start = userTaskInit((uint32 *)&task_stack[0] ,TASK_STACK_SIZE ,&usertask1) ;
	uint32 *usertask2_stack_start = userTaskInit((uint32 *)&task_stack[1] ,TASK_STACK_SIZE ,&usertask2) ;
	uint32 *usertask3_stack_start = userTaskInit((uint32 *)&task_stack[2] ,TASK_STACK_SIZE ,&usertask3) ;
	
	kprintf("\r\nWelcome to my Beaglebone black OS!\r\n") ;

	kprintf("\nInitialize user leds...\r\n") ;
	usrLed_Init() ;

	kprintf("cpsr : %x ---sp : %x ---CP15 : %x\r\n---Exception Vector Base : %x \r\n"
			,READ_CPSR() ,READ_SP() ,READ_CP15_c1() ,READ_VECTOR_BASE());


	kprintf("Tasks Starting...\r\n");


	while(1){
		usertask1_stack_start = userTaskRun(usertask1_stack_start);
    	kprintf("		Back to kernal mode. #1\r\n\r\n") ;

		usrLed_blink() ;

    	usertask2_stack_start = userTaskRun(usertask2_stack_start);
    	kprintf("		Back to kernal mode. #2\r\n\r\n") ;

		usrLed_blink() ;

		usertask3_stack_start = userTaskRun(usertask3_stack_start);
    	kprintf("		Back to kernal mode. #3\r\n\r\n") ;

		usrLed_blink() ;
	}
   	return 0;
}

