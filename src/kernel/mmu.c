
#include "memory.h"
#include "mmu.h"
#include "../klib/mem.h"

/****************************************************************************************/
// MMU
/****************************************************************************************/

// 獲得pte內容 ,PTE:
//  -----------------------------------------------------------
//  || base   |   0   |   AP | 0 | Domain | 1 | C | B | 1 | 0 |        Content
//  -----------------------------------------------------------
//   31  ~ 20 |19 ~ 12| 11 10| 9 | 8 ~ 5  | 4 | 3 | 2 | 1 | 0 |        bit
//
pte_t gen_pte (paddr_t paddr)
{
    return (paddr & L1_PAGE_PTE_BASE_MASK) | L1_PAGE_PTE_BITS ;
}


pte_paddr_t gen_pte_addr (pgt_paddr_t pgt_base ,vaddr_t vaddr)
{
    return (pgt_base & L1_PAGE_TABLE_BASE_MASK) | vaddr2L1pteidx(vaddr ,MAP_START_VADDR) ;
}


// AP_USER_RW          (0x03)  // privilege R/W
// AP_USER_R_ONLY      (0x02)  // privilege R/W
// AP_USER_PROHIBIT    (0x01)  // privilege R/W
void pte_init (paddr_t pstart ,paddr_t pend ,int permision ,vaddr_t vstart ,uint32_t pgt_base)
{
    pte_t pte ;
    pte_paddr_t pte_paddr ;
    paddr_t vs ;
    paddr_t ve ;
    int page_num = 0 ;

    vs = ROUNDDOWN(pstart ,PAGE_SIZE) ;

    //避免變成 0x1 0000 0000 時 overflow的情況
    if (pend < 0xfff00000) {
        ve = ROUNDUP(pend ,PAGE_SIZE) ;
    } else {
        ve = ROUNDDOWN(pend ,PAGE_SIZE) ;
        //page_num++ ;
    }
    page_num += page_num_cal(pstart ,pend) ;

    // 初始化 page table 的 pte
    for (int i=0 ; i<page_num; i++) { 
        pte = gen_pte(vs + (i << 20)) ;
        pte |= NO_CACHE_WRITEBUF << CACHE_WRITEBUF_BIT_SHIFT ;
        pte |= DEFAULT_DOMAIN << DOMAIN_BIT_SHIFT ;
        pte |= permision << AP_BIT_SHIFT ;
        pte |= XN_DEFAULT << XN_SHIFT ;
        pte |= S_BIT << S_SHIFT ;

        pte_paddr = gen_pte_addr(pgt_base ,vstart + (i << 20)) ;
        _memset((void *)(pte_paddr_t *) pte_paddr ,0 ,4) ;
        *(pte_paddr_t *) pte_paddr = pte ;
        //printk("pte addr =%p ,pte content =%x\r\n" ,(pte_paddr_t *) pte_paddr ,*(pte_paddr_t *) pte_paddr) ;
    }
    
}



void mem_map (void)
{
    uint32_t pgt_base = L1_PAGE_TABLE_BASE_PADDR ;
    pte_init(0x00000000 ,0x80000000 ,AP_USER_PROHIBIT ,0x00000000 ,pgt_base) ;
    pte_init(0x80000000 ,0x82000000 ,AP_USER_PROHIBIT ,0x80000000 ,pgt_base) ;
    pte_init(0x82000000 ,0x82100000 ,AP_USER_R_ONLY ,0x82000000 ,pgt_base) ;
    pte_init(0x82100000 ,0x90000000 ,AP_USER_R_ONLY ,0x82100000 ,pgt_base) ;
    pte_init(0x90000000 ,0x9df00000 ,AP_USER_R_ONLY ,0x90000000 ,pgt_base) ;
    pte_init(0x9df00000 ,0xa0000000 ,AP_USER_R_ONLY ,0x9df00000 ,pgt_base) ;
}

//要先disable mmu再開啟
void mmu_init (void)
{
    invalidate_tlb() ;
    mmu_disable() ;
    mem_map() ;
    set_domain() ;
    set_pgt_base() ;
    mmu_enable() ;
}


