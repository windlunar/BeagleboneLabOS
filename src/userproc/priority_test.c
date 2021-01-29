
#include "usrtasks.h"
#include "../klib/print.h"
#include "../klib/usyscall.h"


#define LOOP_NUM	8



void prio_test(void){
	kprintf("Starting prio_test_%d \r\n" ,__gettid());

	int k = 0 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		kprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		k++ ;
	}
	__exit();
}




int priority_test_main(void)
{
	for(int32_t i=0 ;i<200000;i++);
	kprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	kprintf("Now test priority base multitasking.\r\n") ;

	__do_taskCreate(&prio_test ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&prio_test ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&prio_test ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&prio_test ,1) ;
	__do_taskCreate(&prio_test ,1) ;
	__do_taskCreate(&prio_test ,2) ;
	__do_taskCreate(&prio_test ,3) ;
	__do_taskCreate(&prio_test ,3) ;
	__do_taskCreate(&prio_test ,3) ;
	__do_taskCreate(&prio_test ,4) ;


	for(int32_t i=0 ;i<1000000;i++);

	return 0 ;
}