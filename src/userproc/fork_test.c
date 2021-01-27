#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"

TASK_INFO_t Task0 ;

int fork_test()
{
	MEM_PART_INFO_t *mempart = alloc_one_mem_part();
	mempart->part_status = INUSE_FULL ;
	taskCreate(&Task0 ,&fork_test_main ,mempart->m_top-TASK_STACK_SIZE);
	task_enqueue(&Task0) ; 
	
	return 0 ;
}

void fork_test_main()
{

	int32_t child_tid =-1 ;

	kprintf("\r\n+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test syscall fork function.\r\n") ;

	// 複製task自己
	int r = syscall_fork(&child_tid) ;
	kprintf("After fork ,r_=%d\r\n",r);

	if(r != 0)
	{
		for(int32_t i=0 ;i<100000;i++);
		int32_t ptid =-1 ;
		syscall_get_tid(&ptid) ;
		kprintf("I'm parrent task ,tid=%d\r\n",ptid);
		syscall_exit() ;

	}
	else
	{
		int32_t ctid =-1 ;
		syscall_get_tid(&ctid) ;
		kprintf("I'm child task., tid=%d\r\n" ,ctid) ;
		syscall_exit() ;
	}



/************************************************************************************************/

	for(;;) ;
}