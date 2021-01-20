#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"

TASK_INFO_t Task ;
TASK_INFO_t Task0 ;

int fork_test()
{
	MEM_PART_INFO_t *mempart = alloc_one_mem_part();
	mempart->part_status = INUSE_FULL ;
	taskCreate(&Task0 ,&fork_test_main ,mempart->mempart_top_ptr-TASK_STACK_SIZE);
	task_enqueue(&Task0) ; 
	
	return 0 ;
}

void fork_test_main()
{

	int32_t child_tid =-1 ;

	kprintf("\r\n+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test syscall fork function.\r\n") ;

	// 複製task自己
	// 與一般fork不一樣的是 ,返回的都是 child的 tid
	// 所以用child_tid跟fork後syscall_get_tid來判斷誰是parrent ,誰是child
	syscall_fork(&child_tid) ;

	int32_t tid_after_fork =-1 ;
	syscall_get_tid(&tid_after_fork) ;
	
	kprintf("tid_after_fork=%d\r\n",tid_after_fork);

	if(child_tid != tid_after_fork)
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