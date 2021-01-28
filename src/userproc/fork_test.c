#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"
#include "../klib/usyscall.h"

TASK_INFO_t Task0 ;

int fork_test()
{
	MEM_PART_INFO_t *mempart = memPartAlloc();
	mempart->part_status = INUSE_FULL ;
	taskCreate(&Task0 ,&fork_test_main ,stktop2bottom(mempart->m_top));
	task_enqueue(&Task0) ; 
	
	return 0 ;
}

void fork_test_main()
{

	int32_t child_tid =-1 ;

	kprintf("\r\n+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test syscall fork function.\r\n") ;

	// 複製task自己
	int r = __fork() ;
	kprintf("After fork ,r_=%d\r\n",r);

	if(r != 0)
	{
		for(int32_t i=0 ;i<100000;i++);
		int ptid = __gettid() ;
		kprintf("I'm parrent task ,tid=%d\r\n",ptid);
		__exit() ;

	}
	else
	{
		int ctid = __gettid() ;
		kprintf("I'm child task., tid=%d\r\n" ,ctid) ;
		__exit() ;
	}



/************************************************************************************************/

	for(;;) ;
}