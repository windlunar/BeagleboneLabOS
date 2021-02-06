
#include "commands.h"
#include "usrtasks.h"
#include "../klib/std_io.h"
#include "../klib/queue.h"
#include "../klib/usyscall.h"

void lsdir()
{
	char buf[16*10] ;
	_memset(buf ,0 ,sizeof(buf)) ;
	if(__getsubdir(buf ,sizeof(buf)) < 0)
	{
		printk("buf size not enough.\r\n") ;
	}

	char *delim = ";;\0" ;
	char token[16] ;
	char *start = buf ;

	while(start != NULL)
	{
		_memset(token ,0 ,sizeof(token)) ;
		start = strtok_fst(start ,delim ,_strlen(delim) ,token) ;

		if(*token != '\0') put_str("\r\n\0") ;
		put_str(token) ;
	}

	//
	_memset(buf ,0 ,sizeof(buf)) ;
	if(__getfdir(buf ,sizeof(buf)) < 0)
	{
		printk("buf size not enough.\r\n") ;
	}

	start = buf ;

	while(start != NULL)
	{
		_memset(token ,0 ,sizeof(token)) ;
		start = strtok_fst(start ,delim ,_strlen(delim) ,token) ;

		if(*token != '\0') put_str("\r\n\0") ;
		put_str(token) ;		
	}
}


int cd(char *subdir)
{
    return __chdir(subdir) ;
}



int pwd(char *cwd ,int cwdsz)
{
	__getfullpath(cwd ,cwdsz) ;

	return 0 ;
}
