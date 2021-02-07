
#include "memory.h"
#include "task.h"

uint32_t *kernal_end = (&_end) ;


/****************************************************************************************/
struct PAGE_INFO kpage;
struct BLK_INFO *kblk_list_base = NULL ;
struct PAGE_INFO *free_page_head = NULL;
struct PAGE_INFO *inuse_page_head = NULL;
struct PAGE_INFO *page_list[PAGE_NUM] ;   //存放在 kernel binary的bss segment中

/****************************************************************************************/
// Memory Page Allocate
/****************************************************************************************/
//
// 將所有memory page 用link list串起來
//    
//  |-------|
//  | page  | 
//  |-------|
//  | page  |
//  |-------|
//  | page  |  
//  |-------|
//  |       |
//
// A page(section)
// ----------------------- offset = 0x100000 (1M)
//	Free area for task
// 
// ----------------------- offset = task stack size = 4096 bytes
//	task stack
// ----------------------- offset = sizeof(struct TASK_INFO)
// struct TASK_INFO
// ----------------------- offset = 0
void page_list_init()
{
    for (int32_t j = 0; j < PAGE_NUM ;j++)
        page_list[j] = NULL ;

    page_list[0] = (struct PAGE_INFO *)(kpage.free_start) ;
    free_page_head = page_list[0] ;

    for (int32_t i = 0; i < PAGE_NUM ;i++) {
        page_list[i]->page_status = FREE ; 
        page_list[i]->page_id = i ;
        page_list[i]->pgstart = (uint32_t *)(UMEM_START_PADDR + (i * PAGE_SIZE)) ;
        page_list[i]->task_stk_top = page_list[i]->pgstart + (TASK_STACK_SIZE/4) -1 ;
        page_list[i]->top = page_list[i]->pgstart + (PAGE_SIZE/4) -1 ;
        page_list[i]->blk_list_head = NULL ;
        page_list[i]->free_start = page_list[i]->pgstart ;
        page_list[i]->no_free_blks = FALSE ;
        page_list[i]->blk_not_init = TRUE ;

        page_list[i+1] = page_list[i] + 1 ; // struct pointer + 1

        if (i == PAGE_NUM - 1) {
            page_list[i]->next = NULL ;
        } else {
            page_list[i]->next = page_list[i+1] ;
        }
        
        if (i == 0) {
            page_list[i]->prev = NULL ;
        } else {
            page_list[i]->prev = page_list[i-1] ;
        }
    }  

    kpage.free_start = (uint32_t *)(page_list[PAGE_NUM] + 1) ; 
}



struct PAGE_INFO *page_alloc(void)
{
    //保存原有的 head node
    struct PAGE_INFO *prev_head = free_page_head ;

    //修改現在的 head_ptr
    free_page_head = prev_head->next ;
    free_page_head->prev = NULL ;

    //設定原來的 head node
    prev_head->next =NULL ;
    prev_head->page_status = INUSE ;

    insert_to_inuse_list(prev_head) ;

    //回傳原來 head area的結構指標
    return prev_head ;
}


void page_free(struct PAGE_INFO *page_node)
{
    if (page_node->page_status != FREE) {
        delete_from_inuse_list(page_node) ;
        add_to_free_list_end(page_node) ;
    } else {
        printk("The page info node is already in free list.\r\n") ;
    }
}


void add_to_free_list_end(struct PAGE_INFO *page_node)
{
    //如果還沒有 head node
    if (free_page_head == NULL) {
        free_page_head = page_node ;
        page_node->next = NULL ;
        return ;
    }

    struct PAGE_INFO *curr_head = free_page_head ;

    /** Move pointer from head Node to End node */
    while (curr_head->next != NULL) {
        curr_head = curr_head->next ;
    }
    struct PAGE_INFO *end_ptr = curr_head ;

    end_ptr->next = page_node ;
    page_node->next = NULL ;
    page_node->prev = end_ptr ;

}


