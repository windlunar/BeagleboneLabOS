
#include "../klib/stdio.h"
#include "../klib/usyscall.h"


char get_c()
{
    char byte = '\0' ;
    __read(FD_CONSOLE_IN ,&byte ,1) ; 

    return byte ;
}


void put_c(char character)
{
    char byte = (char)character ;
    __write(FD_CONSOLE_OUT ,&byte ,1) ; 
}


void put_str(char *s)
{
    while (*s != '\0') {
        put_c(*s) ;
        s++ ;
    }
}


int32_t uprintf(const char *fmt, ...)
{
    __builtin_va_list args ;
    __builtin_va_start(args ,fmt) ;

    int32_t i = uprint(fmt ,args) ;

    /** 結束獲取可變參數*/
    __builtin_va_end(args) ;

    return i ;
}


int uprint(const char *fmt ,__builtin_va_list args)
{
    register int character ;
    uint32_t va_val ,base ;
    char padc ;
    int width ;


    while (1) {
        /** 
         * 如果遇到結束字元則跳出
         * *fmt++ : pointer fmt+1 ,但取值為s移動前指向的值
         */
        while ((character = *(uint8_t  *)fmt++) != '%') {
            if (character == '\0') {
                return 0 ;
            }
            /** 印出字元*/
            put_c(character) ;
        }
		padc = ' ';
		width = -1;

        /** An label */
        FORMAT_PRINT:
            /** 判斷the next char after % (like c,s,d,u,p...), 以格式化輸出(like c,s,d,u,p...)
             * 然後pointer fmt才+1 , move to the next char after (c,s,d,u,p...), 以繼續印出
             * 後面的字元
             */
            switch (character = *(uint8_t  *)fmt++) {
                case '-':
                    padc = '-';
                    goto FORMAT_PRINT;

                case '0':
                    padc = '0';
                    goto FORMAT_PRINT;

                /* Character format */
                case 'c':
                    put_c(__builtin_va_arg(args, int));
                    break;   

                /* Pointer format */
                case 'p':
                    put_c('0');
                    put_c('x');

                    /* __builtin_va_arg 返回可變參數的值 */
                    va_val = (uint32) __builtin_va_arg(args, void *);

                    base = 16; /* 格式為16進位 */
                    goto PUT_VA;

                /* Hex format */
                case 'x':
                    put_c('0');
                    put_c('x');
                    va_val = __builtin_va_arg(args, uint32);
                    base = 16;
                    goto PUT_VA;

                /* Signed decimal format */
                case 'd':
                    va_val = __builtin_va_arg(args, int32);
                    if ((long long) va_val < 0) {
                        put_c('-');
                        va_val = -(long long) va_val;
                    }
                    base = 10;
                    goto PUT_VA;

                /* Unsigned decimal format */
                case 'u':
                    va_val = __builtin_va_arg(args, int32);
                    base = 10;
                    goto PUT_VA;

                PUT_VA:
			        put_va(va_val, base, width, padc);
			        break;                           
            }
    }
    return 0 ;
}



static void put_va(uint32_t va_val, uint32_t base, int32_t width, int32_t padc)
{
	if (va_val >= base) {
		put_va(va_val / base, base, width - 1, padc);
	} else {
		while (--width > 0)
			put_c(padc);
	}

	put_c("0123456789abcdef"[va_val % base]);
}
