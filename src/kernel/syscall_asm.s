
#include "syscall.h"
.equ	ostick_msec	, 5


/** 
 * CPSR MODE FIELD :
 */ 
.equ 	CPSR_M_USR,   0x10U		// CPSR: M User mode (PL0)
.equ 	CPSR_M_FIQ,   0x11U		//CPSR: M Fast Interrupt mode (PL1)
.equ 	CPSR_M_IRQ,   0x12U		//CPSR: M Interrupt mode (PL1) 
.equ 	CPSR_M_SVC,   0x13U		//CPSR: M Supervisor mode (PL1)
.equ 	CPSR_M_MON,   0x16U		//CPSR: M Monitor mode (PL1) 
.equ 	CPSR_M_ABT,   0x17U		//CPSR: M Abort mode (PL1)  
.equ 	CPSR_M_HYP,   0x1AU		//CPSR: M Hypervisor mode (PL2) 
.equ 	CPSR_M_UND,   0x1BU		//CPSR: M Undefined mode (PL1) 
.equ 	CPSR_M_SYS,   0x1FU		//CPSR: M system mode 

/************************************************************************************************/
//Define syscall id
.equ	 SYSCALL_ID_print_hello 		,   	0
.equ	 SYSCALL_ID_yield 				,   	1
.equ	 SYSCALL_ID_get_tid				,		2
.equ	 SYSCALL_ID_exit				,		3
.equ	 SYSCALL_ID_fork				,		4
.equ	 SYSCALL_ID_do_taskCreate		,		5
.equ	 SYSCALL_ID_malloc_blk			,		6
.equ	 SYSCALL_ID_mfree_blk			,		7
.equ	 SYSCALL_ID_get_mblk_list		,		8
.equ	 SYSCALL_ID_get_task_priority	,		9
.equ	 SYSCALL_ID_write				,		10
.equ	 SYSCALL_ID_read				,		11
.equ	 SYSCALL_ID_open				,		12
.equ	 SYSCALL_ID_getcwd				,		13
.equ	 SYSCALL_ID_getsubdir			,		14
.equ	 SYSCALL_ID_getfdir             ,		15
.equ	 SYSCALL_ID_chdir               ,		16
.equ	 SYSCALL_ID_getfullpath         ,		17
.equ	 SYSCALL_ID_restart				,		18
.equ	 SYSCALL_ID_close               ,		19
/************************************************************************************************/



/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_print_hello; 
.align	2
syscall_print_hello:
	//保存傳入參數
	push {r0 ,r2 ,lr}
	mov	r2 ,r0
	mov r0, #SYSCALL_ID_print_hello
	svc 0x00
	pop	{r0 ,r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx lr	//返回 user proc




/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_yield; 
.align	2
syscall_yield: 
	push {r0 ,lr}
	mov r0, #SYSCALL_ID_yield

	/**
	* 在這邊 reload ostimer counter ,tick =1ms (mov r0 #1)
	* 而不要在初始化 timer就load的話
	* 看起來可以解決卡住跳不出去的問題
	*/
	stmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}
	mov		r0 ,#(ostick_msec)
	bl 		reloadOsTick
	ldmfd 	sp!,	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10 ,r11 ,ip ,lr}

	svc 0x00
	pop	{r0 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx lr	//返回 user proc




/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_get_tid; 
.align	2
syscall_get_tid:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_get_tid
	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr							/* 返回 user proc */
	

/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_exit; 
.align	2
syscall_exit:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_exit

	svc 	0x00

	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr							/* 返回 user proc */



/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_fork; 
.align	2
syscall_fork:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_fork

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */




/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_do_taskCreate; 
.align	2
syscall_do_taskCreate:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_do_taskCreate

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_malloc_blk; 
.align	2
syscall_malloc_blk:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_malloc_blk

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */




/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_mfree_blk; 
.align	2
syscall_mfree_blk:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_mfree_blk

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */


/*****************************************************************************************/
//
//
/*****************************************************************************************/
.global syscall_get_mblk_list; 
.align	2
syscall_get_mblk_list:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_get_mblk_list

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
// 
// 這邊如果加上 reloadOsTick 的話, priority_test會不正常
// 推測原因應該是, task中的printk不到5ms就執行完了 ,然後在下個迴圈馬上跳進syscall ,
// 這時如果執行 reloadOsTick的話 time slice會一直重計時,而不會達到切換任務的時間 ,
// 直到所有的迴圈執行完畢才會換到下一個task
/*****************************************************************************************/
.global syscall_get_task_priority; 
.align	2
syscall_get_task_priority:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_get_task_priority

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_write; 
.align	2
syscall_write:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_write

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */


/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_read; 
.align	2
syscall_read:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_read

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_open; 
.align	2
syscall_open:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_open

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */




/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_getcwd; 
.align	2
syscall_getcwd:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_getcwd

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_getsubdir; 
.align	2
syscall_getsubdir:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_getsubdir

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */




/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_getfdir; 
.align	2
syscall_getfdir:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_getfdir

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_chdir; 
.align	2
syscall_chdir:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_chdir

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */


/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_getfullpath; 
.align	2
syscall_getfullpath:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_getfullpath

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */



/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_restart; 
.align	2
syscall_restart:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_restart

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr						/* 返回 user task */


/*****************************************************************************************/
// 
/*****************************************************************************************/
.global syscall_close; 
.align	2
syscall_close:
	push 	{r2 ,lr}
	mov		r2 ,r0
	mov 	r0, #SYSCALL_ID_close

	svc 	0x00
	pop		{r2 ,lr}
	msr     CPSR_c, #CPSR_M_USR
	bx 		lr							/* 返回 user task */