void insert_to_inuse_list(struct PAGE_INFO *page_node)
{
    //如果還沒有 head node
    if (inuse_page_head == NULL) {
        inuse_page_head = page_node ;
        page_node->next = NULL ;
        page_node->prev = NULL ;
        return ;
    }

    struct PAGE_INFO *curr_head = inuse_page_head ;

    /** Move pointer from head Node to End node */
    while (curr_head->next != NULL) {
        curr_head = curr_head->next ;
    }
    struct PAGE_INFO *end_ptr = curr_head ;

    end_ptr->next = page_node ;
    page_node->next = NULL ;
    page_node->prev = end_ptr ;

}


void delete_from_inuse_list(struct PAGE_INFO *page_node)
{

    struct PAGE_INFO *prev_node = page_node->prev ;
    struct PAGE_INFO *next_node = page_node->next ;

    //如果 area_node是head ,next node變成 head
    if (prev_node == NULL) {
        inuse_page_head = next_node ;
        next_node->prev = NULL ;
        page_node->next = NULL ;
        return ;
    }

    // 如果 area_node是end ,prev node變成end
    if (next_node == NULL) {
        prev_node->next = NULL ;
        page_node->prev = NULL ;
        return ;
    }

    prev_node->next = next_node ;
    next_node->prev = prev_node ;
    page_node->next = NULL ;
    page_node->prev = NULL ;
}


void clean_area_content(void *start)
{
    uint32_t *startPtr = (uint32_t *)start ;

    for (uint32_t i=0 ;i<(BLK_SIZE/4) ;i++) {
        *startPtr = 0 ;
        startPtr += 1 ;
    }
}


uint32_t atleast_a_page_alloc(void)
{
    if (inuse_page_head == NULL) {
        return FALSE ;
    } else {
        return TRUE ;
    }
}

struct PAGE_INFO *find_page_list_end(struct PAGE_INFO *headnode)
{
    struct PAGE_INFO *head =headnode ;
    while (head != NULL) {
        head = head->next ;
    }
    return head ;
}


struct PAGE_INFO *find_aval_inuse_page(void)
{
    struct PAGE_INFO *head = inuse_page_head ;

    while (head != NULL) {
        if (head->page_status = INUSE) {
            return head;
        }
        head = head->next ;
    }
    return NULL ;
}


// Blocks
// ------------------------------------ page offset = 0x100000
// Free blks
// ------------------------------------ page offset = 0x3000
// struct BLOCK_INFO(needs 2 blks)
// ------------------------------------ page offset = 0x1000
// For Task stack
// ------------------------------------ page offset = 0
struct PAGE_INFO 
*blks_init(struct PAGE_INFO *pg)
{
    //已經初始化過
    if (pg->blk_not_init == FALSE) 
        return pg ;

    int32_t n_blk = BLK_NUM_PER_PAGE ;

    pg->blk_list_head = (struct BLK_INFO *)(pg->pgstart + (BLK_SIZE / 4)) ;
    n_blk = BLK_NUM_PER_PAGE ;
    

    struct BLK_INFO *head = (struct BLK_INFO *)pg->blk_list_head ;
    for ( int i = 0 ; i < n_blk ;i++) {

        head[i].id = i ;
        head[i].status = FREE ;
        head[i].start = pg->pgstart + i * (BLK_SIZE / 4) ;
        head[i].top = head[i].start + (BLK_SIZE - 4) ;
        head[i].owner = pg ;
    
        if (i == 0) {
            head[i].prev = NULL ;
            head[i].next = &head[i+1] ;
        } else if (i == n_blk - 1){
            head[i].prev = &head[i-1] ;
            head[i].next = NULL ;   
        } else {
            head[i].prev = &head[i-1] ; ;
            head[i].next = &head[i+1] ;
        }
    }
    pg->blk_not_init == FALSE ;
    head[0].status = FULL ;         //For task
    head[1].status = FULL ;         // for struct BLK_INFO
    head[2].status = FULL ;         // for struct BLK_INFO
    pg->blk_list_head = (void *)(head + 3) ;

    return pg ;
}

