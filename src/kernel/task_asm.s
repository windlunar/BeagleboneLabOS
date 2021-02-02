
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



/************************************************************************************************/
/************************************************************************************************/
.global TaskRun
.align	2
TaskRun:
	/* save kernel state */
	mrs 	ip, spsr
	push 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}

/************************************************************************************************/
	/* switch to system mode */
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F // clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 // disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	/** Load user stack 
	 * r0為傳入的第一個參數, 也就是user stack pointer
	 */	
	mov		r13, r0				

	// pop ,Load user state ,r9=跳轉addr
	//恢復狀態
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
/************************************************************************************************/
// 在這邊 reload ostimer counter ,tick =1ms (mov r0 #1)
// 而不要在初始化 timer就load的話
// 看起來可以解決卡在 bx r9的問題
/************************************************************************************************/
	stmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	
	//mov		r0 ,#1
	mov		r0 ,#(ostick_msec)

	bl 		reloadOsTick

	ldmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
/************************************************************************************************/


	/* Jump to user task */
	// r9 = user task返回位址
	msr		cpsr, ip
	msr     CPSR_c, #CPSR_M_USR	//如果中斷發生在這行 ,那跳轉回來還是在這行 ,會一直在bx r9卡住?
	
	bx 		r9

/************************************************************************************************/
/************************************************************************************************/