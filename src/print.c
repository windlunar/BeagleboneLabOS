
#include "print.h"
#include "uart.h"



void terminal_printC(int character){
    //Print by the UART ,UART0 can be the terminal
    uart_putC(UART0_PTR ,character) ;
    //Here,you can place the function to print the information.
    //Like LCD, HDMI ...
}


static void print_char(int character ,int *print_count){
    terminal_printC(character) ;
    *print_count++ ;
}


int kprintf(const char *fmt, ...){
    /** va_start初始化args, fmt指向輸入字串*/
    va_list args ;
    va_start(args ,fmt) ;
    int i = vkprintf(fmt ,args) ;

    /** 結束獲取可變參數*/
    va_end(args) ;
    return i ;
}


int vkprintf(const char *fmt, va_list args){
    int print_count ;
    vprintfmt((void *)print_char, &print_count, fmt, args);
    return print_count ;
}


/** function pointer形式 :
 * void (*function)(int, int) ,void is the return value, (int, int) is the input args.
 * put_char is the function pointer point to function print_char
 * 
 * fmt is the string that is the first argument of kprintf function.
 * args is the variable arguments.
 */ 

int vprintfmt(void(*put_char)(int, int*) ,int *put_count ,const char *fmt ,va_list args){
    register int character ;

    while(1){
        //printf("字串",) : 當字串不為%時, 將字串依序印出
        //先判斷是否不等於, 然後才+1跳入while執行, 或是不執行
        //當遇到%時,不執行該迴圈,and then add the pointer fmt to move to the next char after % (like c,s,d,u,p...)
        // and then 往下執行label FORMAT_PRINT
        while((character = *(uint8 *)fmt++) != '%'){
            //如果遇到結束字元則跳出
            if(character == '\0'){
                return 0 ;
            }
            //印出字元 ,put_count會加一
            put_char(character ,put_count) ;
        }
        //An label
        FORMAT_PRINT:
            //判斷the next char after % (like c,s,d,u,p...), 以格式化輸出(like c,s,d,u,p...)
            //然後pointer fmt才+1 , move to the next char after (c,s,d,u,p...), 以繼續印出後面的字元
            switch(character = *(uint8 *)fmt++){
                //character
                case 'c':
                    put_char(va_arg(args, int),put_count);
                    break;                          
            }
    }
    return 0 ;
}


