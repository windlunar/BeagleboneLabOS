
#include "../common.h"
#include "timer.h"
#include "../lib/print.h"
#include "uart.h"
#include "cm_per.h"
#include "../kernel/interrupt.h"
#include "../driver/usr_led.h"


void timer0_ISR(void)
{
	(DMTIMER0_BASE_PTR_t->IRQSTATUS) = (1 << 1);
	usrLedToggle(3);
	usrLedToggle(2);
	usrLedToggle(1);
	usrLedToggle(0);
}

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

    enableTimerAndBindISR(IRQ_NUM_TIMER0 ,timer0_ISR);
}


void timerDisable(volatile DMTIMER_T *DMTIMER_struct_ptr)
{
    DMTIMER_struct_ptr->TCLR = (0 << 1) | (0 << 0);
    DMTIMER_struct_ptr->TLDR = 0;
    DMTIMER_struct_ptr->IRQENABLE_CLR = (1 << 1);
}


void enableTimerAndBindISR(int32_t IRQ_ID ,void (*handler)(void))
{
	irq_isr_bind(IRQ_ID, handler);
}


void disnableTimerAndUnbindISR(int32_t IRQ_ID)
{
	irq_isr_unbind(IRQ_ID);
}


void __attribute__((optimize("O0"))) delay(uint32_t num)
{
	for (uint32_t count = 0 ;count < num ;count++) ;
}