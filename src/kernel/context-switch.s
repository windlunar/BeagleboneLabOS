
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



.global userTaskRun
.align	4
userTaskRun:
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

	/* Jump to user task */
	// r9 = user task返回位址
	msr		cpsr, ip
	msr     CPSR_c, #CPSR_M_USR
	bx 		r9


/*
.global irq_entry
.align	2
irq_entry:
    b irqs_handler
*/

.global irq_entry
.align	4
irq_entry:
/***********************************************************************************************/
// 分支處理timer0 irq(as os tick)跟其他中斷
/***********************************************************************************************/

	stmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}
	bl 		getActivateIrqNum	//回傳值在r0

	cmp 	r0, #66	//判斷是不是 os_tick中斷
	bne 	non_ostick_irq	//不是的話就跳去處理一般中斷

	ldmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}

/***********************************************************************************************/	
// 準備原來user task的context 結構
// 然後存到 r0 作為 irq_handler的傳入參數
/***********************************************************************************************/
	sub		lr, lr, #4	//調整 lr_irq(返回user proc的位址)

	mov		r9 ,lr		//r9 = lr_irq
	mrs		ip, spsr

	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // switch to system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//In system mode
	//r9 =lr_irq
	//push
	// 準備原來user task的context 結構
	// 然後存到 r0 作為 irq_handler的傳入參數
	stmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}

	//make r0 as the user stack pointer (user task的context struct的起始位址)
	mov		r0,	sp	

	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SVC) // switch to svc mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10
/***********************************************************************************************/
	//In SVC mode
	//在svc mode中處理irq中斷 ,應該傳入 user task的context(sp) 結構 address = r0
	bl 		timer0_ISR
/***********************************************************************************************/
non_ostick_irq:
	ldmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}
	b irqs_handler



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