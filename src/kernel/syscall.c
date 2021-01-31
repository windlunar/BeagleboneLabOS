

#include "syscall.h"
#include "../kernel/kprint.h"
#include "debug.h"
#include "task.h"
#include "../klib/mem.h"
#include "memory.h"
#include "file.h"


void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args) ;
void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args)
{
    switch (syscall_id)
    {
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
        __exit_handler() ;
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

    case SYSCALL_ID_getdirent:
        __getdirent_handler(usrTaskContextOld ,args) ;
        break;

    default:
        break;
    }
}


void  __print_hello_handler(uint32_t input)
{
    kprintf("Hello! This is my first system call,Input value =%d\r\n" ,input) ;
}


void __yield_handler(uint32_t *usrTaskContextOld)
{
	// Save old context
	curr_running_task->task_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;

	// Change the task status to ready
	curr_running_task->task_status = TASK_READY ;
	curr_running_task = NULL ;

	//prepare sched() context
	schedFuncContextPrepare();
	_call_sched((uint32_t)schedFuncContextSPtr) ;
}


void __get_tid_handler(uint32_t *usrTaskContextOld)
{
    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = curr_running_task->task_id ;
}


// 回收資源
// 將task設定為 terminate
void __exit_handler()
{
    kprintf("task id=%d exit\r\n" ,curr_running_task->task_id) ;
    curr_running_task->task_status = TASK_TERMINATE ;

    //從queue中移除
    remove_from_readylist(curr_running_task) ;

    //釋放空間
    MEM_AREA_INFO_t *curr_ma = which_mem_area(curr_running_task->stk_bottom) ;
    free_mem_area(curr_ma) ;
  
    curr_running_task->stk_bottom = NULL ;
    curr_running_task = NULL ;

	//prepare sched() context
	schedFuncContextPrepare();
	_call_sched((uint32_t)schedFuncContextSPtr) ;
}



// 複製一份相同的stack ,跟 TASK_INFO_t 結構體
void __fork_handler(uint32_t *usrTaskContextOld)
{    
    // Alloc 一個 Memory Area,並回傳描述該area的結構體 MEM_AREA_INFO_t
    // 並將該 area使用狀況設為 INUSE_FULL ,只屬於這個task使用
    MEM_AREA_INFO_t *n_memarea = memAreaAlloc();
    n_memarea->area_status = TASK_AREA ;


    // 分配 描述 task的TASK_INFO_t結構體, 其起始位置設定為 stack bottom
    // 這樣就能空出前面 4096 -512 bytes的連續空間
    TASK_INFO_t *ntask = (TASK_INFO_t *)stktop2bottom(n_memarea->m_top) ;


    // 找目前正在執行的task(父task)的stack空間屬於那一個 memory area
    MEM_AREA_INFO_t *curr_ma = which_mem_area(curr_running_task->stk_bottom) ;


    // 複製父task使用的記憶體區段內的data給子task的記憶體區段, 包含stack空間內所有內容
    _memcpy((void *)(n_memarea->m_start)
            ,(void *)(curr_ma->m_start)
            ,AREA_SIZE) ;


    // 複製描述 task的TASK_INFO_t結構體
    _memcpy((void *)ntask ,(void *)curr_running_task ,sizeof(TASK_INFO_t));


    // Stack pointer要指向stack中相同的相對位址上
    // (curr_running_task->stk_top - usrTaskContextOld)
    //  = old context相對於stack top 的offset
    ntask->stk_bottom = stktop2bottom(n_memarea->m_top) ;
    ntask->stk_top = n_memarea->m_top ;
    ntask->task_context = (USR_TASK_CONTEXT_t *)(ntask->stk_top - (curr_running_task->stk_top - usrTaskContextOld) ) ;
    

    // 設定TASK_INFO_t結構體的其他內容
    ntask->next_ptr = NULL ;
    ntask->prev_ptr = NULL ;
    ntask->task_status = TASK_READY ;
    ntask->taskCallBack = curr_running_task->taskCallBack ;
    ntask->priority = curr_running_task->priority ;

    //
    //open_console_in_out(ntask) ;


    // 設定子stack的 task id
	taskid++ ;
	ntask->task_id = taskid;
    

    // init blocks
    // 總共應該會有56個blks = (4096-512)/64
    memblks_init(n_memarea 
                ,DEFAULT_AVAL_BLK_SIZE 
                ,DEFAULT_TASK_MA_BLKNUM) ;

    // 將子task放入 rady list中
    task_enqueue(ntask) ;


    //設定回傳值
    ntask->task_context->r0 = 0 ;
    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = ntask->task_id ;
    
}
/************************************************************************************************/
void __do_taskCreate_handler(uint32_t *usrTaskContextOld ,void *arg)
{
    TASK_CONFIG *config = (TASK_CONFIG *)arg ;

    MEM_AREA_INFO_t *n_memarea = memAreaAlloc();
    n_memarea->area_status = TASK_AREA ; 

    TASK_INFO_t *ntask = (TASK_INFO_t *)stktop2bottom(n_memarea->m_top) ;  

    taskCreate(ntask ,config->taskCallBack ,stktop2bottom(n_memarea->m_top) ,config->prio);

    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = ntask->task_id ;

    // init blocks
    // 總共應該會有56個blks = (4096-512)/64
    memblks_init(n_memarea 
                ,DEFAULT_AVAL_BLK_SIZE 
                ,DEFAULT_TASK_MA_BLKNUM) ;

    //
    open_console_in_out(ntask) ;

    //設定路徑
    ntask->cwdn = curr_running_task->cwdn ;

    task_enqueue(ntask) ; 
}



