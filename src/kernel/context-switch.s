

.global userTaskRun
userTaskRun:
	/* save kernel state */
	mrs 	ip, cpsr
	push 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}
	
	/* switch to user mode and enable irq */
	//msr 	CPSR_c, #0xD0 
	msr 	CPSR_c, #0x50

	/** Load user stack 
	 * r0為傳入的第一個參數, 也就是user stack pointer
	 */	
	mov		r13, r0				

	/* Load user state */
	ldmia 	sp!, {r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	msr		apsr, ip
	

	/* Jump to user task */
	blx lr



.type svc_handler, %function    
.global svc_handler

.align	2
svc_handler:
	/** switch to system mode
	 * 要切換到system mode的原因為, SVC mode的r13(sp) ,r14(lr) 與user mode是不共用的
	 * 而 system mode是共用的 ,因此先切換到system mode以保存user state
	 */
	msr 	CPSR_c, #0xDF

    /* Save user state*/
	mrs		ip, apsr

	stmdb sp!, {r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}

	// r0作為返回值 ,返回user mode的sp
	mov		r0, r13

	/* switch to superviser mode */
	msr 	CPSR_c, #0xD3

    /* Restore kernal state*/
	pop 	{r4, r5, r6, r7, r8, r9, r10 ,fp ,ip ,lr}
	msr		cpsr, ip

	blx 	lr

/************************************************************************************************/

.global irq_entry
.align	2
irq_entry:
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
 * CPSR MODE FIELD :
 * 
 * #define 	CPSR_M_USR   0x10U
 * CPSR: M User mode (PL0)
 * 
 * #define 	CPSR_M_FIQ   0x11U
 * CPSR: M Fast Interrupt mode (PL1)
 * 
 * #define 	CPSR_M_IRQ   0x12U
 * CPSR: M Interrupt mode (PL1)
 * 
 * #define 	CPSR_M_SVC   0x13U
 * CPSR: M Supervisor mode (PL1)
 * 
 * #define 	CPSR_M_MON   0x16U
 * CPSR: M Monitor mode (PL1) 
 * 
 * #define 	CPSR_M_ABT   0x17U
 * CPSR: M Abort mode (PL1) 
 * 
 * #define 	CPSR_M_HYP   0x1AU
 * CPSR: M Hypervisor mode (PL2) 
 * 
 * #define 	CPSR_M_UND   0x1BU
 * CPSR: M Undefined mode (PL1) 
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