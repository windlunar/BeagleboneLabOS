
.equ	ostick_msec	, 5


/** 
 * CPSR MODE FIELD :
 */ 
.equ 	CPSR_M_USR,   0x10U		/*  User mode (PL0) */
.equ 	CPSR_M_FIQ,   0x11U		/* Fast Interrupt mode (PL1) */
.equ 	CPSR_M_IRQ,   0x12U		/* Interrupt mode (PL1) */ 
.equ 	CPSR_M_SVC,   0x13U		/* Supervisor mode (PL1) */
.equ 	CPSR_M_MON,   0x16U		/* Monitor mode (PL1) */
.equ 	CPSR_M_ABT,   0x17U		/* Abort mode (PL1) */
.equ 	CPSR_M_HYP,   0x1AU		/* Hypervisor mode (PL2) */
.equ 	CPSR_M_UND,   0x1BU		/* Undefined mode (PL1) */
.equ 	CPSR_M_SYS,   0x1FU		/* system mode */



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
	 * Load user context ,assign r0 to stack pointer. 
	 * R0 is the input argument ,whick point to the start address of context structure
	 * 
	 */	
	mov		r13, r0				


	/* Pop user state */
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}


	/**
	 * Reload os timer counter
	 */
	stmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	
	mov		r0 ,#(ostick_msec)
	bl 		reload_ostick

	ldmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	

	/* Branch to user task */
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


	/* Load context */ 
	mov		r13, r0				


	/* pop ,Load user state */
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}

	
	/* Reload os timer counter */
	stmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	mov		r0 ,#(ostick_msec)
	bl 		reload_ostick
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


	/** Load context */	
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


