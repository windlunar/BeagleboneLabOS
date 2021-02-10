
#include "syscall.h"
#include "../kernel/printk.h"
#include "debug.h"
#include "task.h"
#include "../klib/mem.h"
#include "memory.h"
#include "file.h"
#include "mmu.h"
#include "../driver/timer.h"


void (*syscall[MAX_SYSCALL_ID])(void *usrTaskContextOld ,void *args)  = {
	print_hello_handler
	, yield_handler
	, get_tid_handler
	, exit_handler
	, fork_handler
	, do_taskCreate_handler
	, malloc_blk_handler
	, malloc_mfree_blk_handler
	, get_mblk_list_handler
	, get_task_priority_handler
	, write_handler
	, read_handler
	, open_handler
	, getcwd_handler
	, getsubdir_handler
	, getfdir_handler
	, chdir_handler
	, getfullpath_handler
	, restart_handler
	, close_handler
} ;



void syscall_handler(uint32_t syscall_id ,void *usrTaskContextOld ,void *args)
{
	switch_mm((uint32_t *)L1_PAGE_TABLE_BASE_PADDR) ;   /** switch to kernel pgt */
	syscall[syscall_id](usrTaskContextOld ,args) ;
	switch_mm(curr_running_task->pgtbase) ; /** switch back to task pgt */
}



void  print_hello_handler(void *usrTaskContextOld ,void *args)
{
	int input = (int)args ;
	printk("Hello! This is my first system call,Input value =%d\r\n" ,input) ;
}



void yield_handler(void *usrTaskContextOld ,void *args)
{
	/* Save old context */
	curr_running_task->task_context = (struct TASK_CONTEXT *)usrTaskContextOld ;

	/* Change the task status to ready */
	curr_running_task->task_status = TASK_READY ;
	curr_running_task = NULL ;

	set_sched_context();
	call_sched((uint32_t)schedFuncContextSPtr) ;
}



void get_tid_handler(void *usrTaskContextOld ,void *args)
{
	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = curr_running_task->task_id ;
}



/** 回收資源 ,將task設定為 terminate */
void exit_handler(void *usrTaskContextOld ,void *args)
{
	printk("task id=%d exit\r\n" ,curr_running_task->task_id) ;
	curr_running_task->task_status = TASK_TERMINATE ;

	struct PAGE_INFO *curr_ma = which_page(curr_running_task->stk_bottom) ;

	/* 從queue中移除 */
	task_pop(curr_running_task) ;

	/* 歸還空間 */
	page_free(curr_ma) ;

	/* 歸還 pgt 空間 */
	free_pgt(curr_running_task->pgtbase) ;
}



/** 
 * 
 * 
 * Alloc 一個 Memory Page,並回傳描述該 Page 的結構體 struct PAGE_INFO
 * 並將該 area使用狀況設為 FULL ,只屬於這個task使用
 * 
 * 將描述 task的TASK_INFO 結構體, 放置在該 page 的起始位址
 * 
 * 複製stack空間內所有內容 ,跟 struct TASK_INFO 結構體 
 */
void fork_handler(void *usrTaskContextOld ,void *args)
{    

	struct PAGE_INFO *n_pg = page_alloc();
	n_pg->page_status = FOR_TASK ;
	_memset((void *)n_pg->pgstart, 0, TASK_STACK_SIZE) ;

	struct TASK_INFO *ntask = (struct TASK_INFO *)(n_pg->pgstart) ;
	struct PAGE_INFO *curr_pg = which_page(curr_running_task->stk_bottom) ;

	_memcpy((void *)(n_pg->pgstart)
			,(void *)(curr_pg->pgstart)
			,TASK_STACK_SIZE) ;

	_memcpy((void *)ntask 
			,(void *)curr_running_task 
			,sizeof(struct TASK_INFO));


	/** Stack pointer要指向stack中相同的相對位址上 */
	ntask->stk_bottom = stktop2bottom(n_pg->task_stk_top) ;
	ntask->stk_top = n_pg->task_stk_top ;
	ntask->task_context = (struct TASK_CONTEXT *)(ntask->stk_top - (curr_running_task->stk_top - (uint32_t *)usrTaskContextOld) ) ;
	

	/** 設定TASK_INFO_t結構體的其他內容 */
	ntask->next_ptr = NULL ;
	ntask->prev_ptr = NULL ;
	ntask->task_status = TASK_READY ;
	ntask->taskCallBack = curr_running_task->taskCallBack ;
	ntask->priority = curr_running_task->priority ;

	taskid++ ;
	ntask->task_id = taskid;

	blks_init(n_pg) ;
	task_enqueue(ntask) ;

	/** 設定回傳值 */
	ntask->task_context->r0 = 0 ;
	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = ntask->task_id ;

	ntask->pgtbase = (uint32_t *)task_pgt_setup(n_pg->pgstart ,n_pg->top) ;
	set_pte(curr_pg->pgstart ,curr_pg->top ,ntask->pgtbase) ;
}



