#include "../driver/uart.h"
#include "../klib/queue.h"
#include "../klib/console.h"
#include "../klib/mem.h"
#include "../klib/string.h"
#include "../klib/usyscall.h"
#include "../klib/std_io.h"
#include "usrtasks.h"

////////////////////////////////////////////////////////////////////////////////////
#include "../kernel/task.h"
////////////////////////////////////////////////////////////////////////////////////

void main_shell()
{
/***********************************************************************************/
// Test
/***********************************************************************************/

/***********************************************************************************/
	draw_console();
	uprintf("\r\nWelcome to Beaglebone black OS!\r\n") ;
	uprintf("\r\nThe command line is the first process\r\n") ;

	uprintf("\r\nThe task id :%d\r\n",__gettid()) ;
	uprintf("The task priority :%d\r\n",__get_task_priority()) ;

	char cwd[32] ;
	_memset(cwd ,0 ,sizeof(cwd)) ;
	__getcwd(cwd ,sizeof(cwd)) ;
	put_str("Current dir :") ;
	put_str(cwd) ;
	put_str("\r\n\0") ;

	uprintf("\r\n|cmd>") ;

	char byte = '\0' ;
	char cmdbuf[16] ;
	_memset((void *)cmdbuf ,0 ,sizeof(cmdbuf)) ;
	char *cmd = cmdbuf ;
	char *s = cmd;

	//command line
	while(1)
	{
		//獲取在console輸入的字元
		byte = get_c() ;

		//在console上印出輸入的字元
		uprintf("%c" ,byte) ;
		
		if(strlen(cmd) == 9)
		{
			_memset((void *)cmdbuf ,0 ,10) ;
			s = cmd ;
		}
		if((byte!=0x08) && (byte!=0x0d))
		{
			*s = byte ;
			s++ ;
		}

		switch (byte)
		{
		case 0x08 : //Backspace
			put_c(' ') ;
			put_c(0x08) ;
			_memset((void *)cmdbuf ,0 ,10) ;
			s = cmd ;
			break;
		
		case 0x0d:	//Enter ,按下Enter鍵後的處理
			if (strcmp(cmd ,"test\0")==0){
				uprintf("\r\n");
				uprintf("Please key\r\n");
				uprintf("	'1' for fork function test.\r\n") ;
				uprintf("	'2' for multitasking test\r\n") ; 
				uprintf("	'3' for priority-based multitasking test.\r\n") ;
				uprintf("	'4' for tty0 open test.\r\n") ;
				uprintf("Or '0' break.\r\n") ;

				while(1)
				{
					//獲取在console輸入的字元
					uint8_t get = get_c() ;

					//在console上印出輸入的字元
					uprintf("%c" ,get) ;
					
					if(get == '1'){
						// Test Fork function
						fork_test() ;
						for(int i = 0 ; i<20000;i++) ;
						break ;

					}else if(get == '2'){
						// 測試 multitasking							
						// Time slice base. Round Robin Multitasking    
						multitasking_test_main() ;
						for(int i = 0 ; i<20000;i++) ;
						break ;

					}else if(get == '3'){
						// 測試 priority							
						// Time slice base. priority base Multitasking    
						priority_test_main() ;
						for(int i = 0 ; i<500000;i++) ;
						break ;

					}else if(get == '4'){
						// 測試 tty0							
						tty0_test_main() ;
						for(int i = 0 ; i<20000;i++) ;
						break ;

					}else if(get == '0'){
						break ;
					}
				}

			}else if(strcmp(cmd ,"help\0")==0){
				uprintf("\r\n");
				uprintf("help:\r\n") ;
				uprintf("Available command :test ,info ,help\r\n") ;

			}else if(strcmp(cmd ,"info\0")==0){
				uprintf("\r\n");
				uprintf("A simple os running on Beaglebone black\r\n\r\n") ;
				uprintf("Feature:\r\n") ;
				uprintf("  Time slice based round-robin multitasking.\r\n");
				uprintf("  Simple memory management.\r\n");
				uprintf("  Simple command line.\r\n");
			}

			_memset((void *)cmdbuf ,0 ,10) ;
			s = cmd ;

			__write(FD_CONSOLE_OUT ,"\r\n|cmd>",7) ;
			break;
		
		default:
			break;
		}
	}
	
	for(;;) ;
}