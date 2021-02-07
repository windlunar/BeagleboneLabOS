
#include "../common.h"
#include "timer.h"
#include "../kernel/kprint.h"
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


void OsTickInit(volatile DMTIMER_T *DMTIMER_struct_ptr)
{
    // Component interrupt request enable. Write 1 to set (enable interrupt)
    // 2 TCAR_EN_FLAG   ,IRQ enable for Capture
    // 1 OVF_EN_FLAG    ,IRQ enable for Overflow
    // 0 MAT_EN_FLAG    ,IRQ enable for Match
    DMTIMER_struct_ptr->IRQENABLE_SET = (1 << 1);

    // 1 AR 0h = One shot timer ,1h = Auto-reload timer
    
    DMTIMER_struct_ptr->TCLR = (1 << 1); 

    // LOAD_VALUE ,Timer counter value loaded on overflow in auto-reload mode or on
    // TTGR write access ,32.768 kHz clk
    // When the auto-reload mode is enabled (TCLR AR bit = 1), 
    // the TCRR is reloaded with the Timer Load
    // Register (TLDR) value after a counting overflow
    //
    // 100 =msec(原本應該要放 ostick的時間)
    DMTIMER_struct_ptr->TLDR = ~0 - (32768 * 100 / 1000); 

    // load ,TIMER_COUNTER ,Value of TIMER counter
    // 改在 task_asm 呼叫 reloadOsTick(r0)來 load 值
    //DMTIMER_struct_ptr->TCRR = ~0 - (32768 * msecs / 1000); 

	delay(1000); 

    // 0 ST ,1h = Start timer
    DMTIMER_struct_ptr->TCLR |= (1 << 0); 
}


void reloadOsTick(uint32_t msecs)
{
    // load ,TIMER_COUNTER ,Value of TIMER counter
    DMTIMER0_BASE_PTR_t->TCRR = ~0 - (32768 * msecs / 1000); 
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

/********************************************************************************/

void reload_watchdog(uint32_t base)
{
    REG_RW(base + WDT_WLDR) = 0x01 ;
    while (REG_RW(base + WDT_WWPS) & W_PEND_WLDR);
}


void disable_watchdog(uint32_t base)
{
    REG_RW(base + WDT_WSPR) = DISABLE_FIRST_WR;
    while (REG_RW(base + WDT_WWPS) & W_PEND_WSPR) ;

    REG_RW(base + WDT_WSPR) = DISABLE_SECOND_WR ;
    while (REG_RW(base + WDT_WWPS) & W_PEND_WSPR) ;
}


void enable_watchdog(uint32_t base)
{
    REG_RW(base + WDT_WSPR) = ENABLE_FIRST_WR ;
    while (REG_RW(base + WDT_WWPS) & W_PEND_WSPR) ;

    REG_RW(base + WDT_WSPR) = ENABLE_SECOND_WR ;
    while (REG_RW(base + WDT_WWPS) & W_PEND_WSPR) ;
}



void set_wdt_count(uint32_t base, uint32_t countVal)
{
    REG_RW(base + WDT_WCRR) = countVal;
    while (REG_RW(base + WDT_WWPS) & W_PEND_WCRR) ;
}