#include "usrtasks.h"
#include "../klib/print.h"
#include "../klib/queue.h"
#include "../klib/usyscall.h"

//#include "../kernel/memory.h"
#include "../kernel/debug.h"
#include "../kernel/task.h"


int fork_test()
{
	__do_taskCreate(&fork_test_main) ;
	
	return 0 ;
}

void fork_test_main()
{
	kprintf("\r\n+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Test syscall malloc_blk function.\r\n") ;

	//目前的情況, child的blks會被init
	void *p1 = __malloc_blk() ;
	kprintf("1st blk start addr =%p\r\n" ,p1) ;
	void *p2 = __malloc_blk() ;
	kprintf("2nd blk start addr =%p\r\n" ,p2) ;
	void *p3 = __malloc_blk() ;
	kprintf("3rd blk start addr =%p\r\n" ,p3) ;

	// For debug
	MEM_AREA_INFO_t *curr_ma = which_mem_area(curr_running_task->stk_bottom) ;
	print_from_blk_head(curr_ma) ;

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