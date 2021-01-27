

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../common.h"
#include "../driver/uart.h"
#include "../klib/print.h"
#include "../klib/mem.h"

extern uint32_t _end ;	//_end is define in linker.ld
extern uint32_t *kernal_end ;

#define PART_SIZE	        4096	//bytes
#define KB_SIZE             1024
#define MB_SIZE             1024 * 1024
#define VALUABLE_MEM_SIZE   128 * 1024 * 1024  //128 MB
#define TOTAL_PART_NUM      VALUABLE_MEM_SIZE/4096  //32*1024個parts

// 如果第一個part的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define FIRST_PART_ADDR     (ROUNDUP((uint32_t)kernal_end ,PART_SIZE))
#define FIRST_PART_PTR      (uint32_t *)FIRST_PART_ADDR     



/***********************************************************************************************/
// Structs
/***********************************************************************************************/
// Define part_status
#define FREE    0
#define INUSE_PARTIALLY_FREE    1
#define INUSE_FULL  2


// 如果是last node ,則 next_ptr = NULL
// next_ptr 指向下一個node address
struct PART_INFO{
    struct PART_INFO *next_ptr ;
    struct PART_INFO *prev_ptr ;
    uint32_t part_status;
    uint32_t part_id ;
    uint32_t *m_start ;
    uint32_t *m_top ;

    uint32_t *blk_head_ptr ; //在memory part(page)中可用的起始位址 head
    uint32_t blksize ;
};

typedef struct PART_INFO MEM_PART_INFO_t ;
extern MEM_PART_INFO_t *free_part_list_head;
extern MEM_PART_INFO_t *inuse_part_list_head;
extern MEM_PART_INFO_t parts_list[TOTAL_PART_NUM] ;

/***********************************************************************************************/
// Functions
/***********************************************************************************************/
//主要
void mem_parts_list_init();
MEM_PART_INFO_t *alloc_one_mem_part(void);
void free_mem_part(MEM_PART_INFO_t *part_node);

//次要
void add_to_free_list_end(MEM_PART_INFO_t *part_node);
void insert_to_inuse_list(MEM_PART_INFO_t *part_node);
void delete_from_inuse_list(MEM_PART_INFO_t *part_node);
void clean_mem_part_content(void *start);

uint32_t aleast_a_mempart_alloc(void) ;
MEM_PART_INFO_t *find_mpinfo_end(MEM_PART_INFO_t *headnode);
MEM_PART_INFO_t *find_aval_inuse_mempart(void);
/***********************************************************************************************/
// alloc block
/***********************************************************************************************/
#define DEFAULT_BLK_SIZE    64
#define DEFAULT_AVAL_BLK_SIZE   DEFAULT_BLK_SIZE-4

//主要
MEM_PART_INFO_t * memblks_init(MEM_PART_INFO_t *mpinfo ,uint32_t blk_size);
void *blk_alloc(MEM_PART_INFO_t *mpinfo);
void *demand_a_blk(int for_task_stack);
void free_blk(void *blk_aval_start);

//次要
MEM_PART_INFO_t *which_mem_part(void *address);
uint32_t *find_prev_blk(MEM_PART_INFO_t *mempart ,uint32_t *blk_start);
void put_to_blklist_end(uint32_t *blkstart);
uint32_t is_blk_init(MEM_PART_INFO_t *mpinfo) ;
/***********************************************************************************************/
// alloc 小塊記憶體相關function
/***********************************************************************************************/

void *kmalloc(void);
void kfree(void *p);
/***********************************************************************************************/
#endif