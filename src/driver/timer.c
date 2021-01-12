
#include "../common.h"
#include "timer.h"
#include "../lib/print.h"
#include "uart.h"
#include "cm_per.h"
#include "../kernel/interrupt.h"


/************************************************************************************************/
void timer_init(volatile DMTIMER_T *DMTIMER_struct_ptr ,uint32_t msecs)
{
    DMTIMER_struct_ptr->IRQENABLE_SET = (1 << 1);
    DMTIMER_struct_ptr->TCLR = (1 << 1); 

    // 32.768 kHz clk
    DMTIMER_struct_ptr->TLDR = ~0 - (32768 * msecs / 1000); 

    // load
    DMTIMER_struct_ptr->TCRR = ~0 - (32768 * msecs / 1000); 

	delay(1000); 

    // Start
    DMTIMER_struct_ptr->TCLR |= (1 << 0); 
}


void timerDisable(volatile DMTIMER_T *DMTIMER_struct_ptr)
{
    DMTIMER_struct_ptr->TCLR = (0 << 1) | (0 << 0);
    DMTIMER_struct_ptr->TLDR = 0;
    DMTIMER_struct_ptr->IRQENABLE_CLR = (1 << 1);
}


void timer_ISR_bind(int32_t IRQ_ID ,void (*handler)(void))
{
	irq_isr_bind(IRQ_ID, handler);
}


void timer_ISR_unbind(int32_t IRQ_ID)
{
	irq_isr_unbind(IRQ_ID);
}