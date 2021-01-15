
#include "syscall.h"

/** 
 * CPSR MODE FIELD :
 */ 
.equ 	CPSR_M_USR,   0x10U		// CPSR: M User mode (PL0)
.equ 	CPSR_M_FIQ,   0x11U		//CPSR: M Fast Interrupt mode (PL1)
.equ 	CPSR_M_IRQ,   0x12U		//CPSR: M Interrupt mode (PL1) 
.equ 	CPSR_M_SVC,   0x13U		//CPSR: M Supervisor mode (PL1)
.equ 	CPSR_M_MON,   0x16U		//CPSR: M Monitor mode (PL1) 
.equ 	CPSR_M_ABT,   0x17U		//CPSR: M Abort mode (PL1)  
.equ 	CPSR_M_HYP,   0x1AU		//CPSR: M Hypervisor mode (PL2) 
.equ 	CPSR_M_UND,   0x1BU		//CPSR: M Undefined mode (PL1) 
.equ 	CPSR_M_SYS,   0x1FU		//CPSR: M system mode 


//Define syscall id
.equ	 SYSCALL_ID_print_hello ,   1
.equ	 SYSCALL_ID_yield ,   		2



.global syscall_print_hello; 
.align	4
syscall_print_hello: 
	push {r0 ,lr}
	mov r0, #SYSCALL_ID_print_hello
	svc 0x00
	pop	{r0 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx lr	//返回 user proc


.global syscall_yield; 
.align	4
syscall_yield: 
	push {r0 ,lr}
	mov r0, #SYSCALL_ID_yield
	svc 0x00
	pop	{r0 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx lr	//返回 user proc



	



