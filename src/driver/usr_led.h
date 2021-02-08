
#ifndef __USR_LED_H_
#define __USR_LED_H_


#include "../common.h"


void usrLedInitAsm() ;
void usrLedBlinkAsm() ;
void usrLedInit() ;
void usrLedBlink() ;
void usrLedToggle(uint32_t usr_led_num);


#endif