
#include "../common.h"
#include "timer.h"
#include "../lib/print.h"
#include "uart.h"

/**
 * dmtimer0's reg :TIDR :0x4fff1301                                                
 * dmtimer0's reg :TIOCP_CFG :0x0                                                  
 * dmtimer0's reg :IRQ_EOI :0x0                                                    
 * dmtimer0's reg :IRQSTATUS_RAW :0x0                                              
 * dmtimer0's reg :IRQSTATUS :0x0                                                  
 * dmtimer0's reg :IRQENABLE_SET :0x0                                              
 * dmtimer0's reg :IRQENABLE_CLR :0x0                                              
 * dmtimer0's reg :IRQWAKEEN :0x0                                                  
 * dmtimer0's reg :TCLR :0x0                                                       
 * dmtimer0's reg :TCRR :0x0                                                       
 * dmtimer0's reg :TLDR :0x0                                                       
 * dmtimer0's reg :TTGR :0xffffffff                                                
 * dmtimer0's reg :TWPS :0x0                                                       
 * dmtimer0's reg :TMAR :0x0                                                       
 * dmtimer0's reg :TCAR1 :0x0                                                      
 * dmtimer0's reg :TSICR :0x4                                                      
 * dmtimer0's reg :TCAR2 :0x0   
 */  
void dmtimer0PrintRegs(){
    kprintf("dmtimer0's reg :TIDR :%x\r\n",DMTIMER0_BASE_PTR_t->TIDR) ;
    kprintf("dmtimer0's reg :TIOCP_CFG :%x\r\n",DMTIMER0_BASE_PTR_t->TIOCP_CFG) ;
    kprintf("dmtimer0's reg :IRQ_EOI :%x\r\n",DMTIMER0_BASE_PTR_t->IRQ_EOI) ;
    kprintf("dmtimer0's reg :IRQSTATUS_RAW :%x\r\n",DMTIMER0_BASE_PTR_t->IRQSTATUS_RAW) ;
    kprintf("dmtimer0's reg :IRQSTATUS :%x\r\n",DMTIMER0_BASE_PTR_t->IRQSTATUS) ;
    kprintf("dmtimer0's reg :IRQENABLE_SET :%x\r\n",DMTIMER0_BASE_PTR_t->IRQENABLE_SET) ;
    kprintf("dmtimer0's reg :IRQENABLE_CLR :%x\r\n",DMTIMER0_BASE_PTR_t->IRQENABLE_CLR) ;
    kprintf("dmtimer0's reg :IRQWAKEEN :%x\r\n",DMTIMER0_BASE_PTR_t->IRQWAKEEN) ;
    kprintf("dmtimer0's reg :TCLR :%x\r\n",DMTIMER0_BASE_PTR_t->TCLR) ;
    kprintf("dmtimer0's reg :TCRR :%x\r\n",DMTIMER0_BASE_PTR_t->TCRR) ;
    kprintf("dmtimer0's reg :TLDR :%x\r\n",DMTIMER0_BASE_PTR_t->TLDR) ;
    kprintf("dmtimer0's reg :TTGR :%x\r\n",DMTIMER0_BASE_PTR_t->TTGR) ;
    kprintf("dmtimer0's reg :TWPS :%x\r\n",DMTIMER0_BASE_PTR_t->TWPS) ;
    kprintf("dmtimer0's reg :TMAR :%x\r\n",DMTIMER0_BASE_PTR_t->TMAR) ;
    kprintf("dmtimer0's reg :TCAR1 :%x\r\n",DMTIMER0_BASE_PTR_t->TCAR1) ;
    kprintf("dmtimer0's reg :TSICR :%x\r\n",DMTIMER0_BASE_PTR_t->TSICR) ;
    kprintf("dmtimer0's reg :TCAR2 :%x\r\n",DMTIMER0_BASE_PTR_t->TCAR2) ;
}