
.align	2
.global WRITE_REG32
WRITE_REG32:
	str 	r1,[r0]
	bx 		lr

.align	2	
.global READ_REG32
READ_REG32:
	ldr 	r0,[r0]
	bx 		lr

.align	2
.global WR_REG16
WR_REG16:
	strh 	r1,[r0]
	bx 		lr

.align	2
.global RD_REG16
RD_REG16:
	ldrh 	r0,[r0]
	bx 		lr
	
.align	2	
.global WR_REG8
WR_REG8:
	strb 	r1,[r0]
	bx 		lr
	
.align	2
.global RD_REG8
RD_REG8:
	ldrb 	r0,[r0]
	bx 		lr
	

.align	2
.global READ_CPSR
READ_CPSR:
	mrs 	r0, cpsr
	bx 		lr

.align	2
.global READ_SP
READ_SP:
	//r13 is stack pointer
	mov 	r0, r13					
	bx 		lr

.align	2
.global READ_CP15_c1
READ_CP15_c1:
	MRC     p15, #0, r0, c1, c0, #0
	bx 		lr

.align	2
.global READ_VECTOR_BASE
READ_VECTOR_BASE:
	/**
	 * CP15 c12暫存器的 bit[31:5]存放 exception vector base address
	 * exception vectors offset
	 * 
	 * 0x00 Reset 				Reset
	 * 0x04 Undefined 			PC = [base + 20h]
	 * 0x08 SWI 				PC = [base + 24h]
	 * 0x0C Pre-fetch abort 	PC = [base + 28h]
	 * 0x10 Data abort 			PC = [base + 2ch]
	 * 0x14 Unused 				PC = [base + 30h]
	 * 0x18 IRQ 				PC = [base + 34h]
	 * 0x20 FIQ 				PC = [base + 38h]
	 */ 
	mrc 	p15, #0, r0, c12, c0, #0
	bx 		lr

/****************************************************************************************/ 
 // c 指  CPSR中的control field ( PSR[7:0])
 // f 指  flag field (PSR[31:24])
 // x 指  extend field (PSR[15:8])
 // s 指  status field ( PSR[23:16])
 // 				
/****************************************************************************************/
 //.equ exception_vector_base,	0x9ff52000
.equ kernel_stack_top ,0x9df3fffc
.equ STACK_SIZE, 4096
/****************************************************************************************/

.type _start, %function  
.align	2
.global _start
_start:
	/**
	 * U-Boot version :2017.01
	 * 初始化已透過U-Boot初始化
	 * 
	 * 透過 READ_VECTOR_BASE function 讀取 exception vector 存放的位址:
	 * 0x9ff52000為 exception vector's base addr
	 * SMI at 0x9ff52008 -> jump to 0x9ff52024
	 * So svc_entry's address should load to 0x9ff52024
	 */

	/**設定svc_entry 的entry
	 * 0x9ff52024 存放實際的svc_entry function的 entry address
	 * 所以下面三行意思是令 
	 * r0 = 0x9ff52024
	 * r1 = svc_entry的 address
	 * 再令 r0(0x9ff52024)這個位址的值 = svc_entry的 address(存在r1)
	 * 
	 * 所以當觸發 svc中斷時, 系統會先根據exception vector去 0x9ff52024這個位址拿svc_entry的
	 * entry addr 
	 * */ 

	mrs r0, cpsr
	bic r0, r0, #0x1F 	// clear mode bits
	orr r0, r0, #0x13 	// switch to SVC mode
	orr r0, r0, #0xC0 	// disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr cpsr, r0

/********************************************************************/
// 設定 exception vector的起始位址
// 以下這些位址應該要存放跳轉指令 ,以SVC為例 ,PC = [0x9ff52024] (跳轉到這個記憶體存放的內容)
// 0x9ff52000 Reset 				
// 0x9ff52004 Undefined 			
// 0x9ff52008 SWI 				
// 0x9ff5200C Pre-fetch abort 	
// 0x9ff52010 Data abort 			
// 0x9ff52014 Unused 				
// 0x9ff52018 IRQ 				
// 0x9ff52020 FIQ 				
/********************************************************************/
	// ldr 	r0, =exception_vector_base
	// mcr 	p15, #0, r0, c12, c0, #0		//Set the exception vector base
	mrc 	p15, #0, r0, c12, c0, #0		//Get the exception vector base from c12
/********************************************************************/

	ldr		r1, =0xe59ff014 // 0xe59ff014 = ldr	 pc, [pc, #20]
	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//Undefined ,exception_vector_base +0x04

	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//SWI ,exception_vector_base +0x08	

	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//Pre-fetch abort ,exception_vector_base +0x0c	 

	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//Data abort ,exception_vector_base +0x10	 	

	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//Unused ,exception_vector_base +0x14	

	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//IRQ ,exception_vector_base +0x18	

	add		r0 ,r0 ,#4
	str		r1 ,[r0]	//FIQ ,exception_vector_base +0x1c	

/****************************************************************************************/
// 設定 svc_entry ,irq_handler的 entry位址
/****************************************************************************************/
	// 存放 svc entry 位址的位址 =exception_vector_base +0x24
	//mrc 	p15, #0, r0, c12, c0, #0		//Get the exception vector base from c12
	add	r0 ,r0 ,#0x8						//r0 =exception_vector_base +0x24
	ldr r1, =svc_entry
	str r1, [r0]
	

	// 存放 irq entry 位址的位址 =exception_vector_base +0x34
	add	r0 ,r0 ,#0x10			//r0 =exception_vector_base +0x34
	ldr r1, =irq_entry
	str r1, [r0]

/****************************************************************************************/
// 設定 svc stack top	
/****************************************************************************************/
	ldr sp, =kernel_stack_top	
    add r1, sp, #STACK_SIZE		//Now r1 = kernel_stack_top +0x1000

    // save svc mode
    mrs r3, cpsr

/****************************************************************************************/
// 設定 irq stack top
/****************************************************************************************/
    mov r2, #0x12 		
    msr cpsr_cxsf, r2
    mov sp, r1					//irq's sp = kernel_stack_top +0x1000
    add r1, sp, #STACK_SIZE		//Now r1 = kernel_stack_top +0x2000

/****************************************************************************************/
// 回到 svc mode
/****************************************************************************************/
    msr cpsr_cxsf, r3

/****************************************************************************************/
// 跳到kernal_entry
/****************************************************************************************/
	bl 		kernal_entry
/****************************************************************************************/

/*
.loop: 
	b 		.loop
*/

/****************************************************************************************/



/****************************************************************************************/
/****************************************************************************************/
.type _call_sched, %function  
.align	2
.global _call_sched
_call_sched:
	mov		sp, r0
	ldmfd 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	bx 		lr

/****************************************************************************************/
/****************************************************************************************/