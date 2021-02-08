/**
 * U-Boot version: 2017.01
 * 
 * BBB will reset itself after 50 seconds by watchdog
 */ 

//#include "../klib/queue.h"
//#include "../klib/mem.h"
#include "../driver/usr_led.h"
#include "../common.h"
#include "../driver/uart.h"
#include "kprint.h"
#include "../userproc/usrtasks.h"
#include "syscall.h"
#include "debug.h"
#include "task.h"
#include "../driver/timer.h"
#include "interrupt.h"
#include "../driver/rtc_reg.h"
#include "../driver/cm_per.h"
#include "../driver/gpio_reg.h"
#include "memory.h"
#include "mmu.h"
#include "file.h"
#include "../klib/mem.h"
#include "../klib/queue.h"



int kernal_entry(void)
{

/***************************************************************************************/
// mmu	
/***************************************************************************************/
	_memset((void *)KERN_PADDR_PTR, 0, BLK_SIZE) ;
	//mmu_init() ;

	printk("Enable MMU.\r\n") ;

/***************************************************************************************/
// Init Task First thread :Shell
/***************************************************************************************/
	printk("\r\nKernel Init start...\r\n") ;

	printk("sp : %x ---CP15_c1 : %x\r\n" ,READ_SP() ,READ_CP15_c1());
	printk("CPSR register %x\r\n", READ_CPSR());
	printk("Exception Vector Base = %x\r\n",getIntVectorAddr());
	printk("kernel_end address :%x\r\n" ,KERNEL_END_VADDR) ;
	printk("First page of memeory address start at :%p\r\n" ,KERN_VADDR_PTR) ;

	usrLedInit();
	printk("\nInitialize user leds...\r\n") ;

	interrupt_init();
	printk("Init interrupt.\r\n");

	OsTickInit(DMTIMER0_BASE_PTR_t);
	enableOsTick(IRQ_NUM_TIMER0) ;
	printk("Init Timer0 to switch tasks.\r\n");

/***************************************************************************************/
// Init memory-page lists and files
/***************************************************************************************/
	kpage_struct_init();
	page_list_init();

	kpage_blks_init() ;
	
	file_in_ram_init() ;

/***************************************************************************************/
// Test
/***************************************************************************************/
/*	
	struct QUEUE q ;
	kq_init(&q ,10) ;
	enqueue(&q ,'a') ;
	enqueue(&q ,'b') ;
	enqueue(&q ,'c') ;
	enqueue(&q ,'d') ;
	enqueue(&q ,'e') ;
	enqueue(&q ,'f') ;
	enqueue(&q ,'g') ;

	dump_queue(&q) ;

	uint8_t byte = 0 ;
	byte = dequeue(&q);
	printk("%c " ,byte) ;
	byte = dequeue(&q);
	printk("%c " ,byte) ;
	byte = dequeue(&q);
	printk("%c " ,byte) ;

	dump_queue(&q) ;

	enqueue(&q ,'h') ;
	enqueue(&q ,'i') ;
	enqueue(&q ,'j') ;
	enqueue(&q ,'k') ;
	enqueue(&q ,'l') ;
	enqueue(&q ,'m') ;

	dump_queue(&q) ;
	enqueue(&q ,'n') ;
	kq_delete(&q) ;

	for(;;) ;
*/
/***************************************************************************************/
// Init Task First thread :Shell
/***************************************************************************************/
	printk("Init Tasks Shell...\r\n");

	task_init() ;
	
	//Init the first thread
	do_ktaskCreate(LOWEST_PRIORITY ,&main_shell) ;

/***************************************************************************************/
// Start Sched
/***************************************************************************************/
	printk("Sched Starting...\r\n");

	disable_watchdog(WATCHDOG_BASE) ;

	//設定要跳進去sched()的context
	schedFuncContextPrepare();

	//Jump to sched() in task.c
	//跳到 sched後不再回來
	_call_sched((uint32_t)schedFuncContextSPtr) ;

/***************************************************************************************/
	for(;;);

	return 0;
}