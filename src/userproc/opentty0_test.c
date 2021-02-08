
#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/usyscall.h"





void tty0_test(void){
	uprintf("Starting tty0_test_%d \r\n" ,__gettid());

	int fd = __open(TTY0) ; 

	if (fd < 0) {
		uprintf("Error ,fd =%d ,halt\r\n" ,fd) ;
		for(;;) ;
	}

	uprintf("Successfully open tty0 ,fd =%d\r\n",fd) ;
	__write(fd ,"Write tty0 test\r\n\0",18) ;

	__exit();
}




int tty0_test_main(void)
{
	uprintf("+++++++++++++++++++++++++++++++++++++++++\r\n") ;
	uprintf("Now test open tty0.\r\n") ;

	__do_taskCreate(&tty0_test ,HIGHEST_PRIORITY) ;

	return 0 ;
}