
#ifndef __TASK_H_
#define __TASK_H_


#include "../common.h"
#include "printk.h"
#include "debug.h"
#include "memory.h"
#include "file.h"
#include "mmu.h"


extern int32_t taskid;

/*************************************************************************/

#define TASK_RUNNING    0
#define TASK_READY      1
#define TASK_TERMINATE  2
#define TASK_WAIT       3

/*************************************************************************/

#define MAXNUM_PRIORITY    5
#define LOWEST_PRIORITY    MAXNUM_PRIORITY-1 
#define HIGHEST_PRIORITY   0 

/*************************************************************************/

#define TASK_NUM_MAX 32
#define stktop2bottom(x)   (x)-(TASK_STACK_SIZE/4)+1
#define stkbottom2top(x)   (x)+(TASK_STACK_SIZE/4)-1


/*************************************************************************/

#define MAX_FD   8
#define MAX_DIR_LENGTH  64

/*************************************************************************/

#define KSTACK_SCHED_CONTEXT_SP	( (KSTACK_TOP_PADDR)-(4*13) )
// -----------------
// ......
// lr
// r12
// r11
// r10
// r9
// r8
// r7
// r6
// r5
// r4
// r3
// r2
// r1 -----------------kernel stack top + 4*1
// r0 -----------------kernel init stack top
struct SCHED_CONTEXT
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t lr;

};
extern struct SCHED_CONTEXT *schedFuncContextSPtr ; 



// ************************************************
//
//                 SP------>lr
//                          r12
// push=STMFD SP!           r11
//        |                 r10
//        |                 r9      ^
//        |                 r8      |
//        |                 r7      |
//        |                 r6      |
//        |                 r5      |
//        |                 r4      |
//        V                 r3      |
//                          r2  pop=LDMFD SP!      
//                          r1              
//                          r0 <------SP
//
// ************************************************
struct TASK_CONTEXT
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9_return_lr;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12_spsr;
    uint32_t lr;

};


struct TASK_INFO
{
    struct TASK_INFO *next_ptr ;
    struct TASK_INFO *prev_ptr ;   
    uint32_t *stk_bottom ;              // task 的 stack空間 的pointer
    uint32_t *stk_top ;
    void (*taskCallBack)() ;            // task function pointer
    int32_t task_id;
    int32_t task_status ;
    struct TASK_CONTEXT *task_context ; // task context ,存放在stack中
    int32_t priority ; 
    struct FILE *openfiles[MAX_FD] ;   // 打開的檔案
    struct DIR_NODE *cwdn ;            // current working dir node structure
    uint32_t *pgtbase ;
};


struct TASK_ARGS
{
    // task function pointer
    void (*taskCallBack)() ;

    int32_t prio ; //

};


/*************************************************************************/

extern struct TASK_INFO *curr_running_task ;

/*************************************************************************/

void sched_first_run(void);
void sched (void) ;
void set_sched_context(void) ;
void set_first_sched(void);
extern void call_sched(uint32_t schedContext) ;    /*定義在task_asm.s */
void first_run(uint32_t *sp);     /*輸入參數 stack(Process stack pointer)會存到r0 */
void switch_task(uint32_t *sp);
struct TASK_INFO *choose_task(void) ;

void task_init() ;
int32_t taskCreate(struct TASK_INFO *task ,void (*taskFunc)() ,void *stack ,int32_t prio);
int32_t do_ktaskCreate(int32_t prio ,void (*taskFunc)());
void open_console_in_out(struct TASK_INFO *task) ;
void task_enqueue(struct TASK_INFO *task) ;
struct TASK_INFO *task_dequeue(int32_t prio) ;
void task_pop(struct TASK_INFO *task);
void set_page_free_start(uint32_t mv_bytes ,struct PAGE_INFO *pg);

/*************************************************************************/

void print_task_id_from_head(int32_t prio) ;
void print_task_addr_from_head(int32_t prio) ;

#endif