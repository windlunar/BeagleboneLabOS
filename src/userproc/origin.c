#include "usrtasks.h"
#include "../klib/print.h"
#include "../kernel/task.h"
#include "../klib/queue.h"
#include "../kernel/memory.h"
#include "../klib/console.h"
#include "../driver/uart.h"
#include "../klib/mem.h"
#include "../klib/string.h"

TASK_t Task[6] ;

#define TEST_CMD	"test\0"


void main_origin()
{
	//draw_console();
	
/************************************************************************************************/
// Test Fork function
/************************************************************************************************/
	//fork_test() ;

/************************************************************************************************/
// 測試 multitasking
// Time slice base. Round Robin Multitasking
/************************************************************************************************/
	//multitasking_test() ;

/************************************************************************************************/
// A simple command line interface
/************************************************************************************************/

	draw_console();
	kprintf("\r\nWelcome to Beaglebone black OS!\r\n") ;
	kprintf("\r\nThe command line is the first process\r\n") ;

	int tid = -1;
	syscall_get_tid(&tid);
	kprintf("\r\nThe task id :%d\r\n",tid) ;

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
			break;
		
		case 0x0d:	//Enter ,按下Enter鍵後的處理
			if (strcmp(cmd ,"test\0")==0){
				kprintf("\r\n");
				kprintf("Please key '1' for fork function test or '2' for multitasking test\r\n") ; 
				kprintf(",or '3' break.\r\n") ;

				while(1)
				{
					//獲取在console輸入的字元
					uint8_t get = uart_getC(UART0_PTR) ;

					//在console上印出輸入的字元
					kprintf("%c" ,get) ;
					
					if(get == '1'){
						fork_test() ;
						break ;

					}else if(get == '2'){
						multitasking_test() ;
						break ;

					}else if(get == '3'){
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