void do_taskCreate_handler(void *usrTaskContextOld ,void *arg)
{
	struct TASK_ARGS *config = (struct TASK_ARGS *)arg ;

	struct PAGE_INFO *pg = page_alloc();
	pg->page_status = FOR_TASK ; 
	_memset((void *)pg->pgstart, 0, TASK_STACK_SIZE) ;

	/** 把 TASK_INFO 結構放在該 memo區域的起始位址 */
	struct TASK_INFO *ntask = (struct TASK_INFO *)(pg->pgstart) ;

	taskCreate(ntask ,config->taskCallBack ,stktop2bottom(pg->task_stk_top) ,config->prio);

	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = ntask->task_id ;

	blks_init(pg) ;
	open_console_in_out(ntask) ;

	/** 設定路徑 */
	ntask->cwdn = curr_running_task->cwdn ;

	task_enqueue(ntask) ; 

	ntask->pgtbase = (uint32_t *)task_pgt_setup(pg->pgstart ,pg->top) ;
}



void malloc_blk_handler(void *usrTaskContextOld ,void *args)
{
	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;

	struct PAGE_INFO *curr_pg = which_page(curr_running_task->stk_bottom) ;

	old_context->r0 = (uint32_t)blk_alloc(curr_pg) ;
}



void malloc_mfree_blk_handler(void *usrTaskContextOld ,void *args)
{
	free_blk(args) ;
}



void get_mblk_list_handler(void *usrTaskContextOld ,void *args)
{
	struct PAGE_INFO *curr_pg = which_page(curr_running_task->stk_bottom) ;
	struct BLK_INFO *head = curr_pg->blk_list_head ;

	while (head->next != NULL) {
		printk("blk addr =%p ,start =%x\r\n",head ,head->start) ;
		head = head->next ;
	}
	printk("blk addr =%p ,start =%x\r\n",head ,head->start) ;    
}



void get_task_priority_handler(void *usrTaskContextOld ,void *args)
{
	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = curr_running_task->priority ;    
}



/** 之後再implement讓該task staus轉為block ,等寫完在ready */
void write_handler(void *usrTaskContextOld ,void *args)
{
	struct FILE_RDWR_ARGS *write_args = (struct FILE_RDWR_ARGS *)args ;
	int fd = write_args->fd ;
	uint8_t *wrbuf = write_args->buf ;
	uint32_t n_bytes = write_args->n_bytes ;

	int ret = curr_running_task->openfiles[fd]->file_write(wrbuf ,n_bytes) ;

	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = ret ;  
}



void read_handler(void *usrTaskContextOld ,void *args)
{
	struct FILE_RDWR_ARGS *write_args = (struct FILE_RDWR_ARGS *)args ;
	int fd = write_args->fd ;
	uint8_t *rdbuf = write_args->buf ;
	uint32_t n_bytes = write_args->n_bytes ;

	int ret = curr_running_task->openfiles[fd]->file_read(rdbuf ,n_bytes) ; 

	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = ret ;    
}



void open_handler(void *usrTaskContextOld ,void *args)
{
	char *path = (char *)args ;
	int fd = -1 ;

	fd = file_open(path ,(void *)curr_running_task) ;
	if (fd < 0) printk("Error :Can't open file.\r\n") ;

	/** Setting return value */
	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	old_context->r0 = fd ;  
}



