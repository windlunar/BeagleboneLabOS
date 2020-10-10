.thumb
.syntax unified

.global activate
activate:
	/* save kernel state */
	mrs ip, psr
	push {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}

	/* switch to process stack */
	msr psp, r0
	mov r0, #3
	msr control, r0

	/* load user state */
	pop {r4, r5, r6, r7, r8, r9, r10, r11, lr}

	/* jump to user task */
	bx lr


/** .type symbol,%function to indicate that the label symbol
 *  corresponds to a procedure entry
 */
.type svc_handler, %function    
.global svc_handler
svc_handler:
    /* Save user state*/
    mrs r0, psp
    stmdb r0!, {r4, r5, r6, r7, r8 ,r9 ,r10 ,r11 ,lr}

    /* Restore kernal state*/
    pop {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}
    msr psr, ip
    bx lr

.global task_env_init
task_env_init:
	/* save kernel state */
	mrs ip, psr
	push {r4, r5, r6, r7, r8, r9, r10, r11, ip, lr}

	/* switch to process stack */
	msr psp, r0
	mov r0, #3
	msr control, r0

	isb
	bl sys_call
	bx lr




/** 
 *Cortex-M3 Control Register
 *CONTROL[1] Stack status:
 *		1 = Alternate stack is used
 *		0 = Default stack (MSP) is used
 *	If it is in the thread or base level, the alternate stack is the PSP. There is no
 *	alternate stack for handler mode, so this bit must be 0 when the processor is in
 *	handler mode.
 *CONTROL[0]
 *		0 = Privileged in thread mode
 *		1 = User state in thread mode
 *	If in handler mode (not thread mode), the processor operates in privileged mode.
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