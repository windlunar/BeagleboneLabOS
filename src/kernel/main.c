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






void __attribute__((optimize("O0"))) delay(uint32_t num)
{
	for (uint32_t count = 0 ;count < num ;count++) ;
}





int32_t readCpsr(){
	int32_t cpsr;

    asm volatile("mrs %[v], CPSR" : : [v] "r" (cpsr));
	return cpsr ;
		
}


void timer0handler(void)
{
	(DMTIMER0_BASE_PTR_t->IRQSTATUS) = (1 << 1);
	usrLedToggle(3);
	usrLedToggle(2);
	usrLedToggle(1);
	usrLedToggle(0);
}



int kernal_entry(void)
{
		kprintf("\r\nWelcome to Beaglebone black OS!\r\n") ;

		kprintf("cpsr : %x ---sp : %x ---CP15 : %x\r\n---Exception Vector Base : %x \r\n"
				,READ_CPSR() ,READ_SP() ,READ_CP15_c1() ,READ_VECTOR_BASE());

		kprintf("CPSR register %x\r\n", readCpsr());
		kprintf("irqs_handlers_address = %x\r\n",getIntVectorAddr());

		kprintf("Tasks Starting...\r\n");

	usrLedInit();
		kprintf("\nInitialize user leds...\r\n") ;

	interrupt_init();
	timer_ISR_bind(IRQ_NUM_TIMER0 ,timer0handler);
	timer_init(DMTIMER0_BASE_PTR_t ,1000);

/******************************************************************************************
 * Init Tasks 
 ******************************************************************************************/
	uint32 *usertask1_stack_start = userTaskInit((uint32 *)&task_stack[0] 
												,TASK_STACK_SIZE 
												,&usertask1) ;

	uint32 *usertask2_stack_start = userTaskInit((uint32 *)&task_stack[1] 
												,TASK_STACK_SIZE 
												,&usertask2) ;
												
	uint32 *usertask3_stack_start = userTaskInit((uint32 *)&task_stack[2] 
												,TASK_STACK_SIZE 
												,&usertask3) ;
/******************************************************************************************/	

	while(1)
	{
	/******************************************************************************************
	 * Switch Tasks 
	 ******************************************************************************************/
		usertask1_stack_start = userTaskRun(usertask1_stack_start);
    	kprintf("		Back to kernal mode. #1\r\n\r\n") ;

    	usertask2_stack_start = userTaskRun(usertask2_stack_start);
    	kprintf("		Back to kernal mode. #2\r\n\r\n") ;

		usertask3_stack_start = userTaskRun(usertask3_stack_start);
    	kprintf("		Back to kernal mode. #3\r\n\r\n") ;
	/******************************************************************************************/
	}

	timerDisable(DMTIMER0_BASE_PTR_t);
	timer_ISR_unbind(IRQ_NUM_TIMER0);
	disableIrqThroughCpsr();

	return 0;
}