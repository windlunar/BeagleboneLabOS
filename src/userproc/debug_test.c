
#include "usrtasks.h"
#include "debug_test.h"
#include "../klib/stdio.h"
#include "../klib/queue.h"
#include "../klib/usyscall.h"
#include "commands.h"


void test_sys_getsubdir_and_getfdir()
{
	char buf[16*5] ;
	_memset(buf ,0 ,sizeof(buf)) ;
	if (__getsubdir(buf ,sizeof(buf)) < 0)
		printk("buf size not enough.\r\n") ;

	put_str(buf) ;
	put_str("\r\n") ;
	printk("Split subdir list string.") ;

	char *delim = ";;\0" ;
	char token[16] ;
	char *start = buf ;

	while (start != NULL) {
		_memset(token ,0 ,sizeof(token)) ;
		start = strtok_fst(start ,delim ,_strlen(delim) ,token) ;

		put_str("\r\n\0") ;
		put_str(token) ;
	}
	printk("__getsubdir Test end\r\n\r\n") ;

	_memset(buf ,0 ,sizeof(buf)) ;
	if (__getfdir(buf ,sizeof(buf)) < 0)
		printk("buf size not enough.\r\n") ;

	put_str(buf) ;
	put_str("\r\n") ;
	printk("Split file list string.") ;

	start = buf ;

	while (start != NULL) {
		_memset(token ,0 ,sizeof(token)) ;
		start = strtok_fst(start ,delim ,_strlen(delim) ,token) ;

		put_str("\r\n\0") ;
		put_str(token) ;		
	}
	printk("__getfdir Test end\r\n") ;
}


void test_sys_chdir()
{
	put_str("\r\nList subdirs and files\r\n") ;
	lsdir() ;

	put_str("\r\n\r\nChange dir test.") ;
	if (__chdir("/dev\0") < 0) put_str("\r\nDir not found\r\n") ;
	lsdir();

	put_str("\r\n\r\nChange dir test.") ;
	if (__chdir("..\0") < 0) put_str("\r\nDir not found\r\n") ;
	lsdir();

	put_str("\r\n\r\nChange dir test.") ;
	if (__chdir("..\0") < 0) put_str("\r\nDir not found\r\n") ;
	put_str("\r\nList subdirs and files") ;
	lsdir();
}