

#include "syscall.h"
#include "../kernel/kprint.h"
#include "debug.h"
#include "task.h"
#include "../klib/mem.h"
#include "memory.h"
#include "file.h"
#include "mmu.h"
#include "../driver/timer.h"


void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args) ;
void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args)
{
    switch (syscall_id) {
    case SYSCALL_ID_print_hello:
        __print_hello_handler(*(uint32_t *)args) ;
        break;

    case SYSCALL_ID_yield:
        __yield_handler(usrTaskContextOld) ;
        break;   

    case SYSCALL_ID_get_tid:
        __get_tid_handler(usrTaskContextOld) ;
        break;  

    case SYSCALL_ID_exit:
        __exit_handler(usrTaskContextOld) ;
        break;  

    case SYSCALL_ID_fork:
        __fork_handler(usrTaskContextOld) ;
        break;  

    case SYSCALL_ID_do_taskCreate:
        __do_taskCreate_handler(usrTaskContextOld ,args) ;
        break;  

    case SYSCALL_ID_malloc_blk:
        __malloc_blk_handler(usrTaskContextOld) ;
        break;  

    case SYSCALL_ID_mfree_blk:
        __malloc_mfree_blk_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_get_mblk_list:
        __get_mblk_list_handler(usrTaskContextOld) ;
        break;

    case SYSCALL_ID_get_task_priority:
        __get_task_priority_handler(usrTaskContextOld) ;
        break;

    case SYSCALL_ID_write:
        __write_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_read:
        __read_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_open:
        __open_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_getcwd:
        __getcwd_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_getsubdir:
        __getsubdir_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_getfdir:
        __getfdir_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_chdir:
        __chdir_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_getfullpath:
        __getfullpath_handler(usrTaskContextOld ,args) ;
        break;

    case SYSCALL_ID_restart:
        __restart_handler() ;
        break;

    case SYSCALL_ID_close:
        __close_handler(usrTaskContextOld ,args) ;
        break;

    default:
        break;
    }
}


void  __print_hello_handler(uint32_t input)
{
    printk("Hello! This is my first system call,Input value =%d\r\n" ,input) ;
}


void __yield_handler(uint32_t *usrTaskContextOld)
{
	// Save old context
	curr_running_task->task_context = (struct TASK_CONTEXT *)usrTaskContextOld ;

	// Change the task status to ready
	curr_running_task->task_status = TASK_READY ;
	curr_running_task = NULL ;

	//prepare sched() context
	schedFuncContextPrepare();
	_call_sched((uint32_t)schedFuncContextSPtr) ;
}


void __get_tid_handler(uint32_t *usrTaskContextOld)
{
    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = curr_running_task->task_id ;
}


// 回收資源
// 將task設定為 terminate
void __exit_handler(uint32_t *usrTaskContextOld)
{
    printk("task id=%d exit\r\n" ,curr_running_task->task_id) ;
    curr_running_task->task_status = TASK_TERMINATE ;

    struct PAGE_INFO *curr_ma = which_page(curr_running_task->stk_bottom) ;

    //從queue中移除
    task_pop(curr_running_task) ;

    //釋放空間
    page_free(curr_ma) ;

    //回收 pgt
    free_pgt(curr_running_task->pgtbase) ;
  
    //curr_running_task->stk_bottom = NULL ;
    curr_running_task = choose_task() ;

/************************************************************************/
// Test
/************************************************************************/
	switch_mm(curr_running_task->pgtbase) ;
/************************************************************************/

    set_context_sp((uint32_t *)curr_running_task->task_context) ;
}



