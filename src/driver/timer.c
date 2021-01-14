
#include "../common.h"
#include "timer.h"
#include "../lib/print.h"
#include "uart.h"
#include "cm_per.h"
#include "../kernel/interrupt.h"
#include "../driver/usr_led.h"
#include "../kernel/task.h"



void timer_init(volatile DMTIMER_T *DMTIMER_struct_ptr ,uint32_t msecs)
{
	CM_PER_BASE_PTR->CM_PER_TIMER2_CLKCTRL |= (1 << 1) ;

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


void OsTickInit(volatile DMTIMER_T *DMTIMER_struct_ptr ,uint32_t msecs)
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



void timer_start(volatile DMTIMER_T *DMTIMER_struct_ptr){
    // Start
    DMTIMER_struct_ptr->TCLR |= (1 << 0); 
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


void enableOsTick(uint8_t irq_num)
{
    *(INTC_ILR_n_BASE_PTR + irq_num) = (0 << 2) | (0 << 0) ;
	eableINT_NUM(irq_num);
}



void disnableTimerAndUnbindISR(int32_t IRQ_ID)
{
	irq_isr_unbind(IRQ_ID);
}


void __attribute__((optimize("O0"))) delay(uint32_t num)
{
	for (uint32_t count = 0 ;count < num ;count++) ;
}