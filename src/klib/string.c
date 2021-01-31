

#include "../common.h"
#include "../klib/mem.h"
#include "../klib/std_io.h"
#include "../klib/string.h"


// 字串長度(不包含最後的 \0)
// 如 "abcd\0" 則回傳 len=4
int _strlen(char *s)
{
	int len=0 ;
	while(*s != 0)
	{
		s++ ;
		len++ ;
	}
	return len ;
}


int _strcmp(char *s ,char *b)
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


// dest須保留能夠串接src的空間
int _strcat(char *dest ,char *src)
{
    char *d = dest ;
    char *s = src ;

    // 先把pointer移動到 dest的尾部 '\0'
    while(*d != '\0')
    {
        d++ ;
    }
    // 現在 d指向 dest的尾 '\0'

    while(*s != '\0')
    {
        *d = *s ;
        d++ ;
        s++ ;
    }
    *d = '\0' ;

    return 0 ;

}


// 還未經測試
void _strcpy(char *dest ,char *src)
{
    char *s = src ;
    char *d = dest ;
    while(*s != '\0')
    {
        *d = *s ;
        d++ ;
        s++ ;
    }
    *d = '\0' ;
}


void _strncpy(char *dest ,char *src ,int count)
{
    char *s =src ;
    char *d =dest ;
    while(count != 0)
    {
        *d = *s ;
        d++ ;
        s++ ;
        count -- ;
    }
    *d = '\0' ;
}

/**
 * arg1 :待分割字串的起始位址
 * arg2 :分割符號
 * arg3 :從 start開始獲得分割符號前的一個字串
 * return :分割符號後剩餘字串的起始位址
 */ 
char *strtok_fst(char *start ,char *delim ,int delim_sz ,char *token)
{
    char *s = start ;
    char *d = delim ;

    char sbuf[delim_sz+1] ;
    _memset(sbuf ,0 ,sizeof(sbuf)) ;

    // sbuf = delim + '\0'
    _strncpy(sbuf ,s ,delim_sz) ;

    while(_strcmp(sbuf ,delim)){    //不相等就執行 while迴圈
        s++ ;
        if(*s == '\0') break ;
        _strncpy(sbuf ,s ,delim_sz) ;
    }

    _strncpy(token ,start ,s-start) ;

    if(*s != '\0'){
        s += delim_sz ;
        return s ;
    }else{
        return NULL ;
    }

}