
#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#include "../common.h"

/***************************************************************************************/
// Define System call id
/***************************************************************************************/
#define SYSCALL_ID_print_hello          1
#define SYSCALL_ID_yield                2        
#define SYSCALL_ID_get_tid              3
#define SYSCALL_ID_exit                 4
#define SYSCALL_ID_fork                 5
#define SYSCALL_ID_do_taskCreate        6
#define SYSCALL_ID_malloc_blk           7
#define SYSCALL_ID_mfree_blk            8
#define SYSCALL_ID_get_mblk_list        9
#define SYSCALL_ID_get_task_priority    10
#define SYSCALL_ID_write                11
#define SYSCALL_ID_read                 12
#define SYSCALL_ID_open                 13
#define SYSCALL_ID_getcwd               14
#define SYSCALL_ID_getsubdir            15
#define SYSCALL_ID_getfdir              16
#define SYSCALL_ID_chdir                17
#define SYSCALL_ID_getfullpath          18
#define SYSCALL_ID_restart              19
#define SYSCALL_ID_close                20

/***************************************************************************************/
//
/***************************************************************************************/
void svc_entry(void) ;
extern void syscall_handler(uint32_t syscall_id ,uint32_t *usrTaskContextOld ,void *args);


/***************************************************************************************/
//svc syscall call by syscall_handler
/***************************************************************************************/
void __print_hello_handler(uint32_t input) ;
void __yield_handler(uint32_t *usrTaskContextOld);
void __get_tid_handler(uint32_t *usrTaskContextOld);
void __exit_handler(uint32_t *usrTaskContextOld);
void __fork_handler(uint32_t *usrTaskContextOld);
void __do_taskCreate_handler(uint32_t *usrTaskContextOld ,void *arg) ;
void __malloc_blk_handler(uint32_t *usrTaskContextOld) ;
void __malloc_mfree_blk_handler(uint32_t *usrTaskContextOld ,void *blk_aval_start) ;
void __get_mblk_list_handler(uint32_t *usrTaskContextOld) ;
void __get_task_priority_handler(uint32_t *usrTaskContextOld) ;
void __write_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __read_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __open_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __getcwd_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __getsubdir_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __getfdir_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __chdir_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __getfullpath_handler(uint32_t *usrTaskContextOld ,void *args) ;
void __restart_handler(void) ;
void __close_handler(uint32_t *usrTaskContextOld ,void *args) ;

/***************************************************************************************/
//syscall call by user task ,define in syscall_asm.s
/***************************************************************************************/
extern void syscall_print_hello(uint32_t *input) ;
extern void syscall_yield();
extern int syscall_get_tid();
extern void syscall_exit();
extern int syscall_fork();
extern int syscall_do_taskCreate(void *arg);
extern void *syscall_malloc_blk();
extern void syscall_mfree_blk(void *blk_aval_start);
extern void syscall_get_mblk_list();
extern int syscall_get_task_priority() ;
extern int syscall_write() ;
extern int syscall_read() ;
extern int syscall_open() ;
extern void syscall_getcwd(void *arg) ;
extern void syscall_getsubdir() ;
extern void syscall_getfdir() ;
extern int syscall_chdir() ;
extern void syscall_getfullpath() ;
extern void syscall_restart();
extern void syscall_close();

/***************************************************************************************/
// Args Structure for syscall
/***************************************************************************************/
struct BUF_AND_SZ_ARG
{
    char *buf ;
    uint32_t n_size ;
};


/***************************************************************************************/
extern void set_context_sp(uint32_t *context) ;


#endif