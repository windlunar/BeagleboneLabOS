
#include "memory.h"
#include "task.h"

uint32_t *kernal_end = (&_end) ;


/****************************************************************************************/
struct PAGE_INFO *free_page_head = NULL;
struct PAGE_INFO *inuse_page_head = NULL;
struct PAGE_INFO *page_list[PAGE_NUM] ;   //存放在 kernel binary的bss segment中

/****************************************************************************************/
// Memory Page Allocate
/****************************************************************************************/
//
// 將所有memory page (1 page 其實就是一個page的大小)用link list串起來
// A memoy Areas
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
void page_list_init()
{
    for (int32_t j = 0; j < PAGE_NUM ;j++)
        page_list[j] = NULL ;

    page_list[0] = (struct PAGE_INFO *)(KERN_PADDR_PTR + page_num_cal(0x00000000 ,0xa0000000)) ;
    free_page_head = page_list[0] ;

    for (int32_t i = 0; i < PAGE_NUM ;i++) {
        page_list[i]->page_status = FREE ; 
        page_list[i]->page_id = i ;
        page_list[i]->m_start = (uint32_t *)(UMEM_START_PADDR + (i * PAGE_SIZE)) ;
        page_list[i]->task_stk_top = page_list[i]->m_start + (TASK_STACK_SIZE/4) -1 ;
        page_list[i]->top = page_list[i]->m_start + (PAGE_SIZE/4) -1 ;
        page_list[i]->blk_head_ptr = (uint32_t *)page_list[i]->m_start ;
        page_list[i]->m_free_start = page_list[i]->m_start ;

        //0 means blks not init
        page_list[i]->blksize = 0 ; 
        page_list[i]->n_blk = 0 ; 

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
    prev_head->page_status = INUSE_PARTIALLY_FREE ;

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

    for (uint32_t i=0 ;i<(AREA_SIZE/4) ;i++) {
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
        if (head->page_status = INUSE_PARTIALLY_FREE) {
            return head;
        }
        head = head->next ;
    }
    return NULL ;
}

/***********************************************************************************************/
//Blocks
//    A memoy Page
//  |-------|<-
//  | blk   | ^
//  |-------|<-->
//  | blk   | ^
//  |-------|<-->
//  | blk   | ^   每個blk前四個byte為一個pointer, 指向下一個blk的起始位址 ,形成一個link list
//  |-------|->
//  |       |
/****************************************************************************************/
// Need to alloc a mem page fitst
// (call page_alloc() first)
// arg1 : 已經allocate的memory page (page_alloc()的回傳值)
// arg2 : number of bytes
struct PAGE_INFO 
*memblks_init(struct PAGE_INFO *ma ,uint32_t blk_aval_size ,uint32_t num_blks)
{
    //已經初始化過
    if (ma->blksize >4) return ma ;
    if (blk_aval_size <= 0) NULL ;

    uint32_t blk_size = blk_aval_size + 4 ;

    //一個blk最少要四個bytes ,前四個byte放指標 ,指向下一個可用的blk
    ma->blksize = blk_size ;
    ma->n_blk = num_blks ;

    uint32_t *head = ma->blk_head_ptr ;
    uint32_t node_addr = (uint32_t)head ;
    
    for (int32_t i = 1 ; i<=num_blks; i++) {
        //the last blk
        if (i == num_blks) {
            *head = 0 ;
            break ;
        }

        //指標要放的內容為下一個 blk的起始位址
        node_addr += blk_size  ;

        //放內容
        *head = node_addr ;

        //移動指標
        head = (uint32_t *)((uint8_t *)head + blk_size) ;
    }
    return ma ;
}

// alloc a blk
// page_list_init() -> page_alloc() -> memblks_init() 
// -> blk_alloc()
void *blk_alloc(struct PAGE_INFO *ma)
{
    // 如果 ma是 task ,且task還有空間為分配, 那就先從task area分配一個blk
    if ((ma->page_status == PAGE_FOR_TASK) && (no_blks(ma) == FALSE)) {
        void *ret = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ret ;          
    }

    if (ma==NULL) {
        ma = page_alloc();
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *ptr_return = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ma->blk_head_ptr+1 ;
    }

    if (ma->blksize <= 4) {
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *ptr_return = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ma->blk_head_ptr+1 ;        
    }

    if (ma->blk_head_ptr == NULL) {
        ma->page_status = INUSE_FULL ;

        ma = page_alloc();
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *ptr_return = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ma->blk_head_ptr+1 ;
    }


    void *ptr_return = (void *)(ma->blk_head_ptr + 1) ;
    ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

    return ptr_return ;    
}


struct PAGE_INFO *which_page(void *address)
{
    uint32_t *memarea = (uint32_t *)ROUNDDOWN((uint32_t)address ,PAGE_SIZE) ;

    //判斷 memarea這個 addr是那個area
    struct PAGE_INFO *area_head = inuse_page_head ;

    while (area_head->next != NULL) {
        if (area_head->m_start == memarea) break ;
        area_head = area_head->next ;
    }
    struct PAGE_INFO *target = area_head ;

    return target ;
}


// 找前一個 blk空間 (不是link list的 prev node)
// 目前沒有用到的函式
uint32_t *find_prev_blk(struct PAGE_INFO *memarea ,uint32_t *blk_start)
{
    uint32_t * prevblk = (uint32_t *)((uint8_t *)blk_start - memarea->blksize) ;

    return prevblk ;
}



void put_to_blklist_end(struct PAGE_INFO *ma ,uint32_t *blkstart)
{
    uint32_t *head = ma->blk_head_ptr ;
    while (*head != 0) {
        head = (uint32_t *)*head ;
    }

    uint32_t *end = head ;
    *end = (uint32_t)blkstart ;

    *blkstart = 0 ;
}


void free_blk(void *blk_aval_start)
{
    uint32_t *blk_start= (uint32_t *)blk_aval_start -1 ;
    struct PAGE_INFO *ma = which_page(blk_start) ;

    //將原本的blk放到list最後
    put_to_blklist_end(ma ,blk_start) ;
}

uint32_t is_blk_init(struct PAGE_INFO *ma)
{
    if (ma->blksize <= 4) {
        return FALSE ;
    } else {
        return TRUE ;
    }
}


uint32_t no_blks(struct PAGE_INFO *ma)
{
    if (ma->blk_head_ptr == NULL) {
        return TRUE ;
    } else {
        return FALSE ;
    }
}


/****************************************************************************************/
// alloc 小塊記憶體相關function
// 都沒空間的話,要先呼叫 
// page_list_init()
// page_alloc()
// memblks_init();
// blk_alloc() ;
/****************************************************************************************/
void *demand_a_blk()
{
    struct PAGE_INFO *ma ;
    // 尚未 alloc 一個 mem page
    if (atleast_a_page_alloc() == FALSE) {
        ma = page_alloc() ;
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *p = blk_alloc(ma);
        return p ;
    }

    //已經至少alloc過一個area
    ma = find_aval_inuse_page();

    //沒有可用的 mem page
    //預設1個block 64bytes(前4個用作blks linklist指標)
    if (ma == NULL) {
        ma = page_alloc() ;
        ma = memblks_init(ma
                            ,DEFAULT_AVAL_BLK_SIZE 
                            ,DEFAULT_TASK_MA_BLKNUM) ;

        void *p = blk_alloc(ma);
        return p ;
    }

    //有可用的 mem page
    //判斷有無 init blocks
    if (is_blk_init(ma) == FALSE) {
        ma = memblks_init(ma
                            ,DEFAULT_AVAL_BLK_SIZE 
                            ,DEFAULT_TASK_MA_BLKNUM) ;

        void *p = blk_alloc(ma);
        return p ;       
    }

    void *p = blk_alloc(ma);
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