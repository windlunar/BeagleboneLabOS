#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/queue.h"
#include "../klib/usyscall.h"



int fork_test()
{
	__do_taskCreate(&fork_test_main ,HIGHEST_PRIORITY) ;
	
	return 0 ;
}

void fork_test_main()
{
	uprintf("\r\n+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Test syscall malloc_blk and mfree_blk function.\r\n") ;

	//目前的情況, child的blks會被init
	void *p1 = __malloc_blk() ;
	uprintf("1st blk start addr =%p\r\n" ,p1) ;
	void *p2 = __malloc_blk() ;
	uprintf("2nd blk start addr =%p\r\n" ,p2) ;
	void *p3 = __malloc_blk() ;
	uprintf("3rd blk start addr =%p\r\n" ,p3) ;

	__mfree_blk(p2) ;
	__mfree_blk(p1) ;
	__mfree_blk(p3) ;

	//uprintf("\r\nPrint current memory area block list:\r\n\r\n") ;
	//__get_mblk_list() ;	//for test and debug

	uprintf("\r\n+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Now test syscall fork function.\r\n") ;

	// 複製task自己
	int r = __fork() ;
	uprintf("After fork ,r_=%d\r\n",r);

	if(r != 0)
	{
		for(int32_t i=0 ;i<100000;i++);
		int ptid = __gettid() ;
		uprintf("I'm parrent task ,tid=%d\r\n",ptid);
		__write(FD_CONSOLE_OUT ,"syscall __write() test\r\n\0" ,25) ;
		__exit() ;

	}
	else
	{
		int ctid = __gettid() ;
		uprintf("I'm child task., tid=%d\r\n" ,ctid) ;
		__write(FD_CONSOLE_OUT ,"syscall __write() test\r\n\0" ,25) ;
		__exit() ;
	}

}