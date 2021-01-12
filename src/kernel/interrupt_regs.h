
#ifndef __INTERRUPT_REGS_H_
#define __INTERRUPT_REGS_H_

#include "../common.h"

#define INTC_BASE       0x48200000
#define INTC_BASE_PTR   (volatile uint8_t *)INTC_BASE

#define INTC_MIR_BASE           INTC_MIR0
#define INTC_MIR_CLEAR_BASE     INTC_MIR_CLEAR0
#define INTC_MIR_SET_BASE       INTC_MIR_SET0

#define INTC_ILR_n_BASE_PTR     INTC_BASE_PTR + INTC_ILR_n_BASE
#define INT_ROUTE_IRQ   0
#define INT_ROUTE_FRQ   1
#define NEW_IRQ_AGREE   1



//define offset of regs
#define INTC_REVISION       0x00
#define INTC_SYSCONFIG      0x10
#define INTC_SYSSTATUS      0x14

/**bit 0-6      ActiveIRQ   R/W     Active IRQ number */ 
#define INTC_SIR_IRQ        0x40

#define INTC_SIR_FIQ        0x44
#define INTC_CONTROL        0x48
#define INTC_PROTECTION     0x4c
#define INTC_IDLE           0x50
#define INTC_IRQ_PRIORITY   0x60
#define INTC_FIQ_PRIORITY   0x64
#define INTC_THRESHOLD      0x68
#define INTC_ITR0           0x80

//Interrupt mask
#define INTC_MIR0           0x84
#define INTC_MIR_CLEAR0     0x88
#define INTC_MIR_SET0       0x8c

#define INTC_ISR_SET0       0x90
#define INTC_ISR_CLEAR0     0x94
#define INTC_PENDING_IRQ0   0x98
#define INTC_PENDING_FIQ0   0x9c
#define INTC_ITR1           0xa0

// 0x88 + 0x20 *1
#define INTC_MIR1           0xa4
#define INTC_MIR_CLEAR1     0xa8
#define INTC_MIR_SET1       0xac

#define INTC_ISR_SET1       0xb0
#define INTC_ISR_CLEAR1     0xb4
#define INTC_PENDING_IRQ1   0xb8
#define INTC_PENDING_FIQ1   0xbc
#define INTC_ITR2           0xc0

// 0x88 + 0x20 *2
#define INTC_MIR2           0xc4
#define INTC_MIR_CLEAR2     0xc8
#define INTC_MIR_SET2       0xcc

#define INTC_ISR_SET2       0xd0
#define INTC_ISR_CLEAR2     0xd4
#define INTC_PENDING_IRQ2   0xd8
#define INTC_PENDING_FIQ2   0xdc
#define INTC_ITR3           0xe0

// 0x88 + 0x20 *3
#define INTC_MIR3           0xe4
#define INTC_MIR_CLEAR3     0xe8
#define INTC_MIR_SET3       0xec

#define INTC_ISR_SET3       0xf0
#define INTC_ISR_CLEAR3     0xf4
#define INTC_PENDING_IRQ3   0xf8
#define INTC_PENDING_FIQ3   0xfc

// bit 0: FIQnIRQ
//Interrupt IRQ FiQ mapping
//0h = IntIRQ : Interrupt is routed to IRQ.
//1h = IntFIQ : Interrupt is routed to FIQ (this selection is reserved on
//GP devices).
#define INTC_ILR_n_BASE     0x100   //n = 0 to 127

//define interrupt num
#define IRQ_NUM_TIMER0       66  // Timer0
#define IRQ_NUM_TIMER1_1MS   67  // DMTIMER_1ms
#define IRQ_NUM_TIMER2       68  // DMTIMER2
#define IRQ_NUM_TIMER3       69  // DMTIMER3
#define IRQ_NUM_TIMER4       92  // DMTIMER4
#define IRQ_NUM_TIMER5       93  // DMTIMER5
#define IRQ_NUM_TIMER6       94  // DMTIMER6
#define IRQ_NUM_TIMER7       95  // DMTIMER7

#endif