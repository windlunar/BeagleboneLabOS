
#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/usyscall.h"


#define LOOP_NUM	5



void prio_test1(void){
	__do_taskCreate(&prio_test2 ,3) ;
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		k++ ;
	}
	__exit();
}


void prio_test2(void){
	
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		if(k == 2) __do_taskCreate(&prio_test3 ,2) ;
		k++ ;
	}
	__exit();
}

void prio_test3(void){
	__do_taskCreate(&prio_test4 ,2) ;
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		k++ ;
	}
	__exit();
}

void prio_test4(void){
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		if(k == 1) __do_taskCreate(&prio_test5 ,1) ;
		k++ ;
	}
	__exit();
}


void prio_test5(void){
	__do_taskCreate(&prio_test6 ,1) ;
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		k++ ;
	}
	__exit();
}


void prio_test6(void){
	__do_taskCreate(&prio_test7 ,1) ;
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		k++ ;
	}
	__exit();
}


void prio_test7(void){
	
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		if(k==1) __do_taskCreate(&prio_test8 ,0) ;
		k++ ;
	}
	__exit();
}


void prio_test8(void){
	int k = 1 ;
	int tid = -1 ;
	int prio = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		prio = __get_task_priority() ;
		uprintf("tid=%d-prio=%d,#%d\r\n" ,tid ,prio ,k);
		k++ ;
	}
	__exit();
}



int priority_test_main(void)
{
	uprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Now test priority base multitasking.\r\n") ;

	__do_taskCreate(&prio_test1 ,3) ;
	
	for(int i = 0 ; i<100000;i++) ;
	return 0 ;
}