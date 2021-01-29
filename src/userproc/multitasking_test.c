
#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/usyscall.h"


#define LOOP_NUM	50



void usertask0(void){
	uprintf("Starting User Task 0 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{	
		tid = __gettid() ;	
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		//__yield() ;	
		k++ ;
	}
	__exit();
}


void usertask1(void){
	uprintf("Starting User Task 1 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
    	//__print_hello(&k);
		//__yield() ;			
		k++ ;
	}
	__exit();
}

void usertask2(void){
	uprintf("Starting User Task 2 \r\n");

	int k = 0 ;
	int tid = -1 ;
	while(k<LOOP_NUM)
	{
		tid = __gettid() ;
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		//__yield() ;			
		k++ ;
	}
	__exit();
}

void usertask3(void){
	uprintf("Starting User Task 3 \r\n");

	int k = 0 ;
	int tid = -1 ;

	while(k<LOOP_NUM)
	{
		tid = __gettid() ;	
		uprintf("tid=%d,#%d\r\n" ,tid ,k);
		//__yield() ;	
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
		//__yield() ;		
		k++ ;
	}
	__exit();
}


int multitasking_test_main(void)
{
	uprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Now test multitasking.\r\n") ;

	__do_taskCreate(&usertask0 ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&usertask1 ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&usertask2 ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&usertask3 ,HIGHEST_PRIORITY) ;
	__do_taskCreate(&usertask4 ,HIGHEST_PRIORITY) ;


	return 0 ;
}