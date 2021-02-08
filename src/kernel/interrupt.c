
#include "../common.h"
#include "interrupt_regs.h"
#include "interrupt.h"
#include "printk.h"
#include "task.h"
#include "../driver/timer.h"
#include "../driver/usr_led.h"


#define IRQ_NUM 128
#define IRQ_NUM_MASK 127


/** 128個函數陣列 */
static void (*irq_handler_array[IRQ_NUM])(void);



void SYSTEM_INT_Enable(int32_t interruptID)
{             
    int32_t interrupt_group = interruptID >> 5 ;
    *(INTC_BASE_PTR + INTC_MIR_CLEAR_BASE + 0x20 * interrupt_group) = 0x01 << (interruptID & 0x1F) ;

}



void SYSTEM_INT_disable(int32_t interruptID)
{             
    int32_t interrupt_group = interruptID >> 5 ;
    *(INTC_BASE_PTR + INTC_MIR_SET_BASE + 0x20 * interrupt_group) = 0x01 << (interruptID & 0x1F) ;

}



uint32_t getActivateIrqNum(void)
{
    return ( *(INTC_BASE_PTR + INTC_SIR_IRQ) &  IRQ_NUM_MASK);
}



void 
setIntRouteAndPriority(uint32_t interruptID ,uint32_t priority ,uint32_t route)
{
    *(INTC_ILR_n_BASE_PTR + interruptID * 0x04) = ( (priority << 0x02) & 0xfc ) | route ;
}



void 
setIntRoute(uint32_t interruptID ,uint32_t route)
{
    *(INTC_ILR_n_BASE_PTR + interruptID * 0x04) |= route ;
}



void cpsrEnableIRQ(void)
{
    asm volatile("mrs r0, CPSR\n\t"
        		 "bic r0, r0, #0x80\n\t"
        		 "msr CPSR_c, r0");
}



void cpsrDisableIRQ(void)
{
    asm volatile("mrs r0, CPSR\n\t"
        		 "orr r0, r0, #0x80\n\t"
        		 "msr CPSR_c, r0");
}



void cpsrDisableFIQ(void)
{
    /* Disable FIQ in CPSR */
    asm volatile("mrs r0, CPSR\n\t"
        		 "orr r0, r0, #0x40\n\t"
        		 "msr CPSR_c, r0");
}



void cpsrEnableFIQ(void)
{
    /* Enable FIQ in CPSR */
    asm volatile("mrs r0, CPSR\n\t"
        "bic r0, r0, #0x40\n\t"
        "msr CPSR_c, r0");
}



void setNewIrqAgr(){
    *(INTC_BASE_PTR + INTC_CONTROL) |= NEW_IRQ_AGREE ;
}



uint32_t getIntVectorAddr(void)
{
	uint32_t intvector_addr;

	asm volatile("mrc p15, 0, %0, c1, c0, 0\n" : "=r" (intvector_addr) : : );
	if (intvector_addr & (1 << 13)) {
		return 0xFFFF0000;
	}
		
	asm volatile("mrc p15, 0, %0, c12, c0, 0\n" : "=r" (intvector_addr) : : );
	return intvector_addr;
}



static void irq_handler_array_init(void)
{
	int i;

	for (i = 0; i < IRQ_NUM; i++) {
		irq_handler_array[i] = NULL;
	}
}



void interrupt_init(void)
{
	irq_handler_array_init();
	dataSyncBarrier(); 
}



void disableIrqThroughCpsr(void)
{
	dataSyncBarrier(); 
	cpsrDisableIRQ() ;
}



void eableINT_NUM(uint8_t irq_num)
{
	switch (irq_num >> 5) {
		case 0:
			*(INTC_BASE_PTR + INTC_MIR_CLEAR0) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
		case 1:
			*(INTC_BASE_PTR + INTC_MIR_CLEAR1) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
		case 2:
			*(INTC_BASE_PTR + INTC_MIR_CLEAR2) = 1 << (irq_num & ((1 << 5) - 1));
			break;
		case 3:
			*(INTC_BASE_PTR + INTC_MIR_CLEAR3) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
	}
}



