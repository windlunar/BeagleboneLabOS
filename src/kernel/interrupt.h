
#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "../common.h"
#include "interrupt_regs.h"

//void __attribute__((interrupt("IRQ"))) irqs_handler(void) ;
void irqs_handler(void);

/************************************************************************************************/
void SYSTEM_INT_Enable(int32_t interruptID) ;
void SYSTEM_INT_disable(int32_t interruptID) ;
uint32_t getActivateIrqNum(void) ;

void 
setIntRouteAndPriority(uint32_t interruptID ,uint32_t priority ,uint32_t route);

void 
setIntRoute(uint32_t interruptID ,uint32_t route);

void cpsrEnableIRQ(void);
void cpsrDisableIRQ(void);
void cpsrDisableFIQ(void);
void cpsrEnableFIQ(void);
void setNewIrqAgr();


/************************************************************************************************/

uint32_t getIntVectorAddr(void);
void interrupt_init(void);
void disableIrqThroughCpsr(void);
void eableINT_NUM(uint8_t irq_num);
void disableINT_NUM(uint8_t irq_num);
void irq_isr_bind(uint8_t irq_num, void (*handler)(void));
void irq_isr_unbind(uint8_t irq_num);

#endif