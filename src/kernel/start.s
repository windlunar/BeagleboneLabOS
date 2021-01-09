
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
	//r13 is stack pointer
	mov 	r0, r13					
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

				


.globl _start
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
	ldr r0, =0x9ff52024

	ldr r1, =svc_handler
	str r1, [r0]

	/** 跳到kernal_entry*/
	bl 		kernal_entry

.loop: 
	b 		.loop






