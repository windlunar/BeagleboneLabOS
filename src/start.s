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
	
/*
The MIT License (MIT)

Copyright (c)  Alexis Marquet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/