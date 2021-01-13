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




void sched(void)
{
	//choose a task to run
	for(int32_t id =0 ;id<TASK_NUM ;id++)
	{
		if(userTask[id].taskStatus == TASK_READY)
		{
			userTask[id].taskStatus = TASK_RUNNING ;

			userTask[id].userTaskStackPtr = userTaskRun(userTask[id].userTaskStackPtr);

			userTask[id].taskStatus = TASK_READY ;
			kprintf("		Back to kernal sched from taskid=%d ,Run another task\r\n\r\n",id) ;
			kprintf("		"); readCpsrMode();
		}
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

	timer_init(DMTIMER0_BASE_PTR_t ,20);
	enableTimerAndBindISR(IRQ_NUM_TIMER0 ,timer0_ISR);

	kprintf("Init Timer0 to switch tasks.\r\n");

/*************************************************************************************************
 * Init Tasks 
 *************************************************************************************************/
	userTaskVector[0] = usertask0 ; 
	userTaskVector[1] = usertask1 ; 
	userTaskVector[2] = usertask2 ; 
	userTaskVector[3] = usertask3 ; 
	userTaskVector[4] = usertask4 ; 

	for(int32_t id =0 ;id<TASK_NUM ;id++)
	{
		userTask[id].userTaskStackPtr = NULL ;
	}

	for(int32_t id =0 ;id<TASK_NUM ;id++)
	{
		userTaskInit(id ,&userTask[id] ,userTaskVector[id]);
	}
/*************************************************************************************************/
	kprintf("Init Tasks.\r\n");
	kprintf("Tasks Starting...\r\n");

	for(;;)
	{
		sched() ;
	}

	return 0;
}