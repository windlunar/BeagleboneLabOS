
#ifndef __USR_LED_H_
#define __USR_LED_H_


#include "../common.h"


void init_usrled_asm() ;
void blink_usrled_asm() ;
void usrled_init() ;
void usrled_blink() ;
void toggle_usrled(uint32_t usr_led_num);


#endif