
.equ	ostick_msec	, 5


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



/***************************************************************************************/
.global first_run
.align	2
first_run:

	/* switch to system mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F // clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 // disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	/** 
	 * Load user stack 
	 * r0為傳入的第一個參數, 也就是user stack pointer
	 */	
	mov		r13, r0				

	/* Pop user state */
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}

	/**
	 * 在這邊 reload ostimer counter ,tick =1ms (mov r0 #1)
	 * 而不要在初始化 timer就load的話
	 * 看起來可以解決卡在 bx r9的問題
	 */
	stmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	
	mov		r0 ,#(ostick_msec)
	bl 		reloadOsTick

	ldmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	

	/* Jump to user task */
	msr     CPSR_c, #CPSR_M_USR
	
	bx 		r9






/***************************************************************************************/
.global switch_task
.align	2
switch_task:
	/* save kernel state */
	mrs 	ip, spsr
	push 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}


	/* switch to system mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F
	orr 	r10, r10, #(CPSR_M_SYS)
	orr 	r10, r10, #0xC0 			/* disable FIQ and IRQ */
	msr 	cpsr, r10

	/* Load user stack */ 
	mov		r13, r0				

	/* pop ,Load user state */
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}

	
	/* 在這邊 reload ostimer counter */
	stmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	mov		r0 ,#(ostick_msec)
	bl 		reloadOsTick
	ldmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	

	/* Jump to user task */
	msr		cpsr, r12
	bx 		r9




/*****************************************************************************************/
.type set_context_sp, %function
.global set_context_sp
.align	2
set_context_sp:
	stmfd 	sp! ,{r1, r2, r3 ,r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}

	/* switch to system mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 			/* clear bits */
	orr 	r10, r10, #(CPSR_M_SYS) 	
	orr 	r10, r10, #0xC0 
	msr 	cpsr, r10

	/** Load user stack */	
	mov		r13, r0	


	/* switch back to svc mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F
	orr 	r10, r10, #(CPSR_M_SVC)
	orr 	r10, r10, #0xC0
	msr 	cpsr, r10

	ldmfd 	sp! ,{r1, r2, r3 ,r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	mov  pc, lr



/****************************************************************************************/
.type call_sched, %function  
.align	2
.global call_sched
call_sched:
	mov		sp, r0
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	bx 		lr


