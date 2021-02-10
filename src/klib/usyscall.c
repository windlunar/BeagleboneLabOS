
#include "../klib/usyscall.h"
#include "../klib/stdio.h"


void __print_hello(uint32_t *input)
{
	syscall_print_hello(input) ;
}



void __yield(void)
{
	syscall_yield();
}



int __gettid(void)
{
	return syscall_get_tid();
}



void __exit(void)
{
	syscall_exit();
	for(;;) ;
}



int __fork(void)
{
	return syscall_fork();
}



int __do_taskCreate(void (*taskFunc)() ,int32_t prio)
{
	struct TASK_ARGS config ;
	config.taskCallBack = taskFunc ;
	config.prio = prio ;
	return syscall_do_taskCreate(&config) ;
}



/** Allocate one block(4K bytes) */
void *__malloc_blk()
{
	return syscall_malloc_blk() ;
}



void __mfree_blk(void *blk_aval_start)
{
	syscall_mfree_blk(blk_aval_start) ;
}



void __get_mblk_list()
{
	syscall_get_mblk_list() ;
}



int __get_task_priority()
{
	return syscall_get_task_priority() ;
}



int __write(int fd ,char *buf ,uint32_t n_bytes)
{
	struct FILE_RDWR_ARGS args ;
	args.fd = fd ;
	args.buf =buf ;
	args.n_bytes = n_bytes ;

	return syscall_write(&args) ;    
}



int __read(int fd ,char *buf ,uint32_t n_bytes)
{
	struct FILE_RDWR_ARGS args ;
	args.fd = fd ;
	args.buf =buf ;
	args.n_bytes = n_bytes ;

	return syscall_read(&args) ;    
}



int __open(char *path)
{
	return syscall_open(path) ;
}



void __getcwd(char *buf ,int n_bytes)
{
	_memset(buf ,0 ,n_bytes) ;

	struct BUF_AND_SZ_ARG args ;
	args.buf = buf ;
	args.n_size = n_bytes ;

	syscall_getcwd(&args) ;
}



int __getsubdir(char *buf ,int n_bytes)
{
	_memset(buf ,0 ,n_bytes) ;

	struct BUF_AND_SZ_ARG args ;
	args.buf = buf ;
	args.n_size = n_bytes ;

	syscall_getsubdir(&args) ;    

	if(args.buf == NULL) return -1 ;
	return 0 ;
}



int __getfdir(char *buf ,int n_bytes)
{
	_memset(buf ,0 ,n_bytes) ;

	struct BUF_AND_SZ_ARG args ;
	args.buf = buf ;
	args.n_size = n_bytes ;

	syscall_getfdir(&args) ;    

	if(args.buf == NULL) return -1 ;
	return 0 ;
}



// return 0  : success
// return -1 : No dir found
int __chdir(char *subdirname)
{
	return syscall_chdir(subdirname) ;
}



void __getfullpath(char *buf ,int n_bytes)
{
	struct BUF_AND_SZ_ARG args ;
	args.buf = buf ;
	args.n_size = n_bytes ;
	
	syscall_getfullpath(&args) ;
}



void __restart(void)
{
	syscall_restart() ;
}



void __close(int fd)
{
	syscall_close(fd) ;
}