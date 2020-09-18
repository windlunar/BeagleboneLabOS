/*Need to close the watchdog ,or the BBB will reset itself after 50 seconds*/
////
#include "usr_led.h"
#include "common.h"
#include "uart.h"
#include "print.h"


int _main (void)
{
	char testvar1 = 'A' ;
	char testvar2 = 'B' ;
	char testvar3 = 'C' ;
	usrLed_Init() ;
	uart_tx_str(UART0_PTR ,"\nWelcome to my very first Beaglebone OS Kernal!\r\n" ,49) ;
	kprintf("print testvar1 = %c, testvar2 = %c, testvar3 = %c\r\n" ,testvar1,testvar2,testvar3) ;
	while(1){
		usrLed_blink() ;
	}
   return 0;
}

