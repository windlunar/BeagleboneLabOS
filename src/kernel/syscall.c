

#include "syscall.h"
#include "../klib/print.h"
#include "debug.h"
#include "task.h"
#include "../klib/queue.h"
#include "../klib/mem.h"


void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args) ;
void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args)
{
    //kprintf("syscall_id=%d\r\n",syscall_id) ;
    switch (syscall_id)
    {
    case SYSCALL_ID_print_hello:
        __print_hello(*(uint32_t *)args) ;
        break;

    case SYSCALL_ID_yield:
        __yield(usrTaskContextOld) ;
        break;   

    case SYSCALL_ID_get_tid:
        __get_tid((uint32_t *)args) ;
        break;  

    case SYSCALL_ID_exit:
        __exit() ;
        break;  

    case SYSCALL_ID_fork:
        __fork(usrTaskContextOld ,(uint32_t *)args) ;
        break;  

    default:
        break;
    }
}


void  __print_hello(uint32_t input)
{
    kprintf("Hello! This is my first system call,Input value =%d\r\n" ,input) ;
    //readCpsrMode();

}

void __yield(uint32_t *usrTaskContextOld)
{
	// Save old context
	curr_running_task->task_context_sp = (USR_TASK_CONTEXT_t *)usrTaskContextOld ;

	// Change the task status to ready
	curr_running_task->task_status = TASK_READY ;
	curr_running_task = NULL ;

	//prepare sched() context
	schedFuncContextPrepare();

	//dataSyncBarrier();

	_call_sched((uint32_t)schedFuncContextSPtr) ;
}

void __get_tid(uint32_t *tid_return)
{
    //kprintf("In get tid\r\n");
    *tid_return = curr_running_task->task_id ;
}


// 回收資源
// 將task設定為 terminate
void __exit()
{
    kprintf("task id=%d exit\r\n" ,curr_running_task->task_id) ;
    curr_running_task->task_status = TASK_TERMINATE ;

    //從queue中移除
    remove_from_readylist(curr_running_task) ;

    //釋放空間
    MEM_PART_INFO_t *curr_mpinfo = which_mem_part(curr_running_task->task_stack_ptr) ;
    free_part_mem(curr_mpinfo) ;
  
    curr_running_task->task_stack_ptr = NULL ;
    curr_running_task = NULL ;

	//prepare sched() context
	schedFuncContextPrepare();
	_call_sched((uint32_t)schedFuncContextSPtr) ;
}



//應該要複製一份相同的stack
//共用 .text segment
void __fork(uint32_t *usrTaskContextOld ,uint32_t *args)
{    
    // Prepare new stack
    // and TAST_t structure
    MEM_PART_INFO_t *n_mempart = alloc_one_mem_part();
    n_mempart->part_status = INUSE_FULL ;

    TASK_t *ntask = (TASK_t *)n_mempart->mempart_start_ptr ;

    //copy stack
    MEM_PART_INFO_t *curr_mpinfo = which_mem_part(curr_running_task->task_stack_ptr) ;

    _memcpy((void *)(n_mempart->mempart_start_ptr)
            ,(void *)(curr_mpinfo->mempart_start_ptr)
            ,PART_SIZE) ;

    _memcpy((void *)ntask ,(void *)curr_running_task ,sizeof(TASK_t));

    uint32_t old_sp = (uint32_t)usrTaskContextOld ;
    uint32_t new_sp = (uint32_t)(n_mempart->mempart_start_ptr) ;

    old_sp &= 0xFFF ;
    new_sp |= old_sp ;

    ntask->task_context_sp = (USR_TASK_CONTEXT_t *)new_sp ;

	taskid++ ;
	ntask->task_id = taskid;

    ntask->next_ptr = NULL ;
    ntask->prev_ptr = NULL ;
    ntask->task_status = TASK_READY ;
    ntask->taskCallBack = curr_running_task->taskCallBack ;
    ntask->task_stack_ptr = n_mempart->mempart_top_ptr-256 ;
    
    task_enqueue(ntask) ;

    // return child tid
    *args = ntask->task_id ;
    
}
/************************************************************************************************/
