

#include "../common.h"


// 字串長度(不包含 \0)
// 如 "1236\0" 則回傳 len=4
int32_t strlen(char *s)
{
	int32_t len=0 ;
	while(*s != 0)
	{
		s++ ;
		len++ ;
	}
	return len ;
}


int strcmp(char *s ,char *b)
{
    while(*s == *b)
    {
        if((*s==0) && (*b!=0)){
            break ;
        }else if((*s!=0) && (*b==0)){
            
        }else if((*s==0) && (*b==0)){
            return 0 ;
        }
        s++ ;
        b++ ;
    } 

    // not eq
    return 1 ;
}