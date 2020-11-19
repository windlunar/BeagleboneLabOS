/**
 * U-Boot 2017.01 (Sep 13 2020 - 04:52:37 +0800)   
 * 
 * Need to close the watchdog ,or the BBB will reset itself after 50 seconds
 */ 
#ifndef __USER_TASK_H_
#define __USER_TASK_H_


#include "task.h"



void usertask1(void);

void usertask2(void);


#endif

