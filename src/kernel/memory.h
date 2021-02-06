

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



//#define AREA_SIZE	        (65536)	// 4096 * 16 = 65536(0x10000) bytes(64KB) 
//#define AREA_SIZE	        (0x100000)	
#define AREA_SIZE	        (8192)	
#define PAGE_SIZE           (0x100000) // 1M
#define KB_SIZE             (1024)
#define MB_SIZE             (1024 * 1024)
#define VALUABLE_MEM_SIZE   (128 * 1024 * 1024)  //128 MB
#define TOTAL_AREA_NUM      (VALUABLE_MEM_SIZE/(AREA_SIZE))  //2*1024個areas

// 如果第一個area的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define AFTER_KEND_PADDR     ( ROUNDUP((uint32_t)kernal_end ,0x10000) )
#define AFTER_KEND_PADDR_PTR      ( (uint32_t *)AFTER_KEND_PADDR )     
#define AFTER_KEND_VADDR    (AFTER_KEND_PADDR)
#define AFTER_KEND_VADDR_PTR    ( (uint32_t *)AFTER_KEND_VADDR ) 

#define AREAS_START_PADDR    ( ROUNDUP((uint32_t)kernal_end ,PAGE_SIZE) )
#define AREAS_START_PADDR_PTR    ( (uint32_t *)AREAS_START_PADDR ) 

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
struct MEM_AREA_INFO{
    struct MEM_AREA_INFO *next_ptr ;
    struct MEM_AREA_INFO *prev_ptr ;
    uint32_t area_status;
    uint32_t area_id ;
    uint32_t *m_start ;
    uint32_t *m_top ;
    uint32_t *m_aval_start ;

    uint32_t *blk_head_ptr ; //在memory area(page)中可用的起始位址 head
    uint32_t blksize ;
    uint32_t n_blk ;
};

extern struct MEM_AREA_INFO *free_area_list_head;
extern struct MEM_AREA_INFO *inuse_area_list_head;
extern struct MEM_AREA_INFO areas_list[TOTAL_AREA_NUM] ;


/****************************************************************************************/
// Memory Area Allocate Functions
/****************************************************************************************/


//主要
void mem_areas_list_init();
struct MEM_AREA_INFO *alloc_mem_area(void);
void free_mem_area(struct MEM_AREA_INFO *area_node);

//次要
void add_to_free_list_end(struct MEM_AREA_INFO *area_node);
void insert_to_inuse_list(struct MEM_AREA_INFO *area_node);
void delete_from_inuse_list(struct MEM_AREA_INFO *area_node);
void clean_mem_area_content(void *start);

uint32_t atleast_a_memarea_alloc(void) ;
struct MEM_AREA_INFO *find_ma_end(struct MEM_AREA_INFO *headnode);
struct MEM_AREA_INFO *find_aval_inuse_memarea(void);
/****************************************************************************************/
// alloc block
/****************************************************************************************/
#define DEFAULT_BLK_SIZE    64
#define DEFAULT_AVAL_BLK_SIZE   DEFAULT_BLK_SIZE-4

//主要
struct MEM_AREA_INFO *memblks_init(struct MEM_AREA_INFO *ma ,uint32_t blk_aval_size ,uint32_t num_blks);
void *blk_alloc(struct MEM_AREA_INFO *ma);
void *demand_a_blk();
void free_blk(void *blk_aval_start);

//次要
struct MEM_AREA_INFO *which_mem_area(void *address);
uint32_t *find_prev_blk(struct MEM_AREA_INFO *memarea ,uint32_t *blk_start);
void put_to_blklist_end(struct MEM_AREA_INFO *ma ,uint32_t *blkstart);
uint32_t is_blk_init(struct MEM_AREA_INFO *ma) ;
uint32_t no_blks(struct MEM_AREA_INFO *ma);
/****************************************************************************************/
// alloc 小塊記憶體相關function
/****************************************************************************************/

void *kmalloc(void);
void kfree(void *p);
/****************************************************************************************/
#endif