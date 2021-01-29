#include "../driver/uart.h"
#include "../kernel/kprint.h"
#include "../klib/queue.h"
#include "../klib/console.h"
#include "../klib/mem.h"
#include "../klib/string.h"
#include "../klib/usyscall.h"
#include "usrtasks.h"


#include "../kernel/file.h"
#include "../kernel/task.h"

void main_shell()
{
/***********************************************************************************/
// Test
/***********************************************************************************/

/***********************************************************************************/
	draw_console();
	kprintf("\r\nWelcome to Beaglebone black OS!\r\n") ;
	kprintf("\r\nThe command line is the first process\r\n") ;

	kprintf("\r\nThe task id :%d\r\n",__gettid()) ;
	kprintf("The task priority :%d\r\n",__get_task_priority()) ;

	kprintf("\r\n|cmd>") ;

	char byte = '\0' ;
	char cmdbuf[10] ;
	_memset((void *)cmdbuf ,0 ,10) ;
	char *cmd = cmdbuf ;
	char *s = cmd;

	//command line
	while(1)
	{
		//獲取在console輸入的字元
		byte = uart_getC(UART0_PTR) ;

		//在console上印出輸入的字元
		kprintf("%c" ,byte) ;
		
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
			uart_putC(UART0_PTR ,' ') ;
			uart_putC(UART0_PTR ,0x08) ;
			_memset((void *)cmdbuf ,0 ,10) ;
			s = cmd ;
			break;
		
		case 0x0d:	//Enter ,按下Enter鍵後的處理
			if (strcmp(cmd ,"test\0")==0){
				kprintf("\r\n");
				kprintf("Please key '1' for fork function test or '2' for multitasking test\r\n") ; 
				kprintf(",or '3' for priority-based multitasking test ,or '0' break.\r\n") ;

				while(1)
				{
					//獲取在console輸入的字元
					uint8_t get = uart_getC(UART0_PTR) ;

					//在console上印出輸入的字元
					kprintf("%c" ,get) ;
					
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

					}else if(get == '0'){
						break ;
					}
				}
				
			}else if(strcmp(cmd ,"help\0")==0){
				kprintf("\r\n");
				kprintf("help:\r\n") ;
				kprintf("Available command :test ,info ,help\r\n") ;

			}else if(strcmp(cmd ,"info\0")==0){
				kprintf("\r\n");
				kprintf("A simple os running on Beaglebone black\r\n\r\n") ;
				kprintf("Feature:\r\n") ;
				kprintf("  Time slice based round-robin multitasking.\r\n");
				kprintf("  Simple memory management.\r\n");
				kprintf("  Simple command line.\r\n");
			}

			_memset((void *)cmdbuf ,0 ,10) ;
			s = cmd ;

			uart_tx_str(UART0_PTR ,"\r\n|cmd>",7) ;
			break;
		
		default:
			break;
		}
	}
	
	for(;;) ;
}