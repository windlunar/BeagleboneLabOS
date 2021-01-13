
#include "user_task.h"
#include "../lib/print.h"
#include "../driver/uart.h"



void usertask1(void){
	kprintf("Starting User Task 1 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 1 #%d\r\n" ,k);
    	svc_call();
		k++ ;
		//uart_putC(UART0_PTR ,0x30) ;
		//uart_putC(UART0_PTR ,'\r') ;
		//uart_putC(UART0_PTR ,'\n') ;
	}
}

void usertask2(void){
	kprintf("Starting User Task 2 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 2 #%d\r\n" ,k);
    	//svc_call();
		k++ ;
		//uart_putC(UART0_PTR ,0x31) ;
		//uart_putC(UART0_PTR ,'\r') ;
		//uart_putC(UART0_PTR ,'\n') ;
	}
}

void usertask3(void){
	kprintf("Starting User Task 3 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 3 #%d\r\n" ,k);
    	svc_call();
		k++ ;
		//uart_putC(UART0_PTR ,0x32) ;
		//uart_putC(UART0_PTR ,'\r') ;
		//uart_putC(UART0_PTR ,'\n') ;
	}
}


void usertask4(void){
	kprintf("Starting User Task 4 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 4 #%d\r\n" ,k);
    	svc_call();
		k++ ;
		//uart_putC(UART0_PTR ,0x32) ;
		//uart_putC(UART0_PTR ,'\r') ;
		//uart_putC(UART0_PTR ,'\n') ;
	}
}


void usertask0(void){
	kprintf("Starting User Task 0 \r\n");

	int k = 0 ;

	while(1)
	{
		kprintf("User Task 0 #%d\r\n" ,k);
    	//svc_call();
		k++ ;
		//uart_putC(UART0_PTR ,0x32) ;
		//uart_putC(UART0_PTR ,'\r') ;
		//uart_putC(UART0_PTR ,'\n') ;
	}
}