// 複製一份相同的stack ,跟 struct TASK_INFO 結構體
void __fork_handler(uint32_t *usrTaskContextOld)
{    
    // Alloc 一個 Memory Page,並回傳描述該area的結構體 struct PAGE_INFO
    // 並將該 area使用狀況設為 FULL ,只屬於這個task使用
    struct PAGE_INFO *n_pg = page_alloc();
    n_pg->page_status = FOR_TASK ;
    _memset((void *)n_pg->pgstart, 0, TASK_STACK_SIZE) ;


    // 分配 描述 task的TASK_INFO 結構體, 其起始位置設定為 stack bottom
    struct TASK_INFO *ntask = (struct TASK_INFO *)(n_pg->pgstart) ;


    // 找目前正在執行的task(父task)的stack空間屬於那一個 memory page
    struct PAGE_INFO *curr_pg = which_page(curr_running_task->stk_bottom) ;


    // 複製父task使用的記憶體區段內的data給子task的記憶體區段, 包含stack空間內所有內容
    _memcpy((void *)(n_pg->pgstart)
            ,(void *)(curr_pg->pgstart)
            ,TASK_STACK_SIZE) ;


    // 複製描述 task的TASK_INFO_t結構體
    _memcpy((void *)ntask 
            ,(void *)curr_running_task 
            ,sizeof(struct TASK_INFO));


    // Stack pointer要指向stack中相同的相對位址上
    // (curr_running_task->stk_top - usrTaskContextOld)
    //  = old context相對於stack top 的offset
    ntask->stk_bottom = stktop2bottom(n_pg->task_stk_top) ;
    ntask->stk_top = n_pg->task_stk_top ;
    ntask->task_context = (struct TASK_CONTEXT *)(ntask->stk_top - (curr_running_task->stk_top - usrTaskContextOld) ) ;
    

    // 設定TASK_INFO_t結構體的其他內容
    ntask->next_ptr = NULL ;
    ntask->prev_ptr = NULL ;
    ntask->task_status = TASK_READY ;
    ntask->taskCallBack = curr_running_task->taskCallBack ;
    ntask->priority = curr_running_task->priority ;

    // 設定子stack的 task id
	taskid++ ;
	ntask->task_id = taskid;

    blks_init(n_pg) ;

    // 將子task放入 ready list中
    task_enqueue(ntask) ;


    //設定回傳值
    ntask->task_context->r0 = 0 ;
    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = ntask->task_id ;

/************************************************************************/
// Test
/************************************************************************/
	ntask->pgtbase = (uint32_t *)task_pgt_setup(n_pg->pgstart ,n_pg->top) ;
    set_pte(curr_pg->pgstart ,curr_pg->top ,ntask->pgtbase) ;
/************************************************************************/

}


/************************************************************************************************/
void __do_taskCreate_handler(uint32_t *usrTaskContextOld ,void *arg)
{
    struct TASK_ARGS *config = (struct TASK_ARGS *)arg ;

    struct PAGE_INFO *pg = page_alloc();
    pg->page_status = FOR_TASK ; 
    _memset((void *)pg->pgstart, 0, TASK_STACK_SIZE) ;

    // 把 TASK_INFO 結構放在該 memo區域的起始位址
    struct TASK_INFO *ntask = (struct TASK_INFO *)(pg->pgstart) ;

    taskCreate(ntask ,config->taskCallBack ,stktop2bottom(pg->task_stk_top) ,config->prio);


    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = ntask->task_id ;

    // init blocks
    blks_init(pg) ;

    open_console_in_out(ntask) ;

    //設定路徑
    ntask->cwdn = curr_running_task->cwdn ;

    task_enqueue(ntask) ; 

/************************************************************************/
// Test
/************************************************************************/
	ntask->pgtbase = (uint32_t *)task_pgt_setup(pg->pgstart ,pg->top) ;
/************************************************************************/
}



void __malloc_blk_handler(uint32_t *usrTaskContextOld)
{
    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;

    struct PAGE_INFO *curr_pg = which_page(curr_running_task->stk_bottom) ;

    old_context->r0 = (uint32_t)blk_alloc(curr_pg) ;
}


void __malloc_mfree_blk_handler(uint32_t *usrTaskContextOld ,void *blk_aval_start)
{
    free_blk(blk_aval_start) ;
}



void __get_mblk_list_handler(uint32_t *usrTaskContextOld)
{
    struct PAGE_INFO *curr_pg = which_page(curr_running_task->stk_bottom) ;
    struct BLK_INFO *head = curr_pg->blk_list_head ;

    while (head->next != NULL) {
        printk("blk addr =%p ,start =%x\r\n",head ,head->start) ;
        head = head->next ;
    }
    printk("blk addr =%p ,start =%x\r\n",head ,head->start) ;    
}



void __get_task_priority_handler(uint32_t *usrTaskContextOld)
{
    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = curr_running_task->priority ;    
}


// 之後再implement讓該task staus轉為block ,等寫完在ready
void __write_handler(uint32_t *usrTaskContextOld ,void *args)
{
    struct FILE_RDWR_ARGS *write_args = (struct FILE_RDWR_ARGS *)args ;
    int fd = write_args->fd ;
    uint8_t *wrbuf = write_args->buf ;
    uint32_t n_bytes = write_args->n_bytes ;

    int ret = curr_running_task->openfiles[fd]->file_write(wrbuf ,n_bytes) ;

    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = ret ;  
}



