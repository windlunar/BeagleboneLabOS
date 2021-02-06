
#include "kprint.h"
#include "../driver/uart.h"

//__builtin_va_list is built in to the compiler ,not a macro
//
// __builtin_va_start(ap, last)
// __builtin_va_arg(ap, type)
// __builtin_va_end(ap)
// 
// ap point to the first var of variable-arguments(可變參數)
// last point to the previous argument
// ex printf(const char * ,...)
// ap point to const char * ,last point to ...
//

void terminal_printC(int32_t character)
{
    //Print by the UART ,UART0 is terminal output
    uart_putC(UART0_PTR ,character) ;
}


void print_char(int32_t character)
{
    terminal_printC(character) ;
}


int32_t printk(const char *fmt, ...)
{
    /** __builtin_va_start初始化args, fmt指向輸入字串*/
    __builtin_va_list args ;
    __builtin_va_start(args ,fmt) ;

    int32_t i = kprint(fmt ,args) ;

    /** 結束獲取可變參數*/
    __builtin_va_end(args) ;

    return i ;
}



int kprint(const char *fmt ,__builtin_va_list args){
    register int character ;
    uint32_t va_val ,base ;
    char padc ;
    int width ;


    while(1){
        //printf("字串",) : 當字串不為%時, 將字串依序印出
        //先判斷是否不等於, 然後才+1跳入while執行, 或是不執行
        //當遇到%時,不執行該迴圈,and then add the pointer fmt to move to the next char after % (like c,s,d,u,p...)
        // and then 往下執行label FORMAT_PRINT
        while((character = *(uint8_t  *)fmt++) != '%'){
            //如果遇到結束字元則跳出
            if(character == '\0'){
                return 0 ;
            }
            //印出字元
            print_char(character) ;
        }
		padc = ' ';
		width = -1;

        //An label
        FORMAT_PRINT:
            //判斷the next char after % (like c,s,d,u,p...), 以格式化輸出(like c,s,d,u,p...)
            //然後pointer fmt才+1 , move to the next char after (c,s,d,u,p...), 以繼續印出後面的字元
            switch(character = *(uint8_t  *)fmt++){
                case '-':
                    padc = '-';
                    goto FORMAT_PRINT;

                case '0':
                    padc = '0';
                    goto FORMAT_PRINT;

                //Character format
                case 'c':
                    print_char(__builtin_va_arg(args, int));
                    break;   

                //Pointer format
                case 'p':
                    print_char('0');
                    print_char('x');
                    //__builtin_va_arg 返回可變參數的值
                    va_val = (uint32) __builtin_va_arg(args, void *);
                    //格式為16進位
                    base = 16;
                    goto PRINT_VA;

                //Hex format
                case 'x':
                    print_char('0');
                    print_char('x');
                    va_val = __builtin_va_arg(args, uint32);
                    base = 16;
                    goto PRINT_VA;

                // Signed decimal format
                case 'd':
                    va_val = __builtin_va_arg(args, int32);
                    if ((long long) va_val < 0) {
                        print_char('-');
                        va_val = -(long long) va_val;
                    }
                    base = 10;
                    goto PRINT_VA;

                // Unsigned decimal format
                case 'u':
                    va_val = __builtin_va_arg(args, int32);
                    base = 10;
                    goto PRINT_VA;

                PRINT_VA:
			        print_va(va_val, base, width, padc);
			        break;                           
            }
    }
    return 0 ;
}


static void print_va(uint32_t va_val, uint32_t base, int32_t width, int32_t padc){
	if (va_val >= base) {
		print_va(va_val / base, base, width - 1, padc);
	} else {
		while (--width > 0)
			print_char(padc);
	}

	print_char("0123456789abcdef"[va_val % base]);
}
