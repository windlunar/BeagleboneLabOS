
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



/************************************************************************************************
 * 當呼叫SVC時，SVC會觸發exception 
 * 跳進 svc_handler
************************************************************************************************/
.type svc_handler, %function    
.global svc_handler
.align	4
svc_handler:
	/** switch to system mode
	 * 要切換到system mode的原因為, SVC mode的r13(sp) ,r14(lr) 與user mode是不共用的
	 * 而 system mode是共用的 ,因此先切換到system mode以保存user state
	 */
	mov 	r9 ,lr	//保存返回user proc address
	
	/* Save user state*/
	mrs		ip, spsr
/************************************************************************************************/
	//switch to system mode to sace user proc env
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//儲存 user proc context
	// r0在syscall_print_hello時已經push到stack中 r0
	mov		lr ,r9
	stmdb  sp!, {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	
/************************************************************************************************/
	//switch back to svc mode
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SVC) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//call syscall handler ,r0 is the syscall id
	push	{r0 ,r1 ,r2 ,r3}
	bl 		syscall_handler
	pop		{r0 ,r1 ,r2 ,r3}
/************************************************************************************************/
	//switch to system mode
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10
	
	//pop to user context
	ldmia 	sp!, {r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	mrs		r10 ,spsr
/************************************************************************************************/
	//switch back to svc mode
	msr		cpsr, r10

	//返回 syscall_print_hello
	blx 	lr	
	//bx	r9

.globl syscall_print_hello; 
.align	4
syscall_print_hello: 
	//stmdb sp!, {r13}
	push {r0 ,lr}
	mov r0, #SYSCALL_ID_print_hello
	svc 0x00
	pop	{r0 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx lr	//返回 user proc


.globl syscall_print_hello; 
.align	4
syscall_yield: 
	//stmdb sp!, {r13}
	push {r0 ,lr}
	mov r0, #SYSCALL_ID_yield
	svc 0x00
	pop	{r0 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx lr	//返回 user proc



