
#ifndef __MEMORY_H_
#define __MEMORY_H_


#include "../common.h"
#include "../driver/uart.h"
#include "printk.h"


/*_end is define in linker.ld */
extern uint32_t _end ;	
extern uint32_t *kernal_end ;


#define KERNEL_END_PADDR    (uint32_t)kernal_end
#define KERNEL_END_VADDR    (KERNEL_END_PADDR)

#define PAGE_SIZE           (0x100000) /* 1M */
#define PAGE_NUM            (446) /* 0x82100000 ~ 0x9df00000 */

#define TASK_STACK_SIZE     (4096)
#define BLK_SIZE	        (4096)	
#define BLK_NUM_PER_PAGE    ((PAGE_SIZE)/(BLK_SIZE))    /* 256 blks */

#define KBLK_SIZE	        (16384)	
#define KBLK_NUM_PER_PAGE    ((PAGE_SIZE)/(KBLK_SIZE))    /* 64 blks */

#define KERN_PADDR     ( ROUNDUP((uint32_t)kernal_end ,0x10000) )
#define KERN_PADDR_PTR      ( (uint32_t *)KERN_PADDR )     
#define KERN_VADDR    (KERN_PADDR)
#define KERN_VADDR_PTR    ( (uint32_t *)KERN_VADDR ) 

#define AREAS_START_PADDR    ( ROUNDUP((uint32_t)kernal_end ,PAGE_SIZE) )
#define AREAS_START_PADDR_PTR    ( (uint32_t *)AREAS_START_PADDR ) 

/****************************************************************************************/
// Structs
/****************************************************************************************/
/* Define page_status */
#define FREE            0
#define INUSE           1
#define FULL            2
#define FOR_TASK        3
#define FOR_KERN        4
#define FOR_QUEUE       5


struct PAGE_INFO{
	struct PAGE_INFO *next ;
	struct PAGE_INFO *prev ;
	uint32_t page_status;
	uint32_t page_id ;
	uint32_t *pgstart ;
	uint32_t *task_stk_top ;    /** 指向可用的stack top */
	uint32_t *top ;
	uint32_t *free_start ;

	struct BLK_INFO *blk_list_head ;    /** 在 page 中可用的起始位址 head */
	uint32_t no_free_blks ;
	uint32_t blk_not_init ;
};

extern struct PAGE_INFO *free_page_head;
extern struct PAGE_INFO *inuse_page_head;
extern struct PAGE_INFO *page_list[PAGE_NUM] ;

/****************************************************************************************/
// Memory page Allocate Functions
/****************************************************************************************/

struct BLK_INFO
{
	struct PAGE_INFO *owner ;
	struct BLK_INFO *next ;
	struct BLK_INFO *prev ;
	uint32_t *start ;
	uint32_t *top ; /* 可用的最後四個bytes(指向四個bytes中位址最小的) */
	uint32_t status ;
	uint32_t id ;
};

/** 主要 */
void page_list_init();
struct PAGE_INFO *page_alloc(void);
void page_free(struct PAGE_INFO *page_node);

/** 次要 */
void add_to_free_list_end(struct PAGE_INFO *page_node);
void insert_to_inuse_list(struct PAGE_INFO *page_node);
void delete_from_inuse_list(struct PAGE_INFO *page_node);
void clean_area_content(void *start);
uint32_t atleast_a_page_alloc(void) ;
struct PAGE_INFO *find_page_list_end(struct PAGE_INFO *headnode);
struct PAGE_INFO *find_aval_inuse_page(void);

/****************************************************************************************/
// Block alloc 
/****************************************************************************************/

/** 主要 */
struct PAGE_INFO *blks_init(struct PAGE_INFO *pg);
void *blk_alloc(struct PAGE_INFO *pg);
void free_blk(void *address);

/** 次要 */
struct PAGE_INFO *which_page(void *address);
struct BLK_INFO *find_blk_list_end(struct PAGE_INFO *pg) ;
struct BLK_INFO *which_blk(void *address) ;
void put_to_blklist_end(struct PAGE_INFO *pg ,struct BLK_INFO *blk);
uint32_t is_blk_init(struct PAGE_INFO *pg) ;
uint32_t no_blks(struct PAGE_INFO *pg);

/****************************************************************************************/
// kernel page : 0x82000000 ~ 0x82100000
/****************************************************************************************/

extern struct PAGE_INFO kpage;

void kpage_struct_init() ;
void kpage_blks_init();
void *kblk_alloc(int purpose) ;
void kblk_free(void *address) ;
void free_pgt (void *pgtbase) ;
struct BLK_INFO *which_kblk(void *address) ;

/****************************************************************************************/
// alloc 小塊記憶體相關function
/****************************************************************************************/


#endif