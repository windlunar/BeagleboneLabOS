/**
 * U-Boot 2017.01 (Sep 13 2020 - 04:52:37 +0800)   
 * 
 * Need to close the watchdog ,or the BBB will reset itself after 50 seconds
 */ 
#ifndef __TASK_H_
#define __TASK_H_

#include "usr_led.h"
#include "common.h"
#include "uart.h"
#include "print.h"
#include "syscall.h"
#include "debug.h"



void usertask1(void);

void usertask2(void);

uint32 *userTaskInit(uint32 *userTaskStack ,int stackSize ,void (*taskFunc)() ) ;

#endif

