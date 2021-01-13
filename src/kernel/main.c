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


uint32_t *usertask1_stack_start = NULL ;
uint32_t *usertask2_stack_start = NULL ;
uint32_t *usertask3_stack_start = NULL ;

void sched(void)
{
	while(1)
	{
		
		usertask1_stack_start = userTaskRun(usertask1_stack_start);
    	kprintf("		Back to kernal mode. #1\r\n\r\n") ;

    	usertask2_stack_start = userTaskRun(usertask2_stack_start);
    	kprintf("		Back to kernal mode. #2\r\n\r\n") ;

		usertask3_stack_start = userTaskRun(usertask3_stack_start);
    	kprintf("		Back to kernal mode. #3\r\n\r\n") ;
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

	timer_init(DMTIMER0_BASE_PTR_t ,50);
	enableTimerAndBindISR(IRQ_NUM_TIMER0 ,timer0_ISR);

	kprintf("Init Timer0 to switch tasks.\r\n");

/*************************************************************************************************
 * Init Tasks 
 *************************************************************************************************/
	usertask1_stack_start = userTaskInit((uint32_t *)&task_stack[0] ,TASK_STACK_SIZE ,&usertask1);
	usertask2_stack_start = userTaskInit((uint32_t *)&task_stack[1] ,TASK_STACK_SIZE ,&usertask2);
	usertask3_stack_start = userTaskInit((uint32_t *)&task_stack[2] ,TASK_STACK_SIZE ,&usertask3);
/*************************************************************************************************/
	kprintf("Init Tasks.\r\n");
	kprintf("Tasks Starting...\r\n");

	sched() ;
	for(;;) ;

	return 0;
}