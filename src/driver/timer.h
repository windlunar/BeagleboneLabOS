/**
 * Define registers of Timer
 */  

#ifndef __TIMER_H_
#define __TIMER_H_

#include "../common.h"

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

    //TIMER_COUNTER
    uint32 TCRR ;           // offset = 0x3c

    // LOAD_VALUE: Timer counter value loaded on overflow in auto-reload mode or on
    // TTGR write access
    uint32 TLDR ;           // offset = 0x40
    uint32 TTGR ;           // offset = 0x44

    /**
     * TWPS Register Field Descriptions:
     * bit4 | W_PEND_TMAR | R/W | 0h | When equal to 1, a write is pending to the TMAR register
     * bit3 | W_PEND_TTGR | R/W | 0h | When equal to 1, a write is pending to the TTGR register
     * bit2 | W_PEND_TLDR | R/W | 0h | When equal to 1, a write is pending to the TLDR register
     * bit1 | W_PEND_TCRR | R/W | 0h | When equal to 1, a write is pending to the TCRR register
     * bit0 | W_PEND_TCLR | R/W | 0h | When equal to 1, a write is pending to the TCLR register
     */ 
    uint32 TWPS ;           // offset = 0x48
    uint32 TMAR ;           // offset = 0x4c
    uint32 TCAR1 ;          // offset = 0x50
    uint32 TSICR ;          // offset = 0x54
    uint32 TCAR2 ;          // offset = 0x58
}DMTIMER_T ;



#define DMTIMER0_BASE    0x44E05000
#define DMTIMER0_BASE_PTR_t           ((volatile DMTIMER_T *)DMTIMER0_BASE)

#define DMTIMER7_BASE    0x4804A000
#define DMTIMER7_BASE_PTR_t           ((volatile DMTIMER_T *)DMTIMER7_BASE)
/*****************************************************************************************/

void timer_init(volatile DMTIMER_T *DMTIMER_struct_ptr ,uint32_t msecs);
void timerDisable(volatile DMTIMER_T *DMTIMER_struct_ptr);
void timer_ISR_bind(int32_t IRQ_ID ,void (*handler)(void));
void timer_ISR_unbind(int32_t IRQ_ID);

#endif