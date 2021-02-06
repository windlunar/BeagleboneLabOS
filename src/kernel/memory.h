

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../common.h"
#include "../driver/uart.h"
#include "kprint.h"
//#include "../klib/mem.h"

extern uint32_t _end ;	//_end is define in linker.ld
extern uint32_t *kernal_end ;
#define KERNEL_END_PADDR    (uint32_t)kernal_end
#define KERNEL_END_VADDR    (KERNEL_END_PADDR)



#define AREA_SIZE	        ( 65536 )	// 4096 * 16 = 65536(0x10000) bytes(64KB) 
#define PAGE_SIZE           ( 0x100000) // 1M
#define KB_SIZE             ( 1024 )
#define MB_SIZE             ( 1024 * 1024 )
#define VALUABLE_MEM_SIZE   ( 128 * 1024 * 1024 )  //128 MB
#define TOTAL_AREA_NUM      ( VALUABLE_MEM_SIZE/(AREA_SIZE) )  //2*1024個areas

// 如果第一個area的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define FIRST_AREA_PADDR     ( ROUNDUP((uint32_t)kernal_end ,AREA_SIZE) )
#define FIRST_AREA_PADDR_PTR      ( (uint32_t *)FIRST_AREA_PADDR )     

#define FIRST_AREA_VADDR    (FIRST_AREA_PADDR)
#define FIRST_AREA_VADDR_PTR    ( (uint32_t *)FIRST_AREA_VADDR ) 


/****************************************************************************************/
// Structs
/****************************************************************************************/
// Define area_status
#define FREE    0
#define INUSE_PARTIALLY_FREE    1
#define INUSE_FULL  2
#define TASK_AREA  3

// 如果是last node ,則 next_ptr = NULL
// next_ptr 指向下一個node address
struct AREA_INFO{
    struct AREA_INFO *next_ptr ;
    struct AREA_INFO *prev_ptr ;
    uint32_t area_status;
    uint32_t area_id ;
    uint32_t *m_start ;
    uint32_t *m_top ;
    uint32_t *m_aval_start ;

    uint32_t *blk_head_ptr ; //在memory area(page)中可用的起始位址 head
    uint32_t blksize ;
    uint32_t n_blk ;
};

typedef struct AREA_INFO MEM_AREA_INFO_t ;
extern MEM_AREA_INFO_t *free_area_list_head;
extern MEM_AREA_INFO_t *inuse_area_list_head;
extern MEM_AREA_INFO_t areas_list[TOTAL_AREA_NUM] ;


/****************************************************************************************/
// Memory Area Allocate Functions
/****************************************************************************************/


//主要
void mem_areas_list_init();
MEM_AREA_INFO_t *alloc_mem_area(void);
void free_mem_area(MEM_AREA_INFO_t *area_node);

//次要
void add_to_free_list_end(MEM_AREA_INFO_t *area_node);
void insert_to_inuse_list(MEM_AREA_INFO_t *area_node);
void delete_from_inuse_list(MEM_AREA_INFO_t *area_node);
void clean_mem_area_content(void *start);

uint32_t atleast_a_memarea_alloc(void) ;
MEM_AREA_INFO_t *find_ma_end(MEM_AREA_INFO_t *headnode);
MEM_AREA_INFO_t *find_aval_inuse_memarea(void);
/****************************************************************************************/
// alloc block
/****************************************************************************************/
#define DEFAULT_BLK_SIZE    64
#define DEFAULT_AVAL_BLK_SIZE   DEFAULT_BLK_SIZE-4

//主要
MEM_AREA_INFO_t *memblks_init(MEM_AREA_INFO_t *ma ,uint32_t blk_aval_size ,uint32_t num_blks);
void *blk_alloc(MEM_AREA_INFO_t *ma);
void *demand_a_blk();
void free_blk(void *blk_aval_start);

//次要
MEM_AREA_INFO_t *which_mem_area(void *address);
uint32_t *find_prev_blk(MEM_AREA_INFO_t *memarea ,uint32_t *blk_start);
void put_to_blklist_end(MEM_AREA_INFO_t *ma ,uint32_t *blkstart);
uint32_t is_blk_init(MEM_AREA_INFO_t *ma) ;
uint32_t no_blks(MEM_AREA_INFO_t *ma);
/****************************************************************************************/
// alloc 小塊記憶體相關function
/****************************************************************************************/

void *kmalloc(void);
void kfree(void *p);
/****************************************************************************************/
#endif