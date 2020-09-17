
#include "print.h"
#include "uart.h"



void terminal_printC(int character){
    uart_putC(UART0_PTR ,character) ;
}

static void print_char(int chartr ,int *print_count){
    terminal_printC(chartr) ;
    *print_count++ ;
}

int kprintf(const char *fmt, ...){
    /** va_start初始化args, fmt指向前一個參數*/
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

//function pointer形式 :
//void (*function)(int, int)
int vprintfmt(void(*put_char)(int, int*) ,int *put_count ,const char *fmt ,va_list args){
    register int character ;

    while(1){
        while((character = *(uint8 *)fmt++) != '%'){
            //fmt++ ;
            //如果遇到結束字元則跳出
            if(character == '\0'){
                return 0 ;
            }
            //印出字元 ,put_count會加一
            put_char(character ,put_count) ;
        }

        //An label
        START:
            //判斷格式化輸出
            switch(character = *(uint8 *)fmt++){
                //character
                case 'c':
                    put_char(va_arg(args, int),put_count);
                    break;          
                
            }
    }
    return 0 ;
}


