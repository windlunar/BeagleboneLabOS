/*Need to close the watchdog ,or the BBB will reset itself after 50 seconds*/
////
#include "usr_led.h"
#include "common.h"
#include "uart.h"
#include "print.h"



int _main (void)
{
	kprintf("\nWelcome to my very first Beaglebone's mini toy OS Kernal!\r\n") ;

	kprintf("\nInitialize user leds...\r\n") ;
	usrLed_Init() ;
	kprintf("Initialize user leds done!.\r\n") ;

	kprintf("\nStart to blinking user leds...\r\n") ;

	while(1){
		usrLed_blink() ;
	}
   return 0;
}

