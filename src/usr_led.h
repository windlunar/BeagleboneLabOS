
#ifndef __USR_LED_H_
#define __USR_LED_H_

#include "common.h"


#define GPIO1_BASE		0x4804C000
#define GPIO_SYSCONFIG	0x10
#define GPIO_SYSSTATUS	0x114
#define GPIO_OE			0x134
#define GPIO_CLRDATAOUT	0x190
#define GPIO_SETDATAOUT	0x194

//Clock Module Registers
#define CM_PER_BASE		0x44e00000
#define CM_PER_GPIO1	0xAC

#define TIME 500000

volatile uint32 ra;

void usrLed_Init_asm() ;
void usrLed_blink_asm() ;
void usrLed_Init() ;
void usrLed_blink() ;

#endif