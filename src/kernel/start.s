
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
	
	
.type _start, %function 
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
	 * bit[31:5] of CP15 c12 register keeps the exception vector base.
	 * exception vectors offset
	 */ 
	mrc 	p15, #0, r0, c12, c0, #0
	bx 		lr

/****************************************************************************************/ 
 // c :  CPSR中的control field ( PSR[7:0])
 // f :  flag field (PSR[31:24])
 // x :  extend field (PSR[15:8])
 // s :  status field ( PSR[23:16])
 // 				
/****************************************************************************************/
 //.equ exception_vector_base,	0x9ff52000
.equ kernel_stack_top ,0x9df3fffc
.equ NON_KSTACK_SIZE, 4096
/****************************************************************************************/

.type _start, %function  
.align	2
.global _start
_start:
	/**
	 * U-Boot version :2017.01
	 */ 

	mrs r0, cpsr
	bic r0, r0, #0x1F 	/* clear mode bits */
	orr r0, r0, #0x13 	/* switch to SVC mode
	orr r0, r0, #0xC0 	/* disable FIQ and IRQ ,FIQ is not supported in AM335x devices. */
	msr cpsr, r0

	/**
	 * The address of below should place the branch instruction ,in order to branch to the
	 * actual entry of each exception
	 * 
	 * For instance ,the instruction at(base + 0x08) should make PC = [0x9ff52024] 
	 * (Branch to the address which store in 0x9ff52024)
	 * exception_vector_base +0x00		 	Reset 				
	 * exception_vector_base +0x04		 	Undefined 			
	 * exception_vector_base +0x08 			SWI 				
	 * exception_vector_base +0x0c 			Pre-fetch abort 	
	 * exception_vector_base +0x10	 		Data abort 			
	 * exception_vector_base +0x14 			Unused 				
	 * exception_vector_base +0x18 			IRQ 				
	 * exception_vector_base +0x1c 			FIQ 
	 */

	mrc 	p15, #0, r0, c12, c0, #0	/*Get the exception vector base from c12 */
	

	stmfd	sp!	,{r0-r3}
	bl 		set_exception_entry
	ldmfd	sp!	,{r0-r3}


	/**
	 * Setting the entry of exception handler
	 */

	/* exception_vector_base +0x24 stores the entry address of svc_entry */
	mrc 	p15, #0, r0, c12, c0, #0		/* Get the exception vector base from c12 */
	add	r0 ,r0 ,#0x24						/* r0 =exception_vector_base +0x24 */
	ldr r1, =svc_entry
	str r1, [r0]


	/* prefetch_abort */
	mrc 	p15, #0, r0, c12, c0, #0
	add	r0 ,r0 ,#0x28
	ldr r1, =prefetch_abort_entry
	str r1, [r0]


	/* data_abort */
	mrc 	p15, #0, r0, c12, c0, #0
	add	r0 ,r0 ,#0x2c
	ldr r1, =data_abort
	str r1, [r0]


	/* exception_vector_base +0x34 stores the entry address of irq_entry */
	mrc 	p15, #0, r0, c12, c0, #0
	add	r0 ,r0 ,#0x34					/* r0 =exception_vector_base +0x34 */
	ldr r1, =irq_entry
	str r1, [r0]


	/* fiq */


	/* Setup svc stack top */	
	ldr sp, =kernel_stack_top	
	add r1, sp, #NON_KSTACK_SIZE		/*Now r1 = kernel_stack_top +0x1000 */


	/* save svc cpsr */
	mrs r3, cpsr


	/* Setup irq stack top */
	mov r2, #0x12 		
	msr cpsr_cxsf, r2
	mov sp, r1							/* irq's sp = kernel_stack_top +0x1000 */
	add r1, sp, #NON_KSTACK_SIZE		/* Now r1 = kernel_stack_top +0x2000 */


	/* Setup abort(prefetch_abort and data_abort) stack top */
	mov r2, #0x17
	msr cpsr_cxsf, r2
	mov sp, r1							/* abort's sp = kernel_stack_top +0x2000 */
	add r1, sp, #NON_KSTACK_SIZE		/* Now r1 = kernel_stack_top +0x3000 */


	/* Back to svc mode */
	msr cpsr_cxsf, r3


	/* Branch to kernel entry */
	bl 		kernal_entry
