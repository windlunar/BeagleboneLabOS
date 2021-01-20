
#include "print.h"
#include "../driver/uart.h"

//__builtin_va_list is built in to the compiler ,not a macro
/**
 * __builtin_va_start(ap, last)
 * __builtin_va_arg(ap, type)
 * __builtin_va_end(ap)
 * 
 *  ap point to the first var of variable-arguments(可變參數)
 * last point to the previous argument
 * ex printf(const char * ,...)
 * ap point to const char * ,last point to ...
 * */

void terminal_printC(int32_t character){
    //Print by the UART ,UART0 can be the terminal
    uart_putC(UART0_PTR ,character) ;
    //Here,you can place the function to print the information.
    //Like LCD, HDMI ...
}


void print_char(int32_t character){
    terminal_printC(character) ;
}


int32_t kprintf(const char *fmt, ...){
    /** __builtin_va_start初始化args, fmt指向輸入字串*/
    __builtin_va_list args ;
    __builtin_va_start(args ,fmt) ;
    int32_t i = kprint(fmt ,args) ;

    /** 結束獲取可變參數*/
    __builtin_va_end(args) ;
    return i ;
}


/** function pointer形式 :
 * void (*function)(int, int) ,void is the return value, (int, int) is the input args.
 * print_char is the function pointer point to function print_char
 * 
 * fmt is the string that is the first argument of kprintf function.
 * args is the variable arguments.
 */ 

int32_t kprint(const char *fmt ,__builtin_va_list args){
    register int32_t character ;
    uint32_t va_val ;
    uint32_t base ;

    while(1){
        //printf("字串",) : 當字串不為%時, 將字串依序印出
        //先判斷是否不等於, 然後才+1跳入while執行, 或是不執行
        //當遇到%時,不執行該迴圈,and then add the pointer fmt to move to the next char after % (like c,s,d,u,p...)
        // and then 往下執行label FORMAT_PRINT
        character = *(uint8_t *)fmt ;
        while(character != '%')
        {
            //如果遇到結束字元則跳出
            if(character == '\0')
            {
                return 0 ; 
            }
            //印出字元
            print_char(character) ;

            (uint8_t *)fmt++ ;
            character = *(uint8_t *)fmt ;
        }

        //判斷the next char after % (like c,s,d,u,p...), 以格式化輸出(like c,s,d,u,p...)
        //然後pointer fmt才+1 , move to the next char after (c,s,d,u,p...), 以繼續印出後面的字元
        (uint8_t *)fmt++ ;
        character = *(uint8_t *)fmt ;

        switch(character)
        {
            //Character format
            case 'c':
                va_val = (uint32) __builtin_va_arg(args, int);
                print_char(va_val);
                break;   

            //Pointer format
            case 'p':
                print_char('0');
                print_char('x');
                //__builtin_va_arg返回可變參數的值
                va_val = (uint32) __builtin_va_arg(args, void *);
                //格式為16進位
                base = 16;
                print_va(va_val, base);
                break; 

            //Hex format
            case 'x':
                print_char('0');
                print_char('x');
                va_val = __builtin_va_arg(args, uint32);
                base = 16;
                print_va(va_val, base);
                break; 

            // Signed decimal format
            case 'd':
                va_val = __builtin_va_arg(args, int32);
                if ((long long) va_val < 0) {
                    print_char('-');
                    va_val = -(long long) va_val;
                }
                base = 10;
                print_va(va_val, base);
                break; 

            // Unsigned decimal format
            case 'u':
                va_val = __builtin_va_arg(args, int32);
                base = 10;
                print_va(va_val, base);
                break;                       
        }
        
        (uint8_t *)fmt++ ;
        character = *(uint8_t *)fmt ;
    }
    return 0 ;
}


void print_va(uint32_t va_val, uint32_t base){

    char char_to_print = va_val % base ;
    switch (char_to_print)
    {
    case 0:
        print_char('0') ;
        break;

    case 1:
        print_char('1') ;
        break;

    case 2:
        print_char('2') ;
        break;

    case 3:
        print_char('3') ;
        break;

    case 4:
        print_char('4') ;
        break;

    case 5:
        print_char('5') ;
        break;

    case 6:
        print_char('6') ;
        break;

    case 7:
        print_char('7') ;
        break;

    case 8:
        print_char('8') ;
        break;

    case 9:
        print_char('9') ;
        break;

    case 10:
        print_char('a') ;
        break;

    case 11:
        print_char('b') ;
        break;

    case 12:
        print_char('c') ;
        break;

    case 13:
        print_char('d') ;
        break;

    case 14:
        print_char('e') ;
        break;

    case 15:
        print_char('f') ;
        break;

    default:
        break;
    }
}
