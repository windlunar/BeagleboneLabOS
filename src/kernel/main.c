/**
 * U-Boot version: 2017.01
 * 
 * BBB will reset itself after 50 seconds by watchdog
 */ 


#include "../driver/usr_led.h"
#include "../common.h"
#include "../driver/uart.h"
#include "../klib/print.h"
#include "../userproc/usrtasks.h"
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
	kprintf("\r\nKernel Init start...\r\n") ;

	kprintf("sp : %x ---CP15_c1 : %x\r\n" ,READ_SP() ,READ_CP15_c1());
	kprintf("CPSR register %x\r\n", readCpsr());
	kprintf("Exception Vector Base = %x\r\n",getIntVectorAddr());
	kprintf("kernel_end address :%p\r\n" ,kernal_end) ;
	kprintf("First area of memeory address start at :%p\r\n" ,FIRST_AREA_PTR) ;

	usrLedInit();
	kprintf("\nInitialize user leds...\r\n") ;

	interrupt_init();
	kprintf("Init interrupt.\r\n");

	OsTickInit(DMTIMER0_BASE_PTR_t);
	enableOsTick(IRQ_NUM_TIMER0) ;

	kprintf("Init Timer0 to switch tasks.\r\n");

/*************************************************************************************************
 * Init Task First thread :Origin
 *************************************************************************************************/
	task_init() ;
	
	//Init the first thread
	taskCreate(&task_origin ,&main_origin ,task_origin_stack ,LOWEST_PRIORITY) ;

	//enqueue the first thread's info structure
	task_enqueue(&task_origin) ;

	kprintf("Init Tasks Origin.\r\n");

/*************************************************************************************************
 * Init memory-area lists
*************************************************************************************************/

	mem_areas_list_init();

/*************************************************************************************************/
	kprintf("Sched Starting...\r\n");

	//設定要跳進去sched()的context
	schedFuncContextPrepare();

	//Jump to sched() in task.c
	//跳到 sched後不再回來
	_call_sched((uint32_t)schedFuncContextSPtr) ;

/*************************************************************************************************/
	for(;;);

	return 0;
}