#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"

TASK_t Task ;

void fork_test_main()
{

	int32_t child_tid =-1 ;
	int32_t tid_before_fork =-1 ;
	syscall_get_tid(&tid_before_fork) ;

	//複製task自己
	//這邊跟 linux不一樣的是 ,返回的都是 child的 tid
	//所以用fork前跟fork後來判斷誰是parrent ,誰是child
	kprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test syscall fork function.\r\n") ;
	syscall_fork(&child_tid) ;

	int32_t tid_after_fork =-1 ;
	syscall_get_tid(&tid_after_fork) ;

	if(tid_before_fork != tid_after_fork)
	{
		for(int32_t i=0 ;i<10000;i++);
		kprintf("I'm parrent task ,tid=%d\r\n",tid_after_fork);
		syscall_exit() ;

	}
	else
	{
		kprintf("I'm child task., tid=%d\r\n" ,tid_after_fork) ;
		syscall_exit() ;
	}



/************************************************************************************************/

	for(;;) ;
}