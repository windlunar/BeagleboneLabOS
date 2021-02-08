
#ifndef __USYSCALL_H_
#define __USYSCALL_H_

#include "../common.h"
#include "../kernel/syscall.h"
#include "../kernel/task.h"
#include "../kernel/file.h"

/****************************************************************************/
// Console default file description
/****************************************************************************/
#define FD_CONSOLE_IN   0
#define FD_CONSOLE_OUT  1


/****************************************************************************/
// Common file path
/****************************************************************************/
#define TTY0            "/tty0\0"
#define IPC0            "/ipc0\0"




/****************************************************************************/
// System Calls
/****************************************************************************/

void __print_hello(uint32_t *input) ;
void __yield(void);
int __gettid(void);
void __exit(void);
int __fork(void);
int __do_taskCreate(void (*taskFunc)() ,int32_t prio);
void *__malloc_blk() ;
void __mfree_blk(void *blk_aval_start) ;
void __get_mblk_list();
int __get_task_priority();
int __write(int fd ,char *buf ,uint32_t n_bytes);
int __read(int fd ,char *buf ,uint32_t n_bytes);
int __open(char *path);    //return fd
void __getcwd(char *buf ,int n_bytes);
int __getsubdir(char *buf ,int n_bytes) ;
int __getfdir(char *buf ,int n_bytes) ;
int __chdir(char *subdirname) ;
void __getfullpath(char *buf ,int n_bytes);
void __restart(void) ;
void __close(int fd) ;

#endif