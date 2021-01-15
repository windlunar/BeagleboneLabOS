
.align	4
.globl WRITE_REG32
WRITE_REG32:
	str 	r1,[r0]
	bx 		lr

.align	4	
.globl READ_REG32
READ_REG32:
	ldr 	r0,[r0]
	bx 		lr

.align	4
.globl WR_REG16
WR_REG16:
	strh 	r1,[r0]
	bx 		lr

.align	4
.globl RD_REG16
RD_REG16:
	ldrh 	r0,[r0]
	bx 		lr
	
.align	4	
.globl WR_REG8
WR_REG8:
	strb 	r1,[r0]
	bx 		lr
	
	.align	4
.globl RD_REG8
RD_REG8:
	ldrb 	r0,[r0]
	bx 		lr
	

.align	4
.globl READ_CPSR
READ_CPSR:
	mrs 	r0, cpsr
	bx 		lr

.align	4
.globl READ_SP
READ_SP:
	//r13 is stack pointer
	mov 	r0, r13					
	bx 		lr

.align	4
.globl READ_CP15_c1
READ_CP15_c1:
	MRC     p15, #0, r0, c1, c0, #0
	bx 		lr

.align	4
.globl READ_VECTOR_BASE
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

/***********************************************************************************************/ 
 // c 指  CPSR中的control field ( PSR[7:0])
 // f 指  flag field (PSR[31:24])
 // x 指  extend field (PSR[15:8])
 // s 指  status field ( PSR[23:16])
 // 				
/***********************************************************************************************/
/***********************************************************************************************/
.equ STACK_SIZE, 4096
.align	4
.global _start
_start:
	/**
	 * U-Boot version :2017.01
	 * 初始化已透過U-Boot初始化
	 * 
	 * 透過 READ_VECTOR_BASE function 讀取 exception vector 存放的位址:
	 * 0x9ff52000為 exception vector's base addr
	 * SMI at 0x9ff52008 -> jump to 0x9ff52024
	 * So svc_handler's address should load to 0x9ff52024
	 */

	/**設定svc_handler 的entry
	 * 0x9ff52024 存放實際的svc_handler function的 entry address
	 * 所以下面三行意思是令 
	 * r0 = 0x9ff52024
	 * r1 = svc_handler的 address
	 * 再令 r0(0x9ff52024)這個位址的值 = svc_handler的 address(存在r1)
	 * 
	 * 所以當觸發 svc中斷時, 系統會先根據exception vector去 0x9ff52024這個位址拿svc_handler的entry addr 
	 * */ 

	mrs r0, cpsr
	bic r0, r0, #0x1F // clear mode bits
	orr r0, r0, #0x13 // switch to SVC mode
	orr r0, r0, #0xC0 // disable FIQ and IRQ ,FIQ is not supported in AM335x devices.
	msr cpsr, r0

	ldr r0, =0x9ff52024

	ldr r1, =svc_handler
	str r1, [r0]
	

	//設定 irq entry 位址
	ldr r0, =0x9ff52034
	ldr r1, =irq_entry
	str r1, [r0]

	//設定 stack top
    //ldr sp, =0x9df318e0
	ldr sp, =0x9df31000
    sub r1, sp, #STACK_SIZE

    //save svc mode
    mrs r3, cpsr

	//設定 irq stack
    mov r2, #0x12 		
    msr cpsr_cxsf, r2
    mov sp, r1
    sub r1, sp, #STACK_SIZE

    //回到 svc mode
    msr cpsr_cxsf, r3

	/** 跳到kernal_entry*/
	bl 		kernal_entry
/*
.loop: 
	b 		.loop
*/
/***********************************************************************************************/
/***********************************************************************************************/



/***********************************************************************************************/
/***********************************************************************************************/
.align	4
.global _call_sched
_call_sched:
	//r0 = schedFuncContextPtr =0x9df31000
	mov		sp, r0
	ldmia 	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	bx 		lr

/***********************************************************************************************/
/***********************************************************************************************/