
#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/usyscall.h"
#include "../kernel/task.h"


#define LOOP_NUM	10



void usertask0(void){
	uprintf("Starting User Task 0 \r\n");
	__do_taskCreate(&usertask1 ,HIGHEST_PRIORITY) ;

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		k++ ;
	}
	__exit();
}


void usertask1(void){
	uprintf("Starting User Task 1 \r\n");
	__do_taskCreate(&usertask2 ,HIGHEST_PRIORITY) ;

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		k++ ;
	}
	__exit();
}

void usertask2(void){
	uprintf("Starting User Task 2 \r\n");
	__do_taskCreate(&usertask3 ,HIGHEST_PRIORITY) ;

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		k++ ;
	}
	__exit();
}

void usertask3(void){
	uprintf("Starting User Task 3 \r\n");
	__do_taskCreate(&usertask4 ,HIGHEST_PRIORITY) ;

	int k = 0 ;
	int tid = -1 ;

	while(k<LOOP_NUM)
	{
		tid = __gettid() ;	
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		k++ ;
	}
	__exit();
}


void usertask4(void){

	uprintf("Starting User Task 4 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		k++ ;
	}
	__exit();
}


int multitasking_test_main(void)
{
	uprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Now test multitasking.\r\n") ;

	__do_taskCreate(&usertask0 ,HIGHEST_PRIORITY) ;

	for(int i = 0 ; i<200000;i++) ;

	return 0 ;
}