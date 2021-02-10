
#ifndef __SYSCALL_H_
#define __SYSCALL_H_


#include "../common.h"


/***************************************************************************************/
// Define System call id
/***************************************************************************************/
#define MAX_SYSCALL_ID                  20

#define SYSCALL_ID_print_hello          0
#define SYSCALL_ID_yield                1        
#define SYSCALL_ID_get_tid              2
#define SYSCALL_ID_exit                 3
#define SYSCALL_ID_fork                 4
#define SYSCALL_ID_do_taskCreate        5
#define SYSCALL_ID_malloc_blk           6
#define SYSCALL_ID_mfree_blk            7
#define SYSCALL_ID_get_mblk_list        8
#define SYSCALL_ID_get_task_priority    9
#define SYSCALL_ID_write                10
#define SYSCALL_ID_read                 11
#define SYSCALL_ID_open                 12
#define SYSCALL_ID_getcwd               13
#define SYSCALL_ID_getsubdir            14
#define SYSCALL_ID_getfdir              15
#define SYSCALL_ID_chdir                16
#define SYSCALL_ID_getfullpath          17
#define SYSCALL_ID_restart              18
#define SYSCALL_ID_close                19


/***************************************************************************************/

void svc_entry(void) ;
extern void syscall_handler(uint32_t syscall_id ,void *usrTaskContextOld ,void *args);

/***************************************************************************************/
//svc syscall call by syscall_handler
/***************************************************************************************/

void print_hello_handler(void *usrTaskContextOld ,void *args) ;
void yield_handler(void *usrTaskContextOld ,void *args);
void get_tid_handler(void *usrTaskContextOld ,void *args);
void exit_handler(void *usrTaskContextOld ,void *args);
void fork_handler(void *usrTaskContextOld ,void *args);
void do_taskCreate_handler(void *usrTaskContextOld ,void *args) ;
void malloc_blk_handler(void *usrTaskContextOld ,void *args) ;
void malloc_mfree_blk_handler(void *usrTaskContextOld ,void *args) ;
void get_mblk_list_handler(void *usrTaskContextOld ,void *args) ;
void get_task_priority_handler(void *usrTaskContextOld ,void *args) ;
void write_handler(void *usrTaskContextOld ,void *args) ;
void read_handler(void *usrTaskContextOld ,void *args) ;
void open_handler(void *usrTaskContextOld ,void *args) ;
void getcwd_handler(void *usrTaskContextOld ,void *args) ;
void getsubdir_handler(void *usrTaskContextOld ,void *args) ;
void getfdir_handler(void *usrTaskContextOld ,void *args) ;
void chdir_handler(void *usrTaskContextOld ,void *args) ;
void getfullpath_handler(void *usrTaskContextOld ,void *args) ;
void restart_handler(void *usrTaskContextOld ,void *args) ;
void close_handler(void *usrTaskContextOld ,void *args) ;

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