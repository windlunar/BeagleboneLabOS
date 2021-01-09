/**
 * Define registers of Timer
 */  

#ifndef __TIMER_H_
#define __TIMER_H_

#include "../common.h"


#define DMTIMER0_BASE    0x44E05000
#define DMTIMER0_BASE_PTR_t           ((volatile DMTIMER0_T *)DMTIMER0_BASE)
                                               

typedef struct{
    uint32 TIDR ;           // offset = 0x00
    uint32 NON_USE1 ;       // offset = 0x04
    uint32 NON_USE2 ;       // offset = 0x08
    uint32 NON_USE3 ;       // offset = 0x0c
    uint32 TIOCP_CFG ;      // offset = 0x10
    uint32 NON_USE4 ;       // offset = 0x14
    uint32 NON_USE5 ;       // offset = 0x18
    uint32 NON_USE6 ;       // offset = 0x1c
    uint32 IRQ_EOI ;        // offset = 0x20
    uint32 IRQSTATUS_RAW ;  // offset = 0x24
    uint32 IRQSTATUS ;      // offset = 0x28
    uint32 IRQENABLE_SET ;  // offset = 0x2c
    uint32 IRQENABLE_CLR ;  // offset = 0x30
    uint32 IRQWAKEEN ;      // offset = 0x34
    uint32 TCLR ;           // offset = 0x38
    uint32 TCRR ;           // offset = 0x3c
    uint32 TLDR ;           // offset = 0x40
    uint32 TTGR ;           // offset = 0x44
    uint32 TWPS ;           // offset = 0x48
    uint32 TMAR ;           // offset = 0x4c
    uint32 TCAR1 ;          // offset = 0x50
    uint32 TSICR ;          // offset = 0x54
    uint32 TCAR2 ;          // offset = 0x58
}DMTIMER0_T ;



void dmtimer0PrintRegs() ;


#endif