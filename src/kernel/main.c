/**
 * U-Boot version: 2017.01
 */ 

#include "../driver/usr_led.h"
#include "../common.h"
#include "../driver/uart.h"
#include "printk.h"
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
	printk("\r\nKernel Init start...\r\n") ;
	printk("\r\nInit bss...\r\n") ;
	bss_init() ;


	/**
	 * mmu
	 */ 	
	_memset((void *)KERN_PADDR_PTR, 0, BLK_SIZE) ;
	mmu_init() ;

	printk("Enable MMU.\r\n") ;


	/**
	 * Init Timer
	 */ 
	printk("sp : %x ---CPSR : %x\r\n" ,READ_SP() ,READ_CPSR());
	printk("Exception Vector Base = %x\r\n",get_ivt_base());
	printk("kernel_end address :%x\r\n" ,KERNEL_END_VADDR) ;

	usrled_init();
	printk("\nInitialize user leds...\r\n") ;

	irq_init();
	printk("Init interrupt.\r\n");

	ostick_init(DMTIMER0_BASE_PTR_t);
	enable_ostick(IRQ_NUM_TIMER0) ;
	printk("Init Timer0 to switch tasks.\r\n");


	/**
	 * Init memory-page lists and files-like system
	 */ 
	kpage_struct_init();
	page_list_init();
	kpage_blks_init() ;
	
	file_in_ram_init() ;


	/**
	 * Test
	 */ 


	/**
	 * Init Task First thread :Shell
	 */ 
	printk("Init Tasks Shell...\r\n");

	task_init() ;
	
	/** Init the first Task */
	do_task_create(LOWEST_PRIORITY ,&main_shell) ;


	/**
	 * Start Sched
	 */ 
	printk("Sched Starting...\r\n");

	disable_watchdog(WATCHDOG_BASE) ;

	/* 設定要跳進去sched()的context */
	set_first_sched();


	/** 
	 * Jump to run_first_sched() in task.c.
	 * Never back
	 */ 
	call_sched((uint32_t)schedFuncContextSPtr) ;

	for(;;);

	return 0;
}