void __read_handler(uint32_t *usrTaskContextOld ,void *args)
{
    struct FILE_RDWR_ARGS *write_args = (struct FILE_RDWR_ARGS *)args ;
    int fd = write_args->fd ;
    uint8_t *rdbuf = write_args->buf ;
    uint32_t n_bytes = write_args->n_bytes ;

    int ret = curr_running_task->openfiles[fd]->file_read(rdbuf ,n_bytes) ; 

    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = ret ;    
}



void __open_handler(uint32_t *usrTaskContextOld ,void *args)
{
    char *path = (char *)args ;
    int fd = -1 ;

    fd = file_open(path ,(void *)curr_running_task) ;
    if (fd < 0) printk("Error :Can't open file.\r\n") ;

    // Setting return value
    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    old_context->r0 = fd ;  
}



void __getcwd_handler(uint32_t *usrTaskContextOld ,void *args)
{
    struct BUF_AND_SZ_ARG *getcwdarg = (struct BUF_AND_SZ_ARG *)args ;

    if ( _strlen(curr_running_task->cwdn->namebuf) > getcwdarg->n_size) getcwdarg->buf = NULL ;

    _strcat(getcwdarg->buf ,curr_running_task->cwdn->name) ;
}



void __getsubdir_handler(uint32_t *usrTaskContextOld ,void *args)
{
    struct BUF_AND_SZ_ARG *bufsz = (struct BUF_AND_SZ_ARG *)args ;

    char *dir_ens_buf = bufsz->buf ;
    int sz_buf = bufsz->n_size ;

    _memset(dir_ens_buf ,0 ,sz_buf) ;

    struct DIR_NODE *curr_dir_n = curr_running_task->cwdn ;
    struct DIR_NODE *en = curr_dir_n->firstchild ;

    char *delim =";;\0" ;
    //串dir下的 entry
    while (en != NULL){
        if ((sz_buf - _strlen(dir_ens_buf)) < _strlen(en->name)+sizeof(delim)) {
            dir_ens_buf = NULL ;
            break ;
        }

        _strcat(dir_ens_buf ,en->name) ;
        _strcat(dir_ens_buf ,delim) ;
        en = en->next_sibling ;
    }
}



void __getfdir_handler(uint32_t *usrTaskContextOld ,void *args)
{
    struct BUF_AND_SZ_ARG *bufsz = (struct BUF_AND_SZ_ARG *)args ;

    char *dir_fs_buf = bufsz->buf ;
    int sz_buf = bufsz->n_size ;

    _memset(dir_fs_buf ,0 ,sz_buf) ;

    struct DIR_NODE *curr_dir_n = curr_running_task->cwdn ;
    struct FILE *f = curr_dir_n->firstfile ;

    char *delim =";;\0" ;
    //串dir下 files
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



// 切換至下一層路徑 :路徑名稱
// 上一層路徑 : ".."
void __chdir_handler(uint32_t *usrTaskContextOld ,void *args)
{
    char *subdir_name = (char *)args ;

    struct DIR_NODE *curdir = curr_running_task->cwdn ;
    struct DIR_NODE *targetdir = find_target_subdir(curdir ,subdir_name) ;

    // Setting return value
    struct TASK_CONTEXT *old_context = (struct TASK_CONTEXT *)usrTaskContextOld ;
    if (targetdir != NULL) {
        curr_running_task->cwdn = targetdir ;   // changer current dir
        old_context->r0 = 0 ;
    } else {
        // Keep the current dir.
        old_context->r0 = -1 ;
    }
}


void __getfullpath_handler(uint32_t *usrTaskContextOld ,void *args)
{
    struct BUF_AND_SZ_ARG *arg = (struct BUF_AND_SZ_ARG *)args ;
    char *buf = arg->buf ;
    int n_size = arg->n_size ;

    struct DIR_NODE *curdir = curr_running_task->cwdn ;
    struct DIR_NODE *dirwk[16] ;   //最多16層 dir

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



void __restart_handler(void)
{
    set_wdt_count(WATCHDOG_BASE, 0xfffffff0) ;
    enable_watchdog(WATCHDOG_BASE) ;
}



void __close_handler(uint32_t *usrTaskContextOld ,void *args)
{
    int fd = (int)args ;
    file_close(fd ,(void *)curr_running_task) ;
}