void getcwd_handler(void *usrTaskContextOld ,void *args)
{
	struct BUF_AND_SZ_ARG *getcwdarg = (struct BUF_AND_SZ_ARG *)args ;

	if ( _strlen(curr_running_task->cwdn->namebuf) > getcwdarg->n_size) getcwdarg->buf = NULL ;

	_strcat(getcwdarg->buf ,curr_running_task->cwdn->name) ;
}



void getsubdir_handler(void *usrTaskContextOld ,void *args)
{
	struct BUF_AND_SZ_ARG *bufsz = (struct BUF_AND_SZ_ARG *)args ;

	char *dir_ens_buf = bufsz->buf ;
	int sz_buf = bufsz->n_size ;

	_memset(dir_ens_buf ,0 ,sz_buf) ;

	struct DIR_NODE *curr_dir_n = curr_running_task->cwdn ;
	struct DIR_NODE *en = curr_dir_n->firstchild ;

	char *delim =";;\0" ;

	/** 串dir下的 entry */
	while (en != NULL) {
		if ((sz_buf - _strlen(dir_ens_buf)) < _strlen(en->name)+sizeof(delim)) {
			dir_ens_buf = NULL ;
			break ;
		}

		_strcat(dir_ens_buf ,en->name) ;
		_strcat(dir_ens_buf ,delim) ;
		en = en->next_sibling ;
	}
}



void getfdir_handler(void *usrTaskContextOld ,void *args)
{
	struct BUF_AND_SZ_ARG *bufsz = (struct BUF_AND_SZ_ARG *)args ;

	char *dir_fs_buf = bufsz->buf ;
	int sz_buf = bufsz->n_size ;

	_memset(dir_fs_buf ,0 ,sz_buf) ;

	struct DIR_NODE *curr_dir_n = curr_running_task->cwdn ;
	struct FILE *f = curr_dir_n->firstfile ;

	char *delim =";;\0" ;

	/** 串dir下 files */
	while (f != NULL) {
		if ((sz_buf - _strlen(dir_fs_buf)) < _strlen(f->name)+sizeof(delim)) {
			dir_fs_buf = NULL ;
			break ;
		}

		_strcat(dir_fs_buf ,f->name) ;
		_strcat(dir_fs_buf ,delim) ;
		f = f->next_sibling ;
	}  
}



/** 切換至下一層路徑 :路徑名稱 ,上一層路徑 : ".." */
void chdir_handler(void *usrTaskContextOld ,void *args)
{
	char *subdir_name = (char *)args ;

	struct DIR_NODE *curdir = curr_running_task->cwdn ;
	struct DIR_NODE *targetdir = find_target_subdir(curdir ,subdir_name) ;

	/** Setting return value */
	struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
	if (targetdir != NULL) {
		curr_running_task->cwdn = targetdir ;   /** changer current dir */
		old_context->r0 = 0 ;
	} else {
		/** Keep the current dir */
		old_context->r0 = -1 ;
	}
}


void getfullpath_handler(void *usrTaskContextOld ,void *args)
{
	struct BUF_AND_SZ_ARG *arg = (struct BUF_AND_SZ_ARG *)args ;
	char *buf = arg->buf ;
	int n_size = arg->n_size ;

	struct DIR_NODE *curdir = curr_running_task->cwdn ;
	struct DIR_NODE *dirwk[16] ;   /** 最多16層 dir */

	for (int i=0 ; i<16; i++) {
		dirwk[i] = NULL ;
	}

	int j = 0 ;
	while (curdir != NULL) {
		dirwk[j] = curdir ;
		curdir = curdir->parent ;
		j++ ;
	}
	int level_dir = j ;
	j-- ;

	while(j >= 0) {
		if( (n_size - _strlen(buf)) < _strlen(dirwk[j]->name)) printk("Buf not enough\r\n");
		_strcat(buf ,dirwk[j]->name) ;
		j-- ;
	}
}



void restart_handler(void *usrTaskContextOld ,void *args)
{
	set_wdt_count(WATCHDOG_BASE, 0xfffffff0) ;
	enable_watchdog(WATCHDOG_BASE) ;
}



void close_handler(void *usrTaskContextOld ,void *args)
{
	int fd = (int)args ;
	file_close(fd ,(void *)curr_running_task) ;
}