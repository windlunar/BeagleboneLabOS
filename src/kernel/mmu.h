

#ifndef __MMU_H_
#define __MMU_H_

#include "../common.h"
#include "../driver/uart.h"
#include "kprint.h"


/****************************************************************************************/
// MMU
/****************************************************************************************/
typedef unsigned int paddr_t ;
typedef unsigned int vaddr_t ;
typedef unsigned int pte_t ;        // page table entry connent
typedef unsigned int pte_paddr_t ;  // page table entry addr
typedef unsigned int pgt_paddr_t ;  // page table addr
/****************************************************************************************/
#define L1_PAGE_PTE_BASE_MASK       (0xfff00000)
#define L1_PAGE_PTE_BITS            (0x02)      //代表 1M page(section) ,只有一個level的轉換
#define L1_PAGE_TABLE_BASE_PADDR          AFTER_KEND_PADDR
#define L1_PAGE_TABLE_BASE_VADDR          (L1_PAGE_TABLE_BASE_PADDR)
#define L1_PAGE_TABLE_BASE_MASK     (0xffffc000)

/****************************************************************************************/
#define TOTAL_MEM_SIZE_BYTES    (0x100000000) // 4G bytes
//#define TOTAL_MEM_SIZE_BYTES    (0x20000000) // 512M bytes
#define L1_PAGE_SIZE_BYTES      (0x00100000) // 1M bytes


// vaddr的bit20~bit31(12位元)用作 page table的 index
// 所以先減去 vaddr的起始位址後 與 0xfff00000 做位元OR取得 bit20~bit31
// 然後往右 shift 20 位元後在往左shift 2位元(因為一個pte佔用4 bytes)
// s : vaddr_map_start ,v :vaddr
//
#define vaddr2L1pteidx(v ,s)   ((((v-s)&0xfff00000) >>20) <<2)  

/****************************************************************************************/
// 4096個pages = 4096個pte = 16KB per page table
#define L1_PAGES_NUM                (TOTAL_MEM_SIZE_BYTES / L1_PAGE_SIZE_BYTES)  
#define L1_PAGE_TABLE_SIZE_BYTE     (L1_PAGES_NUM*4)
#define L1_PAGE_TABLE_END_PADDR           (L1_PAGE_TABLE_BASE_PADDR + L1_PAGES_NUM)   
#define L1PGT_BASE2END(x)           ((pgt_paddr_t)x + L1_PAGES_NUM)

/****************************************************************************************/
/**
 * ------------------------- 0xffffffff
 * Un-Used
 * ------------------------- 0xa0000000
 * 
 * Exceptrion vector entry
 * ------------------------- 0x9ff52000
 * 
 * Un-Used
 * ------------------------- 0x9df41000
 * IRQ stack
 * ------------------------- 0x9df40000 ,init sp point to 0x9df3fffc
 * 
 * Kernel stack
 * ------------------------- 0x9df00000
 * 
 * Aval Space
 * 
 * ------------------------- 0x82100000
 * 放置 task_info ,page_info
 * 結構體
 * ------------------------- 0x820x0000 (目前是 0x82020000)
 * Kernel code and un-used
 * ------------------------- 0x82000000
 * Un-used
 * ------------------------- 0x80000000
 * I/O
 * ------------------------- 0x00000000
 */  

#define MEM_END_PADDR               (0xffffffff)
#define KSTACK_TOP_PADDR            (0x9df3fffc)  //sp最高能指向的address
#define KSTACK_BOTTOM_PADDR         (0x9df00000)  //這個addr應為0x9df00000
#define UMEM_START_PADDR            (0x82100000) 
#define KUSE_START_PADDR            (0x82000000)
#define PADDR_MAP_START             (0x00000000)   //paddr從此處開始映射

#define KSTACK_BOTTOM_VADDR         (0x9df00000)  //sp最高能指向的address
#define UMEM_START_VADDR            (0x82100000) //
#define KUSE_START_VADDR            (0x82000000)
#define DRAM_SPACE_START_VADDR      (0x80000000)
#define MAP_START_VADDR             (0x00000000)   //vaddr從此處開始映射

#define pstartend2pagenum(vstart ,vend)   ((ROUNDUP(vend ,PAGE_SIZE)-ROUNDDOWN(vstart ,PAGE_SIZE)) >> 20)   
/****************************************************************************************/
// 獲得pte內容 ,PTE:
//  -----------------------------------------------------------
//  || base   |   0   |   AP | 0 | Domain | 1 | C | B | 1 | 0 |        Content
//  -----------------------------------------------------------
//   31  ~ 20 |19 ~ 12| 11 10| 9 | 8 ~ 5  | 4 | 3 | 2 | 1 | 0 |        bit
//
// Bit define
//
// AP bits
#define AP_USER_RW          (0x03)  // privilege R/W
#define AP_USER_R_ONLY      (0x02)  // privilege R/W
#define AP_USER_PROHIBIT    (0x01)  // privilege R/W
#define AP_BIT_SHIFT          (10)

//domain bits
#define DOMAIN_BIT_SHIFT       (5)
#define DEFAULT_DOMAIN      (0x0) //共有16個domain可選,可控制記憶體區塊權限,但實際上用AP bits來控制

// XN bits :Execute Never
// 要AP permission檢查一定要設定為 0
#define XN_SHIFT    (4)
#define XN_DEFAULT  (0)

// CB bits
#define CACHE_WRITEBUF_BIT_SHIFT    (2)
#define NO_CACHE_WRITEBUF   (0x0)

//APX
#define APX_SHIFT   (15)
#define APX     (0)

//S bit
#define S_SHIFT     (16)
#define S_BIT       1

//NG bit
#define NG_SHIFT     (17)
#define NG       0

/****************************************************************************************/
pte_t gen_pte (paddr_t paddr) ;
pte_paddr_t gen_pte_addr (pgt_paddr_t pgt_base ,vaddr_t vaddr) ;
void pte_init (paddr_t pstart ,paddr_t pend ,int permision ,vaddr_t vstart) ;
void mem_map (void);
void mmu_init (void) ;
void mmu_enable (void) ;
void mmu_disable(void) ;
void set_pgt_base(void) ;
uint32_t get_pgt_base(void);
void set_domain(void) ;
uint32_t get_domain(void);
/****************************************************************************************/

#endif