// alloc a blk
void *blk_alloc(struct PAGE_INFO *pg)
{
    if (pg == NULL)
        return NULL ;
    
    if(pg->no_free_blks == TRUE)
        return NULL ;

    struct BLK_INFO *ret = (struct BLK_INFO *)pg->blk_list_head  ;

    if(ret == NULL)
        return NULL ;

    if (ret->next == NULL) {    //the last
        pg->blk_list_head = NULL ;
        pg->no_free_blks = TRUE ; 
    }

    pg->blk_list_head = ret->next ;
    ret->next->prev = NULL ;

    return (void *)(ret->start) ;
}


struct PAGE_INFO *which_page(void *address)
{
    uint32_t *pg = (uint32_t *)ROUNDDOWN((uint32_t)address ,PAGE_SIZE) ;

    //判斷 memarea這個 addr是那個area
    struct PAGE_INFO *head = inuse_page_head ;

    while (head->next != NULL) {
        if (head->pgstart == pg) break ;
        head = head->next ;
    }
    struct PAGE_INFO *target = head ;

    return target ;
}


struct BLK_INFO *find_blk_list_end(struct PAGE_INFO *pg)
{
    struct BLK_INFO *head = (struct BLK_INFO *)pg->blk_list_head ;
    if(head == NULL)
        return NULL ;
    
    while (head->next != NULL) {
        head = head->next ;
    }
    return head ;
}


void put_to_blklist_end(struct PAGE_INFO *pg ,struct BLK_INFO *blk)
{
    if (pg == NULL)
        return ;

    if (blk == NULL)
        return ;

    if (pg->blk_list_head == NULL) {
        pg->blk_list_head = (void *)blk ;
        return ;
    }
    struct BLK_INFO *end = find_blk_list_end(pg) ;

    end->next = blk ;
    blk->prev = end ;
    blk->next = NULL ;
}


struct BLK_INFO *which_blk(void *address)
{
    uint32_t blk_start = ROUNDDOWN((uint32_t)address ,BLK_SIZE) ;
    uint32_t page_start = ROUNDDOWN((uint32_t)address ,PAGE_SIZE) ;

    uint32_t index = ((blk_start - page_start) / BLK_SIZE) ;

    struct BLK_INFO *ret = ((struct BLK_INFO *)(page_start + BLK_SIZE) + index) ;

    return ret ;

}


void free_blk(void *address)
{
    struct BLK_INFO *blk= which_blk(address);
    struct PAGE_INFO *pg = which_page(blk) ;

    //將原本的blk放到list最後
    put_to_blklist_end(pg ,blk) ;
}


uint32_t is_blk_init(struct PAGE_INFO *pg)
{
    if (pg->blk_not_init) {
        return FALSE ;  // Not init
    } else {
        return TRUE ;
    }
}


uint32_t no_blks(struct PAGE_INFO *pg)
{
    if (pg->blk_list_head == NULL) {
        return TRUE ;
    } else {
        return FALSE ;
    }
}

/****************************************************************************************/
// kernel page : 0x82000000 ~ 0x82100000
/****************************************************************************************/

void kpage_struct_init()
{
    kpage.page_id = -1 ;
    kpage.page_status = FOR_KERN ;
    kpage.pgstart = (uint32_t *)0x82000000 ;
    kpage.top = kpage.pgstart + (PAGE_SIZE-4) ;
    kpage.task_stk_top = NULL ;
    kpage.prev = NULL ;
    kpage.next = NULL ;
    kpage.free_start = (KERN_PADDR_PTR + page_num_cal(0x00000000 ,0xa0000000)) ;
    kpage.blk_list_head = NULL ;
    kpage.blk_not_init = TRUE ;
    kpage.no_free_blks = FALSE ;
}