void disableINT_NUM(uint8_t irq_num)
{
	switch (irq_num >> 5) {
		case 0:
			*(INTC_BASE_PTR + INTC_MIR_SET0) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
		case 1:
			*(INTC_BASE_PTR + INTC_MIR_SET1) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
		case 2:
			*(INTC_BASE_PTR + INTC_MIR_SET2) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
		case 3:
			*(INTC_BASE_PTR + INTC_MIR_SET3) = 1 << (irq_num & ((1 << 5) - 1)); 
			break;
	}
}



void irq_isr_bind(uint8_t irq_num, void (*handler)(void))
{
	irq_handler_array[irq_num] = handler;
    *(INTC_ILR_n_BASE_PTR + irq_num) = (0 << 2) | (0 << 0) ;
	eableINT_NUM(irq_num);
}



void irq_isr_unbind(uint8_t irq_num)
{
	disableINT_NUM(irq_num);
	*(INTC_ILR_n_BASE_PTR + irq_num) = 0 ;
	irq_handler_array[irq_num] = NULL;
}

/****************************************************************************************/

void __attribute__((interrupt("IRQ"))) irqs_handler(void)	
{
	/** 獲得 irq number以判斷是觸發那種中斷 */
	uint8_t irq_num = getActivateIrqNum();

	/** 根據獲得的irq num, 執行陣列中對應的函式 */
	if (irq_handler_array[irq_num]) {
		(*irq_handler_array[irq_num])();
		
	} else {
		return ;
	}

	dataSyncBarrier();
	*(INTC_BASE_PTR + INTC_CONTROL) = (NEW_IRQ_AGREE << 0); 
}

/****************************************************************************************/

void timer0_ISR(uint32_t *usrTaskContextOld)
{
	(DMTIMER0_BASE_PTR_t->IRQSTATUS) = (1 << 1);
	usrLedToggle(3);

	/** Save old context */
	curr_running_task->task_context = (struct TASK_CONTEXT *)usrTaskContextOld ;

	/** Change the task status to ready */
	curr_running_task->task_status = TASK_READY ;
	curr_running_task = NULL ;

	/** prepare sched_first_run() context */
	set_sched_context() ;

	/** 讓下一個irq能觸發 */
	*(INTC_BASE_PTR + INTC_CONTROL) = (NEW_IRQ_AGREE << 0); 

	call_sched((uint32_t)schedFuncContextSPtr) ;
}



/* 2021/1/15--Not work */
void timer2_ISR(void)
{	
	usrLedToggle(2);
	usrLedToggle(1);
	usrLedToggle(0);
	(DMTIMER2_BASE_PTR_t->IRQSTATUS) = (1 << 1);
}

/****************************************************************************************/

#define EXCEPT_NUMS		7 	/* Not include reset */

void set_exception_entry(uint32_t *exept_vec_base)
{
	uint32_t *base = exept_vec_base ;
	base += 1 ;
	for (int i=0 ; i<EXCEPT_NUMS; i++) {
		*base = 0xe59ff014 ;
		base += 1 ;
	}
}

/****************************************************************************************/

void __attribute__((interrupt("PABT"))) prefetch_abort_handler(void)
{
	printk("In prefetch_abort_handler\r\n");
	for(int i=0 ;i<2560 ;i++)
    {
        printk("addr =%p ,context =%x\r\n" ,(curr_running_task->pgtbase)+i ,*((curr_running_task->pgtbase)+i)) ;
    }
    printk("\r\ntask_info addr =%p" ,curr_running_task) ;
    printk(" ,base addr =%p" ,curr_running_task->pgtbase) ;
    printk(" ,task_context addr =%p\r\n" ,curr_running_task->task_context) ;
	set_wdt_count(WATCHDOG_BASE, 0xfffffff0) ;
    enable_watchdog(WATCHDOG_BASE) ;
	for(;;) ;
}



void __attribute__((interrupt("DABT"))) data_abort_handler(void)
{
	printk("In data_abort_handler\r\n");
	for(int i=0 ;i<2560 ;i++)
    {
        printk("addr =%p ,context =%x\r\n" ,(curr_running_task->pgtbase)+i ,*((curr_running_task->pgtbase)+i)) ;
    }
    printk("\r\ntask_info addr =%p" ,curr_running_task) ;
    printk(" ,base addr =%p" ,curr_running_task->pgtbase) ;
    printk(" ,task_context addr =%p\r\n" ,curr_running_task->task_context) ;
	set_wdt_count(WATCHDOG_BASE, 0xfffffff0) ;
    enable_watchdog(WATCHDOG_BASE) ;
	for(;;) ;
}