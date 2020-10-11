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



void usertask(void)
{
	kprintf("User Task #1\r\n");
	kprintf("sp : %x \r\n",READ_SP());
	for(int i = 0; i<100000 ; i++) ;
    sys_call();

	kprintf("HERE! User Task #2\r\n");
	kprintf("sp : %x \r\n",READ_SP());
	for(int i = 0; i<100000 ; i++) ;
    sys_call();

	while (1); 
}


int kernal_entry (void)
{
	/* Initialization of process stack.
	 * r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr 
     * 
     * push : 將data放到sp+4的位址
     * pop : 將sp的data取出,然後sp+4
     * 所以sp指向 stack中最上面的那一個data
     * 
     * usertask_stack_start[0] ~ [13]依次存放 :
     * r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr
     * 所以要把 usertask的entry位址放在 usertask_stack_start[13] 的位址上
     */
	uint32 *usertask_stack_top = (uint32 *)0x89000000 ;
	uint32 *usertask_stack_start = usertask_stack_top - 16;
	usertask_stack_start[9] = (uint32) &usertask;


	
	kprintf("\r\nWelcome to my very first Beaglebone's mini toy OS Kernal!\r\n") ;

	kprintf("\nInitialize user leds...\r\n") ;
	usrLed_Init() ;
	kprintf("Initialize user leds done!.\r\n") ;
	

	kprintf("cpsr : %x \r\n",READ_CPSR());
	kprintf("sp : %x \r\n",READ_SP());
	kprintf("CP15 : %x \r\n",READ_CP15_c1());
	kprintf("Vector Base : %x \r\n",READ_VECTOR_BASE());


	kprintf("OS Starting...\r\n");

	kprintf("usertask_stack_start #0 : %x \r\n",(uint32)usertask_stack_start);

	usertask_stack_start = activate(usertask_stack_start);
    kprintf("Back to the kernal. #1\r\n") ;
	kprintf("sp : %x \r\n",READ_SP());
	for(int i = 0 ; i < 16 ; i++){
		kprintf("Addr of usertask_stack_start[%d] #1 : %p ----",i,&usertask_stack_start[i]);
		kprintf("usertask_stack_start[%d] #1 : %x \r\n",i,usertask_stack_start[i]);
	}
	
	

    usertask_stack_start = activate(usertask_stack_start);
    kprintf("Back to the kernal. #2\r\n") ;
	kprintf("sp : %x \r\n",READ_SP());
	kprintf("usertask_stack_start #2 : %x \r\n",(uint32)usertask_stack_start);

	kprintf("\nStart to blinking user leds...\r\n") ;

	while(1){
		usrLed_blink() ;
	}
   	return 0;
}