void kpage_blks_init()
{
    kpage.blk_list_head = (void *)(kpage.free_start) ;
    struct BLK_INFO *head = (struct BLK_INFO *)(kpage.blk_list_head) ;

    kpage.free_start = (uint32_t *)(head + KBLK_NUM_PER_PAGE) ;

    for ( int i = 0 ; i < KBLK_NUM_PER_PAGE ;i++) {
        head[i].id = i ;
        head[i].status = FREE ;
        head[i].start = kpage.pgstart + i * (KBLK_SIZE / 4) ;
        head[i].top = head[i].start + (KBLK_SIZE - 4) ;
        head[i].owner = &kpage ;
    
        if (i == 0) {
            head[i].prev = NULL ;
            head[i].next = &head[i+1] ;
        } else if (i == KBLK_NUM_PER_PAGE - 1){
            head[i].prev = &head[i-1] ;
            head[i].next = NULL ;   
        } else {
            head[i].prev = &head[i-1] ; ;
            head[i].next = &head[i+1] ;
        }
    }
    kpage.blk_not_init == FALSE ;
    kblk_list_base = &head[0] ;

    // cal usage
    uint32_t end = ROUNDUP((uint32_t)kpage.free_start ,KBLK_SIZE) ;
    uint32_t index = (end - 0x82000000) >> 12 ;

    for (int j ; j<index; j++) {
        head[j].status = FULL ;
    }
    
    kpage.blk_list_head = (void *)&head[index] ;

    kpage.free_start = (uint32_t *)ROUNDUP((uint32_t)kpage.free_start ,KBLK_SIZE) ;
}



void kblk_free(void *address)
{
    uint32_t blk_start = ROUNDDOWN((uint32_t)address ,BLK_SIZE) ;
    struct BLK_INFO *target = NULL ;

    for ( int i = 0 ; i < BLK_NUM_PER_PAGE ;i++) {
        if (kblk_list_base[i].start == (uint32_t *)blk_start) {
            target = &kblk_list_base[i] ;
            break ;
        }
    }
    
    struct BLK_INFO *head = (struct BLK_INFO *)kpage.blk_list_head ;
    struct BLK_INFO *end = NULL ;

    while (head->next != NULL) {
        head = head->next ;
    }
    end = head ;

    end->next = target ;
    target->prev = end ;
    target->next = NULL ;
}


void *kblk_alloc()
{
    if (kpage.blk_list_head == NULL) {
        printk("blk_list_head is empty.\r\n") ;
    }

    if (kpage.blk_list_head->next == NULL) {
        printk("Last blk.\r\n") ;
    }
    struct BLK_INFO *ret = kpage.blk_list_head ;
    kpage.blk_list_head = kpage.blk_list_head->next ;
    kpage.blk_list_head->prev = NULL ;

    ret->next = NULL ;
    ret->prev = NULL ;

    return (void *)(ret->start) ;

}

// free 16K (4 blks)
void free_pgt (void *pgtbase)
{
    kblk_free(pgtbase) ;
    
}
/****************************************************************************************/
// alloc 小塊記憶體相關function
// 都沒空間的話,要先呼叫 
// page_list_init()
// page_alloc()
// blks_init();
// blk_alloc() ;
/****************************************************************************************/
void *demand_a_blk()
{
    struct PAGE_INFO *pg ;
    // 尚未 alloc 一個 mem page
    if (atleast_a_page_alloc() == FALSE) {
        pg = page_alloc() ;
        pg = blks_init(pg) ;

        void *p = blk_alloc(pg);
        return p ;
    }

    //已經至少alloc過一個area
    pg = find_aval_inuse_page();

    //沒有可用的 mem page
    //預設1個block 64bytes(前4個用作blks linklist指標)
    if (pg == NULL) {
        pg = page_alloc() ;
        pg = blks_init(pg) ;

        void *p = blk_alloc(pg);
        return p ;
    }

    //有可用的 mem page
    //判斷有無 init blocks
    if (is_blk_init(pg) == FALSE) {
        pg = blks_init(pg) ;

        void *p = blk_alloc(pg);
        return p ;       
    }

    void *p = blk_alloc(pg);
    return p ;  

}


void *kmalloc(void)
{
    return demand_a_blk() ;
}


void kfree(void *p){
    free_blk(p) ;
}

/****************************************************************************************/