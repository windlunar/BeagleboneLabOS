
#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/usyscall.h"





void ipc0_test(void){
	uprintf("tid=%d \r\n" ,__gettid());

	int fd = __open(IPC0) ; 

	if (fd < 0) {
		uprintf("Error ,fd =%d ,exit\r\n" ,fd) ;
		__exit() ;
	}

	uprintf("Successfully open ipc0 ,fd =%d\r\n",fd) ;
	__write(fd ,"Send :Hello Message\r\n\0",23) ;

	__close(fd) ;

	__exit();
}




int ipc_test_main(void)
{
	uprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Test ipc0.\r\n") ;

	__do_taskCreate(&ipc0_test ,HIGHEST_PRIORITY) ;

	uprintf("tid=%d \r\n" ,__gettid());

	int fd = __open(IPC0) ; 

	if (fd < 0) {
		uprintf("Error ,fd =%d ,exit main\r\n" ,fd) ;
		return 0 ;
	}
	uprintf("Successfully open ,fd =%d\r\n",fd) ;

	char rbuf[32] ;
	_memset(&rbuf[0] ,0 ,sizeof(rbuf)) ;
	int r =__read(fd ,&rbuf[0] ,23) ;
	uprintf("r=%d\r\n" ,r) ;
	put_str("Get :") ;
	put_str(rbuf) ;

	__close(fd) ;

	return 0 ;
}