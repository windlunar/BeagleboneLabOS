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
#include "interrupt.h"
#include "../driver/rtc_reg.h"
#include "../driver/cm_per.h"
#include "../driver/gpio_reg.h"


uint32_t *usertask_stack_start[TASK_NUM] ;

void sched(void)
{
	while(1)
	{
		
		usertask_stack_start[0] = userTaskRun(usertask_stack_start[0]);
    	kprintf("		Back to kernal mode. #0\r\n\r\n") ;

    	usertask_stack_start[1] = userTaskRun(usertask_stack_start[1]);
    	kprintf("		Back to kernal mode. #1\r\n\r\n") ;

		usertask_stack_start[2] = userTaskRun(usertask_stack_start[2]);
    	kprintf("		Back to kernal mode. #2\r\n\r\n") ;

		usertask_stack_start[3] = userTaskRun(usertask_stack_start[3]);
    	kprintf("		Back to kernal mode. #3\r\n\r\n") ;

		usertask_stack_start[4] = userTaskRun(usertask_stack_start[4]);
    	kprintf("		Back to kernal mode. #4\r\n\r\n") ;

	}
}


int kernal_entry(void)
{
/*************************************************************************************************
 * Initial 4 user Leds ,IRQ Interrupt ,Timer0 
 *************************************************************************************************/
	kprintf("\r\nWelcome to Beaglebone black OS!\r\n") ;

	kprintf("sp : %x ---CP15_c1 : %x\r\n" ,READ_SP() ,READ_CP15_c1());
	kprintf("CPSR register %x\r\n", readCpsr());
	kprintf("Exception Vector Base = %x\r\n",getIntVectorAddr());

	usrLedInit();
	kprintf("\nInitialize user leds...\r\n") ;

	interrupt_init();
	kprintf("Init interrupt.\r\n");

	//timer_init(DMTIMER0_BASE_PTR_t ,20);
	//enableTimerAndBindISR(IRQ_NUM_TIMER0 ,timer0_ISR);

	kprintf("Init Timer0 to switch tasks.\r\n");

/*************************************************************************************************
 * Init Tasks 
 *************************************************************************************************/
	for(int32_t i =0 ;i<5;i++){
		usertask_stack_start[i] = NULL ;
	}
	usertask_stack_start[0] = userTaskInit((uint32_t *)&task_stack[0] ,TASK_STACK_SIZE ,&usertask0);
	usertask_stack_start[1] = userTaskInit((uint32_t *)&task_stack[1] ,TASK_STACK_SIZE ,&usertask1);
	usertask_stack_start[2] = userTaskInit((uint32_t *)&task_stack[2] ,TASK_STACK_SIZE ,&usertask2);
	usertask_stack_start[3] = userTaskInit((uint32_t *)&task_stack[3] ,TASK_STACK_SIZE ,&usertask3);
	usertask_stack_start[4] = userTaskInit((uint32_t *)&task_stack[4] ,TASK_STACK_SIZE ,&usertask4);
/*************************************************************************************************/
	kprintf("Init Tasks.\r\n");
	kprintf("Tasks Starting...\r\n");

	sched() ;
	for(;;) ;

	return 0;
}