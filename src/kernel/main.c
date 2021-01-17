/**
 * U-Boot version: 2017.01
 * 
 * BBB will reset itself after 50 seconds by watchdog
 */ 


#include "../driver/usr_led.h"
#include "../common.h"
#include "../driver/uart.h"
#include "../klib/print.h"
#include "../userproc/user_task.h"
#include "syscall.h"
#include "debug.h"
#include "task.h"
#include "../driver/timer.h"
#include "interrupt.h"
#include "../driver/rtc_reg.h"
#include "../driver/cm_per.h"
#include "../driver/gpio_reg.h"
#include "../klib/queue.h"
#include "memory.h"
#include "../klib/mem.h"



int kernal_entry(void)
{
/*************************************************************************************************
 * Initial 4 user Leds ,IRQ Interrupt ,Timer0 
 *************************************************************************************************/
	kprintf("\r\nWelcome to Beaglebone black OS!\r\n") ;

	kprintf("sp : %x ---CP15_c1 : %x\r\n" ,READ_SP() ,READ_CP15_c1());
	kprintf("CPSR register %x\r\n", readCpsr());
	kprintf("Exception Vector Base = %x\r\n",getIntVectorAddr());
	kprintf("kernel_end address :%p\r\n" ,kernal_end) ;
	kprintf("First part of memeory address start at :%p\r\n" ,FIRST_PART_PTR) ;

	usrLedInit();
	kprintf("\nInitialize user leds...\r\n") ;

	interrupt_init();
	kprintf("Init interrupt.\r\n");

	OsTickInit(DMTIMER0_BASE_PTR_t);
	enableOsTick(IRQ_NUM_TIMER0) ;

	kprintf("Init Timer0 to switch tasks.\r\n");

	//timer_init(DMTIMER2_BASE_PTR_t ,1000);
	//enableTimerAndBindISR(IRQ_NUM_TIMER2 ,timer2_ISR);

	//kprintf("Init Timer2.\r\n");
/*************************************************************************************************
 * For test
*************************************************************************************************/
	mem_parts_list_init();
	print_free_part_list_start_addr();

	kprintf("Test alloc\r\n") ;

	PART_INFO_t *mem_part0 = alloc_one_mem_part() ;
	PART_INFO_t *mem_part1 = alloc_one_mem_part() ;
	PART_INFO_t *mem_part2 = alloc_one_mem_part() ;

	print_free_part_list_start_addr();
	print_inuse_part_list_start_addr();

	kprintf("Test free\r\n") ;

	free_part_mem(mem_part0) ;
	free_part_mem(mem_part1) ;

	print_free_part_list_start_addr();
	print_inuse_part_list_start_addr();

	//for(;;) ;
/*************************************************************************************************
 * Init Tasks 
 *************************************************************************************************/

	for(int32_t id =0 ;id<TASK_NUM ;id++)
	{
		userTask[id].usrTaskContextSPtr = NULL ;
		userTaskFuncsVector[id] = NULL ;
	}

	userTaskFuncsVector[0] = usertask0 ; 
	userTaskFuncsVector[1] = usertask1 ; 
	userTaskFuncsVector[2] = usertask2 ; 
	userTaskFuncsVector[3] = usertask3 ; 
	userTaskFuncsVector[4] = usertask4 ; 

	for(int32_t id =0 ;id<TASK_NUM ;id++) userTasksInit(id ,&userTask[id] ,userTaskFuncsVector[id]);

	//Init Task queue
	queueInit(&taskReadyQ ,TASK_NUM) ;
	for(int32_t id =0 ;id<TASK_NUM ;id++) enQueue(&taskReadyQ, &userTask[id]);

/*************************************************************************************************/

	kprintf("Init Tasks.\r\n");
	kprintf("Tasks Starting...\r\n");

	//設定要跳進去sched()的context
	schedFuncContextPrepare();

	//Jump to sched() in task.c
	//跳到 sched後不再回來 ,原來在kernel_entry的環境全部丟棄
	_call_sched((uint32_t)schedFuncContextSPtr) ;

/*************************************************************************************************/
	for(;;);

	return 0;
}