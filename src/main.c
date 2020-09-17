/*Need to close the watchdog ,or the BBB will reset itself after 50 seconds*/

#include "usr_led.h"
#include "common.h"
#include "uart.h"
#include "print.h"


int _main (void)
{
	char testvar = 0x41 ;
	usrLed_Init() ;
	uart_tx_str(UART0_PTR ,"\nWelcome to my very first Beaglebone OS Kernal!\r\n" ,49) ;
	terminal_printC(0x40) ;
	kprintf("print testvar = %c\r\n" ,testvar) ;
	while(1){
		usrLed_blink() ;
	}
   return 0;
}

