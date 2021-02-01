

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
.align	4
irq_entry:
/****************************************************************************************/
// 分支處理timer0 irq(as os tick)跟其他中斷
/****************************************************************************************/

	stmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}
	bl 		getActivateIrqNum	//回傳值在r0

	cmp 	r0, #66			//判斷是不是 os_tick中斷
	bne 	non_ostick_irq	//不是的話就跳去處理一般中斷

	ldmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}

/****************************************************************************************/	
// 準備原來user task的context 結構
// 然後存到 r0 作為 irq_handler的傳入參數
/****************************************************************************************/
	sub		lr, lr, #4	//調整 lr_irq(返回user proc的位址)

	mov		r9 ,lr		//r9 = lr_irq
	mrs		ip, spsr

/***************************************************************************/
 // 測試 ,目前還在 irq mode ,重設 irq的stack pointer為初始pointer
 //
	ldr sp, =0x9df41000
/***************************************************************************/

/****************************************************************************************/	
// switch to system mode
/****************************************************************************************/
	
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) 
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	// In system mode
	// r9 =lr_irq
	// push
	// 準備原來user task的context 結構
	// 然後存到 r0 作為 irq_handler的傳入參數
	stmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}

	//make r0 as the user stack pointer (user task的context struct的起始位址)
	mov		r0,	sp	

/****************************************************************************************/	
// switch to svc mode
/****************************************************************************************/

	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SVC) // switch to svc mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10
/****************************************************************************************/
	//In SVC mode
	//在svc mode中處理irq中斷 ,應該傳入 user task的context(sp) 結構 address = r0
	bl 		timer0_ISR
/****************************************************************************************/
non_ostick_irq:
	ldmfd 	sp!,	{r0 ,r1 ,r2 ,r3 ,r4 ,r5 ,r6 ,r7 ,r8 ,r9 ,r10 ,r11 ,r12 ,lr}
	b 		irqs_handler
/****************************************************************************************/




/****************************************************************************************/
 // 當呼叫SVC時，SVC會觸發svc exception 
 // 跳進 svc_handler
/****************************************************************************************/
.type svc_handler, %function    
.global svc_handler
.align	4
svc_handler:
	/** switch to system mode
	 * 要切換到system mode的原因為, SVC mode的r13(sp) ,r14(lr) 與user mode是不共用的
	 * 而 system mode是共用的 ,因此先切換到system mode以保存user state
	 */
	mov 	r9 ,lr	//保存返回user proc的 address
	
	/* Save user state*/
	mrs		ip, spsr
/****************************************************************************************/
	//switch to system mode to save user proc context
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//儲存 user proc context
	// r0在執行 svc 時已經push到stack中
	//現在 lr跟 r9都是 user proc的返回addr
	mov		lr ,r9	

	// push ,準備原來user task的context 結構
	// 然後存到 r1 作為傳入irq_handler的user task的context 結構 
	// r2為傳入參數
	stmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,r12 ,lr}
	mov		r1,	sp	

/****************************************************************************************/
	//switch back to svc mode
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SVC) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10

	//call syscall handler ,
	//r0 is the syscall id ,
	//r1 is the original context
	//r2 is input args(pointer)
	push	{r0 ,r1 ,r2 ,r3}
	bl 		syscall_handler

	pop		{r0 ,r1 ,r2 ,r3}

/****************************************************************************************/
	//switch to system mode
	mrs 	r10, cpsr
	bic 	r10, r10, #0x1F 		// clear bits
	orr 	r10, r10, #(CPSR_M_SYS) // system mode
	orr 	r10, r10, #0xC0 		// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr 	cpsr, r10
	
	//pop user context
	//恢復狀態
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	mrs		r10 ,spsr
/****************************************************************************************/
	//switch back to svc mode
	msr		cpsr, r10

	//返回 syscall_<NAME>
	blx 	lr	
/****************************************************************************************/



