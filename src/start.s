.equ CM_PER_GPIO1_CLKCTRL, 0x44e000AC
.equ GPIO1_OE, 0x4804C134
.equ GPIO1_SETDATAOUT, 0x4804C194

_start:
   	mrs r0, cpsr
   	bic r0, r0, #0x1F @ clear mode bits
   	orr r0, r0, #0x13 @ set SVC mode
   	orr r0, r0, #0xC0 @ disable FIQ and IRQ
   	msr cpsr, r0

	bl _main


.loop: b .loop


.globl WR_REG32
WR_REG32:
	str r1,[r0]
	bx lr
	
.globl RD_REG32
RD_REG32:
	ldr r0,[r0]
	bx lr

.globl WR_REG16
WR_REG16:
	strh r1,[r0]
	bx lr
	
.globl RD_REG16
RD_REG16:
	ldrh r0,[r0]
	bx lr
	
.globl WR_REG8
WR_REG8:
	strb r1,[r0]
	bx lr
	
.globl RD_REG8
RD_REG8:
	ldrb r0,[r0]
	bx lr
	