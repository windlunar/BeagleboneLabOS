.equ CM_PER_GPIO1_CLKCTRL, 0x44e000AC
.equ GPIO1_OE, 0x4804C134
.equ GPIO1_SETDATAOUT, 0x4804C194
.equ CONF_UART0_RXD, 0x44E10970
.equ CONF_UART0_TXD, 0x44E10974
.equ CM_WKUP_CLKSTCTRL, 0x44E00400
.equ CM_PER_L4HS_CLKSTCTRL, 0x44E0011C
.equ CM_WKUP_UART0_CLKCTRL, 0x44E004B4
.equ CM_PER_UART0_CLKCTRL, 0x44E0006C
.equ UART0_SYSC, 0x44E09054
.equ UART0_SYSS, 0x44E09058
.equ UART0_BASE, 0x44E09000


_start:
   	mrs 	r0, cpsr				//Read Current Program Status Register. 
   	bic 	r0, r0, #0x1F 			//Clear mode bits [4:0]
   	orr 	r0, r0, #0x13 			//Set SVC mode
   	orr 	r0, r0, #0xC0 			//Disable FIQ and IRQ
   	msr 	cpsr, r0

	ldr 	sp, =0x90000000  		//Set the stack pointer

	//b 		SET_UART0_BASE
	b 		MAIN

MAIN:
	bl 		_main

.loop: 
	b 		.loop





/* Set UART0 base address */
SET_UART0_BASE:
    ldr 	r1, =UART0_BASE

    //ldr 	r0, =0x12abcdef
	//mrc 	p15, 0, r0, c1, C0, 0	//Read CP15 c1 to r0 , for c1 bit0 = 0 : disable MMU

	mov 	r3, r0				
	ldr 	r4, =0					//Set counter in the loop
	
PRINT_REG:
	and 	r0, r3, #0xf0000000		//r3是要印出的值 ,先傳送bit[31:28]:跟0xf0000000 and ,存到r0 
	LSL 	r3, r3, #0x04			//左移四位以印出 bit[27:24] ,之後依此類推...
	LSR 	r0, r0, #28				//r0右移28位	

	cmp		r0, #0x09				//判斷是否是abcdef
	bcs		ABCDEF

	add 	r0, r0, #0x30			//r0加0x30變成 ASCII
	b 		PUT

PUT:
	bl 		.uart_putc
	
	add 	r4, r4, #0x1
    cmp  	r4 , #0x08        		//判斷 i 是否等於val       if(i==val)
    bne    	PRINT_REG         		//若不相等 跳轉至tx_reg處進入下次迴圈     else continue
	b 		MAIN

ABCDEF:
	add 	r0, r0, #0x57			//變成 ASCII
	b		PUT





.uart_putc:
    ldrb    r2, [r1, #20]
    uxtb    r2, r2
    tst     r2, #32
    beq     .uart_putc
    strb    r0, [r1]
    bx      lr


.globl WR_REG32
WR_REG32:
	str 	r1,[r0]
	bx 		lr
	
.globl RD_REG32
RD_REG32:
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
	