

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
#define PAGE_NUM            (446) //0x82100000 ~ 0x9df00000

#define KB_SIZE             (1024)
#define MB_SIZE             (1024 * 1024)
#define VALUABLE_MEM_SIZE   (128 * 1024 * 1024)  //128 MB
#define AREA_NUM            (VALUABLE_MEM_SIZE/(AREA_SIZE))  //2*1024個areas

// 如果第一個area的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define KERN_PADDR     ( ROUNDUP((uint32_t)kernal_end ,0x10000) )
#define KERN_PADDR_PTR      ( (uint32_t *)KERN_PADDR )     
#define KERN_VADDR    (KERN_PADDR)
#define KERN_VADDR_PTR    ( (uint32_t *)KERN_VADDR ) 

#define AREAS_START_PADDR    ( ROUNDUP((uint32_t)kernal_end ,PAGE_SIZE) )
#define AREAS_START_PADDR_PTR    ( (uint32_t *)AREAS_START_PADDR ) 

/****************************************************************************************/
// Structs
/****************************************************************************************/
// Define page_status
#define FREE    0
#define INUSE_PARTIALLY_FREE    1
#define INUSE_FULL  2
#define PAGE_FOR_TASK  3

// 如果是last node ,則 next = NULL
// next 指向下一個node address
struct PAGE_INFO{
    struct PAGE_INFO *next ;
    struct PAGE_INFO *prev ;
    uint32_t page_status;
    uint32_t page_id ;
    uint32_t *m_start ;
    uint32_t *task_stk_top ;    //指向可用的stack top
    uint32_t *top ;
    uint32_t *m_free_start ;
    uint32_t *this_info_location ;

    uint32_t *blk_head_ptr ;    //在 page 中可用的起始位址 head
    uint32_t blksize ;
    uint32_t n_blk ;
};

extern struct PAGE_INFO *free_page_head;
extern struct PAGE_INFO *inuse_page_head;
extern struct PAGE_INFO *page_list[PAGE_NUM] ;


/****************************************************************************************/
// Memory page Allocate Functions
/****************************************************************************************/

//主要
void page_list_init();
struct PAGE_INFO *page_alloc(void);
void page_free(struct PAGE_INFO *page_node);

//次要
void add_to_free_list_end(struct PAGE_INFO *page_node);
void insert_to_inuse_list(struct PAGE_INFO *page_node);
void delete_from_inuse_list(struct PAGE_INFO *page_node);
void clean_area_content(void *start);

uint32_t atleast_a_page_alloc(void) ;
struct PAGE_INFO *find_page_list_end(struct PAGE_INFO *headnode);
struct PAGE_INFO *find_aval_inuse_page(void);
/****************************************************************************************/
// alloc block
/****************************************************************************************/
#define DEFAULT_BLK_SIZE    64
#define DEFAULT_AVAL_BLK_SIZE   DEFAULT_BLK_SIZE-4

//主要
struct PAGE_INFO *memblks_init(struct PAGE_INFO *ma ,uint32_t blk_aval_size ,uint32_t num_blks);
void *blk_alloc(struct PAGE_INFO *ma);
void *demand_a_blk();
void free_blk(void *blk_aval_start);

//次要
struct PAGE_INFO *which_page(void *address);
uint32_t *find_prev_blk(struct PAGE_INFO *memarea ,uint32_t *blk_start);
void put_to_blklist_end(struct PAGE_INFO *ma ,uint32_t *blkstart);
uint32_t is_blk_init(struct PAGE_INFO *ma) ;
uint32_t no_blks(struct PAGE_INFO *ma);
/****************************************************************************************/
// alloc 小塊記憶體相關function
/****************************************************************************************/

void *kmalloc(void);
void kfree(void *p);
/****************************************************************************************/
#endif