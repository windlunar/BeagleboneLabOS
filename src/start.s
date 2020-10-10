//.thumb
//.syntax unified
.equ UART0_BASE, 0x44E09000

.globl WRITE_REG32
WRITE_REG32:
	str 	r1,[r0]
	bx 		lr
	
.globl READ_REG32
READ_REG32:
	ldr 	r0,[r0]
	bx 		lr

.globl WR_REG16
WR_REG16:
	strh 	r1,[r0]
	bx 		lr
	
.globl RD_REG16
RD_REG16:
	ldrh 	r0,[r0]
	bx 		lr
	
.globl WR_REG8
WR_REG8:
	strb 	r1,[r0]
	bx 		lr
	
.globl RD_REG8
RD_REG8:
	ldrb 	r0,[r0]
	bx 		lr
	


.globl READ_CPSR
READ_CPSR:
	mrs 	r0, cpsr
	bx 		lr


.globl READ_SP
READ_SP:
	mov 	r0, r13					//r13 is stack pointer
	bx 		lr

.globl READ_CP15_c1
READ_CP15_c1:
	MRC     p15, #0, r0, c1, c0, #0
	bx 		lr

.globl READ_VECTOR_BASE
READ_VECTOR_BASE:
	/**
	 * CP15 c12暫存器的 bit[31:5]存放 exception vector base address
	 * exception vectors offset
	 * 
	 * 0x00 Reset 				Reset
	 * 0x04 Undefined 			PC = [base + 24h]
	 * 0x08 SWI 				PC = [base + 28h]
	 * 0x0C Pre-fetch abort 	PC = [base + 2Ch]
	 * 0x10 Data abort 			PC = [base + 30h]
	 * 0x14 Unused 				PC = [base + 34h]
	 * 0x18 IRQ 				PC = [base + 38h]
	 * 0x20 FIQ 				PC = [base + 3Ch]
	 */ 
	mrc 	p15, #0, r0, c12, c0, #0
	bx 		lr

				


.globl _start
_start:
	//MRC     p15, #0, r0, c1, c0, #0			//load CP15 to r0
	//orr 	r0, r0, #0x00002000 			//Set CP15 bit13 = 1
	//MCR     p15, #0, r0, c1, c0, #0			//store r0 to CP15

	/**
	 * U-Boot 2017.01 (Sep 13 2020 - 04:52:37 +0800)   
	 * 
	 * 0x9ff52000為 exception vector's base addr
	 * SMI at 0x9ff52008 -> jump to 0x9ff52024
	 * So svc_handler's address should load to 0x9ff52024
	 */
	/*設定svc_handler 的entry*/ 
	ldr r0, =0x9ff52024

	ldr r1, =svc_handler
	str r1, [r0]

	bl 		kernal_entry

.loop: 
	b 		.loop






