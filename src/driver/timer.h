/**
 * Define registers of Timer
 */  

#ifndef __TIMER_H_
#define __TIMER_H_


#include "../common.h"

/*****************************************************************************************/

typedef struct{
	uint32_t TIDR ;           /* offset = 0x00 */
	uint32_t NON_USE1 ;       /* offset = 0x04 */
	uint32_t NON_USE2 ;       /* offset = 0x08 */
	uint32_t NON_USE3 ;       /* offset = 0x0c */
	uint32_t TIOCP_CFG ;      /* offset = 0x10 */
	uint32_t NON_USE4 ;       /* offset = 0x14 */
	uint32_t NON_USE5 ;       /* offset = 0x18 */
	uint32_t NON_USE6 ;       /* offset = 0x1c */
	uint32_t IRQ_EOI ;        /* offset = 0x20 */
	uint32_t IRQSTATUS_RAW ;  /* offset = 0x24 */
	uint32_t IRQSTATUS ;      /* offset = 0x28 */
	uint32_t IRQENABLE_SET ;  /* offset = 0x2c */
	uint32_t IRQENABLE_CLR ;  /* offset = 0x30 */
	uint32_t IRQWAKEEN ;      /* offset = 0x34 */
	uint32_t TCLR ;           /* offset = 0x38 */

	/* TIMER_COUNTER */
	uint32_t TCRR ;           /* offset = 0x3c */

	/** 
	 * LOAD_VALUE: Timer counter value loaded on overflow in auto-reload mode or on
	 * TTGR write access
	 */
	uint32_t TLDR ;           /* offset = 0x40 */
	uint32_t TTGR ;           /* offset = 0x44 */

	/**
	 * TWPS Register Field Descriptions:
	 * bit4 | W_PEND_TMAR | R/W | 0h | When equal to 1, a write is pending to the TMAR register
	 * bit3 | W_PEND_TTGR | R/W | 0h | When equal to 1, a write is pending to the TTGR register
	 * bit2 | W_PEND_TLDR | R/W | 0h | When equal to 1, a write is pending to the TLDR register
	 * bit1 | W_PEND_TCRR | R/W | 0h | When equal to 1, a write is pending to the TCRR register
	 * bit0 | W_PEND_TCLR | R/W | 0h | When equal to 1, a write is pending to the TCLR register
	 */ 
	uint32_t TWPS ;           /* offset = 0x48 */
	uint32_t TMAR ;           /* offset = 0x4c */
	uint32_t TCAR1 ;          /* offset = 0x50 */
	uint32_t TSICR ;          /* offset = 0x54 */
	uint32_t TCAR2 ;          /* offset = 0x58 */
}DMTIMER_T ;


#define DMTIMER0_BASE    (0x44E05000)
#define DMTIMER0_BASE_PTR_t           ((volatile DMTIMER_T *)DMTIMER0_BASE)

#define DMTIMER2_BASE    (0x48040000)
#define DMTIMER2_BASE_PTR_t           ((volatile DMTIMER_T *)DMTIMER2_BASE)

/*****************************************************************************************/

void timer_init(volatile DMTIMER_T *DMTIMER_struct_ptr ,uint32_t msecs);
void timerDisable(volatile DMTIMER_T *DMTIMER_struct_ptr);
void enableTimerAndBindISR(int32_t IRQ_ID ,void (*handler)(void));
void disnableTimerAndUnbindISR(int32_t IRQ_ID);

void __attribute__((optimize("O0"))) delay(uint32_t num);

void timer_start(volatile DMTIMER_T *DMTIMER_struct_ptr) ;
void reloadOsTick(uint32_t msecs) ;

void OsTickInit(volatile DMTIMER_T *DMTIMER_struct_ptr);
void enableOsTick(uint8_t irq_num) ;

/*****************************************************************************************/

#define WATCHDOG_BASE       (0x44E35000)
#define WDT_WCRR            (0x28)
#define WDT_WLDR            (0x2c)  /* The Watchdog Load Register */
#define WDT_WTGR            (0x30)
#define WDT_WWPS            (0x34)
#define WDT_WSPR            (0x48)

#define DISABLE_FIRST_WR    (0x0000AAAA)
#define DISABLE_SECOND_WR   (0x00005555)
#define ENABLE_FIRST_WR     (0x0000BBBB)
#define ENABLE_SECOND_WR    (0x00004444)

#define W_PEND_WCLR         (0x00000001)
#define W_PEND_WCRR         (0x00000002)
#define W_PEND_WLDR         (0x00000004)
#define W_PEND_WTGR         (0x00000008)
#define W_PEND_WSPR         (0x00000010)
#define W_PEND_WDLY         (0x00000020)

#define REG_RW(x)           (*((volatile uint32_t *)(x)))

void reload_watchdog(uint32_t base) ;
void disable_watchdog(unsigned int baseAdd) ;
void enable_watchdog(uint32_t base) ;
void set_wdt_count(uint32_t base, uint32_t countVal) ;

#endif