void __malloc_blk_handler(uint32_t *usrTaskContextOld)
{
    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;

    MEM_AREA_INFO_t *curr_ma = which_mem_area(curr_running_task->stk_bottom) ;
    old_context->r0 = (uint32_t)blk_alloc(curr_ma) ;
}


void __malloc_mfree_blk_handler(uint32_t *usrTaskContextOld ,void *blk_aval_start)
{
    free_blk(blk_aval_start) ;
}



void __get_mblk_list_handler(uint32_t *usrTaskContextOld)
{
    MEM_AREA_INFO_t *curr_ma = which_mem_area(curr_running_task->stk_bottom) ;
    uint32_t *head = curr_ma->blk_head_ptr ;

    while(*head != 0)
    {
        kprintf("blk addr =%p ,content =%x\r\n",head ,*head) ;
        head = (uint32_t *)*head ;
    }
    kprintf("blk addr =%p ,content =%x\r\n",head ,*head) ;    
}



void __get_task_priority_handler(uint32_t *usrTaskContextOld)
{
    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = curr_running_task->priority ;    
}


// 之後再implement讓該task staus轉為block ,等寫完在ready
void __write_handler(uint32_t *usrTaskContextOld ,void *args)
{
    FILE_RDWR_ARGS_t *write_args = (FILE_RDWR_ARGS_t *)args ;
    int fd = write_args->fd ;
    uint8_t *wrbuf = write_args->buf ;
    uint32_t n_bytes = write_args->n_bytes ;

    int ret = curr_running_task->openfiles[fd]->file_write(wrbuf ,n_bytes) ;

    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = ret ;  
}



void __read_handler(uint32_t *usrTaskContextOld ,void *args)
{
    FILE_RDWR_ARGS_t *write_args = (FILE_RDWR_ARGS_t *)args ;
    int fd = write_args->fd ;
    uint8_t *rdbuf = write_args->buf ;
    uint32_t n_bytes = write_args->n_bytes ;

    int ret = curr_running_task->openfiles[fd]->file_read(rdbuf ,n_bytes) ; 

    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = ret ;    
}



void __open_handler(uint32_t *usrTaskContextOld ,void *args)
{
    char *path = (char *)args ;
    int fd = -1 ;

    fd = file_open(path ,(void *)curr_running_task) ;
    if(fd < 0) kprintf("Error :Can't open file.\r\n") ;

    // Setting return value
    USR_TASK_CONTEXT_t *old_context = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;
    old_context->r0 = fd ;  
}



void __getcwd_handler(uint32_t *usrTaskContextOld ,void *args)
{
    GETCWD_ARG_t *getcwdarg = (GETCWD_ARG_t *)args ;

    if( strlen(curr_running_task->cwdn->namebuf) > getcwdarg->n_size) getcwdarg->buf = NULL ;

    strcat(getcwdarg->buf ,curr_running_task->cwdn->name) ;
}



void __getdirent_handler(uint32_t *usrTaskContextOld ,void *args)
{
    char *dir_ens = (char *)args ;

}