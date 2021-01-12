
#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "../common.h"
#include "interrupt_regs.h"


void SYSTEM_INT_Enable(int32 interruptID) ;
void SYSTEM_INT_disable(int32 interruptID) ;
uint32 getActivateIrqNum(void) ;

void 
setIntRouteAndPriority(uint32 interruptID ,uint32 priority ,uint32 route);

void 
setIntRoute(uint32 interruptID ,uint32 route);

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