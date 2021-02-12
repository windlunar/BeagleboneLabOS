
#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "../common.h"
#include "interrupt_regs.h"


/****************************************************************************************/

void int_unmask(int32_t irq_id) ;
void int_mask(int32_t irq_id) ;
uint32_t get_act_irqnum(void) ;

void set_irq_route_prio(uint32_t irq_id ,uint32_t priority ,uint32_t route);
void set_irq_route(uint32_t irq_id ,uint32_t route);

void cpsr_enable_irq(void);
void cpsr_disable_irq(void);
void cpsr_disable_fiq(void);
void cpsr_enable_fiq(void);
void set_new_irq_agr();

/****************************************************************************************/

uint32_t get_ivt_base(void);
void irq_init(void);
void disable_sys_irq(void);
void enable_irq_id(uint8_t irq_id);
void disable_irq_id(uint8_t irq_id);
void irq_handler_array_init(void) ;
void irq_isr_bind(uint8_t irq_id, void (*handler)(void));
void irq_isr_unbind(uint8_t irq_id);

/****************************************************************************************/

void set_exception_entry(uint32_t *exept_vec_base);

/****************************************************************************************/

void __attribute__((interrupt("ABORT"))) prefetch_abort_handler(uint32_t r0) ;
void __attribute__((interrupt("ABORT"))) data_abort_handler(uint32_t r0) ;
void __attribute__((interrupt("IRQ"))) irqs_handler(void) ;
void timer0_isr(uint32_t *usrTaskContextOld) ;
void timer2_isr(void) ;

/****************************************************************************************/


#endif