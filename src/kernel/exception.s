

/**
 * 
 * CPSR : Current Program Status Register.
 * Bits	Name	Function
 * [31]	N	Negative condition code flag
 * [30]	Z	Zero condition code flag
 * [29]	C	Carry condition code flag
 * [28]	V	Overflow condition code flag
 * [27]	Q	Cumulative saturation bit
 * [26:25]	IT[1:0]	If-Then execution state bits for the Thumb IT (If-Then) instruction
 * [24]	J	Jazelle bit
 * [19:16]	GE	Greater than or Equal flags
 * [15:10]	IT[7:2]	If-Then execution state bits for the Thumb IT (If-Then) instruction
 * [9]	E	Endianness execution state bit: 0 - Little-endian, 1 - Big-endian
 * [8]	A	Asynchronous abort mask bit
 * [7]	I	IRQ mask bit
 * [6]	F	FIRQ mask bit
 * [5]	T	Thumb execution state bit
 * [4:0]	M	Mode field
 */ 

/****************************************************************************************/
 // CPSR MODE FIELD :
/****************************************************************************************/
.equ 	CPSR_M_USR,   0x10U		// CPSR: M User mode (PL0)
.equ 	CPSR_M_FIQ,   0x11U		//CPSR: M Fast Interrupt mode (PL1)
.equ 	CPSR_M_IRQ,   0x12U		//CPSR: M Interrupt mode (PL1) 
.equ 	CPSR_M_SVC,   0x13U		//CPSR: M Supervisor mode (PL1)
.equ 	CPSR_M_MON,   0x16U		//CPSR: M Monitor mode (PL1) 
.equ 	CPSR_M_ABT,   0x17U		//CPSR: M Abort mode (PL1)  
.equ 	CPSR_M_HYP,   0x1AU		//CPSR: M Hypervisor mode (PL2) 
.equ 	CPSR_M_UND,   0x1BU		//CPSR: M Undefined mode (PL1) 
.equ 	CPSR_M_SYS,   0x1FU		//CPSR: M system mode 


/****************************************************************************************/
// IRQ 中斷處理
/****************************************************************************************/
.global irq_entry
.align	2
irq_entry:
	/**
	 * Handle timer0 irq(os tick) and the others irqs seperately.
	 */
	stmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}


	/* Return val stores at r0 */
	bl 		get_act_irqnum


	/* Check if it is os_tick interrupt or not. */
	cmp 	r0, #66


	/* If not, branch to non_ostick_irq */
	bne 	non_ostick_irq


	ldmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}


	/* Adjust the return address lr_irq(Return from privilege mode to user task) */
	sub		lr, lr, #4			


	/* Save lr_irq to r9 */
	mov		r9 ,lr

	/* Save cpsr of user task to R12 */				
	mrs		r12, spsr

	
	/* Reset irq's sp ,cause we will handle os tick interrupt at svc mode ,
	 * and back to user mode directly
	 */
	ldr sp, =0x9df41000


	/**	
	 * switch to system mode
	 * Disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	 */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		/* clear bits */
	orr 	r10, r10, #(CPSR_M_SYS) 
	orr 	r10, r10, #0xC0
	msr 	cpsr, r10


	/**
	 * In system mode ,user mode shared the same stack pointer with system mode
	 * r9 =lr_irq
	 *
	 * Prepare context structure of user task.(Push it to stack)
	 * And then asign the stack pointer(context struct's start addr) to r0 as input arg.
	 */
	stmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	mov		r0,	sp	


	/**	
	 * switch to svc mode
	 */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SVC) // switch to svc mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10


	/* 
	 * Handle ostick irq at svc mode ,the input argument(r0) is the should be the start 
	 * address of user context
	 */
	bl 		timer0_isr


non_ostick_irq:
	ldmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}
	b 		irqs_handler





/****************************************************************************************/
 // SVC exception entry 
 // Will jump to here by using svc instruction.
/****************************************************************************************/
.type svc_entry, %function    
.global svc_entry
.align	2
svc_entry:

	/* Move lr to r9 to save the return address of user task */
	mov 	r9 ,lr	
	

	/* Save user state*/
	mrs		ip, spsr


	/** 
	 * switch to system mode to sace the context.
	 * In system mode ,user mode shared the same stack pointer with system mode
	 */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		/* clear bits */
	orr 	r10, r10, #(CPSR_M_SYS) 
	orr 	r10, r10, #0xC0 		/* disable FIQ and IRQ ,FIQ */
	msr 	cpsr, r10


	/** 
	 * Prepare context structure of user task.(Push it to stack)
	 * And then asign the spr(context struct's start addr) to r1 as 2nd input arg.
	 * 
	 * r0 : syscall id
	 * r1 : User context(start address ad context structure)
	 * r3 : The input args of system call.
	 */
	stmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	mov		r1,	sp	


	/* switch back to svc mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F
	orr 	r10, r10, #(CPSR_M_SVC)
	orr 	r10, r10, #0xC0
	msr 	cpsr, r10


	/**
	 * Call syscall_handler
	 */
	push	{r0 ,r1 ,r2 ,r3}
	bl 		syscall_handler
	pop		{r0 ,r1 ,r2 ,r3}


	/* switch to system mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F
	orr 	r10, r10, #(CPSR_M_SYS)
	orr 	r10, r10, #0xC0
	msr 	cpsr, r10
	

	/* Pop user context back */
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	mrs		r10 ,spsr


	/* switch back to user mode */
	msr		cpsr, r10


	/* Return to syscall_<NAME> */
	blx 	lr	




/****************************************************************************************/
//
/****************************************************************************************/
.type prefetch_abort_entry, %function    
.global prefetch_abort_entry
.align	2
prefetch_abort_entry:
	mov	r0	,lr
	b 	prefetch_abort_handler



/****************************************************************************************/
//
/****************************************************************************************/
.type data_abort, %function    
.global data_abort
.align	2
data_abort:
	mov	r0	,lr
	b 	data_abort_handler