void mmu_enable(void)
{
    asm volatile(
        "stmfd sp! ,{r0}\n\t"
        "mrc p15 ,0 ,r0 ,c1 ,c0 ,0\n\t"
        "orr r0 ,#0x01\n\t"
        "mcr p15 ,0 ,r0 ,c1 ,c0 ,0\n\t"       // enable mmu
        "dsb\n\t"
        "ldmfd sp! ,{r0}\n\t"
        :
        :
        :
    ) ;
}


void invalidate_tlb(void)
{
    asm volatile(
        "stmfd sp! ,{r0}\n\t"
        "mcr     p15, #0, r0, c8, c7, #0\n\t"
        "dsb\n\t"
        "ldmfd sp! ,{r0}\n\t"
    ) ;    
}


void mmu_disable(void)
{
    asm volatile(
        "stmfd sp! ,{r0}\n\t"
        "mrc     p15, #0, r0, c1, c0, #0\n\t"
        "bic     r0, r0, #0x01\n\t"
        "mcr     p15, #0, r0, c1, c0, #0\n\t"
        "dsb\n\t"
        "ldmfd sp! ,{r0}\n\t"
        :
        :
        :
    ) ;
}

 

void set_pgt_base(void)
{
    pgt_paddr_t pgtb = (pgt_paddr_t)L1_PAGE_TABLE_BASE_PADDR ;
    asm volatile(
        "stmfd sp! ,{r0}\n\t"
        "mov r0 ,%0\n\t"
        "mcr p15 ,0 ,%0 ,c2 ,c0 ,0\n\t"       // 設定 page table base, CP15的c2 register保存
        "ldmfd sp! ,{r0}\n\t"
        :
        :"r" (pgtb)
        :
    ) ;
}

uint32_t get_pgt_base(void)
{
	uint32_t pgtb;
	asm volatile(
        "mrc p15, 0, %0, c2, c0, 0\n\t" 
        : "=r" (pgtb) 
        : 
        : 
    );
	
	return pgtb;
}

// C3 保存16個domain ,一個domain佔用2bits
// 00 :記憶體區域不能訪問 ,會引起domain fault
// 01 :記憶體訪問須配合pte中的AP bits
// 10 :保留
// 11 :都不進行permission檢查
void set_domain(void)
{
    uint32_t domain = 0x00000001 ;
    asm volatile(
        "stmfd sp! ,{r0}\n\t"
        "mov r0 ,%0\n"
        "mcr p15 ,0 ,%0 ,c3 ,c0 ,0\n\t"
        "ldmfd sp! ,{r0}\n\t"
        :
        :"r" (domain)
        :
    );
}


uint32_t get_domain(void)
{
	uint32_t domain;
	asm volatile(
        "mrc p15, 0, %0, c3, c0, 0\n\t" 
        : "=r" (domain) 
        : 
        : 
    );
	
	return domain;
}


void pgt_base_setup(uint32_t *base)
{
    pgt_paddr_t pgtb = (pgt_paddr_t)base ;
    asm volatile(
        "stmfd sp! ,{r0}\n\t"
        "mov r0 ,%0\n\t"
        "mcr p15 ,0 ,%0 ,c2 ,c0 ,0\n\t"       // 設定 page table base, CP15的c2 register保存
        "ldmfd sp! ,{r0}\n\t"
        :
        :"r" (pgtb)
        :
    ) ;
}


// page table要align 16K ,所以分配4個block(4K)
void *task_pgt_setup (void *pgstart ,void *pgtop)
{
    void *pgt_base = kblk_alloc(FOR_KERN) ;

    pte_init(0x82000000 ,0xa0000000 ,AP_USER_PROHIBIT ,0x82000000 ,(uint32_t)pgt_base) ;

    pte_init((paddr_t)pgstart 
            ,(paddr_t)pgtop 
            ,AP_USER_RW 
            ,(vaddr_t)pgstart
            ,(uint32_t)pgt_base) ;

    return pgt_base ;
}


void set_pte (void *pgstart ,void *pgtop ,void *pgt_base)
{
    pte_init((paddr_t)pgstart 
            ,(paddr_t)pgtop 
            ,AP_USER_RW 
            ,(vaddr_t)pgstart
            ,(uint32_t)pgt_base) ;    
}


void switch_mm (uint32_t *base)
{
    invalidate_tlb() ;

    pgt_base_setup(base) ;
}