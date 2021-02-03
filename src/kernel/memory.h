

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../common.h"
#include "../driver/uart.h"
#include "kprint.h"
//#include "../klib/mem.h"

extern uint32_t _end ;	//_end is define in linker.ld
extern uint32_t *kernal_end ;

#define AREA_SIZE	        ( 65536 )	// 4096 * 16 = 65536(0x10000) bytes(64KB) 
#define PAGE_SIZE           ( 0x100000) // 1M
#define KB_SIZE             ( 1024 )
#define MB_SIZE             ( 1024 * 1024 )
#define VALUABLE_MEM_SIZE   ( 128 * 1024 * 1024 )  //128 MB
#define TOTAL_AREA_NUM      ( VALUABLE_MEM_SIZE/(AREA_SIZE) )  //2*1024個areas

// 如果第一個area的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define FIRST_AREA_ADDR     ( ROUNDUP((uint32_t)kernal_end ,AREA_SIZE) )
#define FIRST_AREA_PTR      ( (uint32_t *)FIRST_AREA_ADDR )     



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
// MMU
/****************************************************************************************/
typedef unsigned int paddr_t ;
typedef unsigned int vaddr_t ;
typedef unsigned int* paddr_ptr_t ;
typedef unsigned int pte_t ;        // page table entry connent
typedef unsigned int pte_paddr_t ;  // page table entry addr
typedef unsigned int pgt_paddr_t ;  // page table addr
/****************************************************************************************/
#define L1_PAGE_PTE_BASE_MASK       (0xfff00000)
#define L1_PAGE_PTE_BITS            (0x02)      //代表 1M page(section) ,只有一個level的轉換
#define L1_PAGE_TABLE_BASE          (FIRST_AREA_ADDR)
#define L1_PAGE_TABLE_BASE_MASK     (0xffffc000)

#define PADDR_MAP_START     ((paddr_t)0x00000000)   //paddr從此處開始映射
#define VADDR_MAP_START     ((vaddr_t)0x00000000)   //vaddr從此處開始映射
/****************************************************************************************/
#define TOTAL_MEM_SIZE_BYTES    (0x100000000) // 4G bytes
//#define TOTAL_MEM_SIZE_BYTES    (0x20000000) // 512M bytes
#define L1_PAGE_SIZE_BYTES      (0x00100000) // 1M bytes


// vaddr的bit20~bit31(12位元)用作 page table的 index
// 所以先減去 vaddr的起始位址後 與 0xfff00000 做位元OR取得 bit20~bit31
// 然後往右 shift 20 位元後在往左shift 2位元(因為一個pte佔用4 bytes)
#define VADDR2L1PTEIDX(x ,s)   ((((x-s)&0xfff00000) >>20) <<2)  // s = vaddr_start

/****************************************************************************************/
// 4096個pages = 4096個pte = 16KB per page table
#define L1_PAGES_NUM  (TOTAL_MEM_SIZE_BYTES / L1_PAGE_SIZE_BYTES)  
#define L1_PAGE_TABLE_SIZE_BYTE     (L1_PAGES_NUM*4)
#define L1_PAGE_TABLE_END   (L1_PAGE_TABLE_BASE + L1_PAGES_NUM)   
#define L1PGT_BASE2END(x)   ((pgt_paddr_t)x + L1_PAGES_NUM)

/****************************************************************************************/
// 獲得pte內容 ,PTE:
//  -----------------------------------------------------------
//  || base   |   0   |   AP | 0 | Domain | 1 | C | B | 1 | 0 |        Content
//  -----------------------------------------------------------
//   31  ~ 20 |19 ~ 12| 11 10| 9 | 8 ~ 5  | 4 | 3 | 2 | 1 | 0 |        bit
//
#define AP_USER_RW          (0x03)  // privilege R/W
#define AP_USER_R_ONLY      (0x02)  // privilege R/W
#define AP_USER_PROHIBIT    (0x01)  // privilege R/W
#define AP_BIT_SHIFT          (10)
#define DOMAIN_BIT_SHIFT       (5)
#define DEFAULT_DOMAIN      (0x0) //共有16個domain可選,可控制記憶體區塊權限,但實際上用AP bits來控制
#define NO_CACHE_WRITEBUF   (0x0)
#define CACHE_WRITEBUF_BIT_SHIFT    (2)
/****************************************************************************************/
pte_t gen_pte (paddr_t paddr) ;
pte_paddr_t gen_pte_addr (pgt_paddr_t pgt_base ,vaddr_t vaddr) ;
void mmu_init (void) ;
void enable_mmu (void) ;
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