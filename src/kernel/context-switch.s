
/** 
 * CPSR MODE FIELD :
 */ 
// CPSR: M User mode (PL0)
.equ 	CPSR_M_USR,   0x10U

//CPSR: M Fast Interrupt mode (PL1)
.equ 	CPSR_M_FIQ,   0x11U

//CPSR: M Interrupt mode (PL1) 
.equ 	CPSR_M_IRQ,   0x12U
 
//CPSR: M Supervisor mode (PL1)
.equ 	CPSR_M_SVC,   0x13U
 
//CPSR: M Monitor mode (PL1) 
.equ 	CPSR_M_MON,   0x16U
 
//CPSR: M Abort mode (PL1)  
.equ 	CPSR_M_ABT,   0x17U

//CPSR: M Hypervisor mode (PL2)   
.equ 	CPSR_M_HYP,   0x1AU
 
//CPSR: M Undefined mode (PL1) 
.equ 	CPSR_M_UND,   0x1BU
 
//CPSR: M system mode 
.equ 	CPSR_M_SYS,   0x1FU



.global userTaskRun
userTaskRun:
	/* save kernel state */
	mrs 	ip, cpsr
	push 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}
	
	/* switch to user mode and enable irq */
	//msr 	CPSR_c, #0xD0 
	//msr 	CPSR_c, #0x50
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F // clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 // disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10


	/** Load user stack 
	 * r0為傳入的第一個參數, 也就是user stack pointer
	 */	
	mov		r13, r0				

	/* Load user state */
	ldmia 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	msr		apsr, ip
	
	/* Jump to user task */
	// r9 = user task返回位址
	msr 	CPSR_c, #0x50

	mov		pc,r9



.type svc_handler, %function    
.global svc_handler

.align	2
svc_handler:
	/** switch to system mode
	 * 要切換到system mode的原因為, SVC mode的r13(sp) ,r14(lr) 與user mode是不共用的
	 * 而 system mode是共用的 ,因此先切換到system mode以保存user state
	 */
	mov 	r9 ,lr
	//msr 	CPSR_c, #0xDF

	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

    /* Save user state*/
	mrs		ip, apsr

	stmdb sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}

	// r0作為返回值 ,返回user mode的sp
	mov		r0, r13

	/* switch to superviser mode */
	//msr 	CPSR_c, #0xD3
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F // clear mode bits
	orr 	r10, r10, #(CPSR_M_SVC) // switch to svc mode
	orr 	r10, r10, #0xC0 // disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

    /* Restore kernal state*/
	pop 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}
	msr		cpsr, ip

	blx 	lr

/************************************************************************************************/
/*
.global irq_entry
.align	2
irq_entry:
    b irqs_handler
*/

.global irq_entry
.align	2
irq_entry:
	sub		lr, lr, #4	//保存 lr_irq(返回user proc的位址)
	mov		r9 ,lr		//r9 = lr_irq

	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // switch to system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//In system mode
	//save previous user state
	mrs		ip, apsr

	//r9 =lr_irq
	stmdb 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}

	//return value r0 as the user stack pointer
	mov		r0,	sp	

	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SVC) // switch to svc mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//In SVC mode
	//在svc mode中處理irq中斷
	push 	{r0 ,r1 ,r2 ,r3}
	bl 		irqs_handler
	pop 	{r0 ,r1 ,r2 ,r3}

	pop 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}
	msr		cpsr, ip

	//回到sched
	bx 		lr



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



/** 
 *stmdb
 *ex：
 *
 *指令：stmdb sp!,{r0-r12,lr}
 *含意：sp = sp - 4，先push lr，sp = lr（將lr中的内容放入sp所指的記憶體位址）。sp = sp - 4，
 *再push r12，sp = r12。sp = sp - 4，再push r11，sp = r11......sp = sp - 4，
 *最後 pushr0，sp = r0。
 *
 *如果想要將 r0-r12和 lr pop出，可以用 ldmia指令：
 *指令：ldmia sp!,{r0-r12,lr}
 */