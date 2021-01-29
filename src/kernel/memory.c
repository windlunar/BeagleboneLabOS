
#include "memory.h"
#include "task.h"

uint32_t *kernal_end = &_end ;

MEM_AREA_INFO_t *free_area_list_head = NULL;
MEM_AREA_INFO_t *inuse_area_list_head = NULL;
MEM_AREA_INFO_t areas_list[TOTAL_AREA_NUM] ;   //存放在 kernel binary的bss segment中

/***********************************************************************************************/
// Memory Area Functions
/***********************************************************************************************/
//
// 將所有memory area (1 area 其實就是一個page的大小)用link list串起來
// A memoy Areas
//    
//  |-------|
//  | area  | 
//  |-------|
//  | area  |
//  |-------|
//  | area  |  
//  |-------|
//  |       |
//
void mem_areas_list_init()
{
    free_area_list_head = &areas_list[0] ;
    
    for(int32_t i=0; i<TOTAL_AREA_NUM ;i++)
    {
        areas_list[i].area_status = FREE ; 
        areas_list[i].area_id = i ;
        areas_list[i].m_start = FIRST_AREA_PTR + i*(AREA_SIZE/4) ;
        areas_list[i].blk_head_ptr = (uint32_t *)areas_list[i].m_start ;
        areas_list[i].m_top = areas_list[i].m_start + (AREA_SIZE/4) -1 ;

        //0 means blks not init
        areas_list[i].blksize = 0 ; 
        areas_list[i].n_blk = 0 ; 

        if(i == TOTAL_AREA_NUM-1){
            areas_list[i].next_ptr = NULL ;
        }else{
            areas_list[i].next_ptr = &areas_list[i+1] ;
        }
        

        if(i == 0){
            areas_list[i].prev_ptr = NULL ;
        }else{
            areas_list[i].prev_ptr = &areas_list[i-1] ;
        }
    }   
}


MEM_AREA_INFO_t *memAreaAlloc(void)
{
    //保存原有的 head node
    MEM_AREA_INFO_t *prev_head = free_area_list_head ;
    //修改現在的 head_ptr
    free_area_list_head = prev_head->next_ptr ;
    free_area_list_head->prev_ptr = NULL ;

    //設定原來的 head node
    prev_head->next_ptr =NULL ;
    prev_head->area_status = INUSE_PARTIALLY_FREE ;

    insert_to_inuse_list(prev_head) ;

    //回傳原來 head area的結構指標
    return prev_head ;
}


void free_mem_area(MEM_AREA_INFO_t *area_node)
{
    if(area_node->area_status != FREE ){
        delete_from_inuse_list(area_node) ;
        add_to_free_list_end(area_node) ;
    }else{
        kprintf("The area info node is already in free list.\r\n") ;
    }
}


void add_to_free_list_end(MEM_AREA_INFO_t *area_node)
{
    //如果還沒有 head node
    if(free_area_list_head == NULL){
        free_area_list_head = area_node ;
        area_node->next_ptr = NULL ;
        return ;
    }

    MEM_AREA_INFO_t *curr_head = free_area_list_head ;

    /** Move pointer from head Node to End node */
    while(curr_head->next_ptr != NULL){
        curr_head = curr_head->next_ptr ;
    }
    MEM_AREA_INFO_t *end_ptr = curr_head ;

    end_ptr->next_ptr = area_node ;
    area_node->next_ptr = NULL ;
    area_node->prev_ptr = end_ptr ;

}


void insert_to_inuse_list(MEM_AREA_INFO_t *area_node)
{
    //如果還沒有 head node
    if(inuse_area_list_head == NULL){
        inuse_area_list_head = area_node ;
        area_node->next_ptr = NULL ;
        area_node->prev_ptr = NULL ;
        return ;
    }

    MEM_AREA_INFO_t *curr_head = inuse_area_list_head ;

    /** Move pointer from head Node to End node */
    while(curr_head->next_ptr != NULL){
        curr_head = curr_head->next_ptr ;
    }
    MEM_AREA_INFO_t *end_ptr = curr_head ;

    end_ptr->next_ptr = area_node ;
    area_node->next_ptr = NULL ;
    area_node->prev_ptr = end_ptr ;

}


void delete_from_inuse_list(MEM_AREA_INFO_t *area_node)
{

    MEM_AREA_INFO_t *prev_node = area_node->prev_ptr ;
    MEM_AREA_INFO_t *next_node = area_node->next_ptr ;

    //如果 area_node是head ,next node變成 head
    if(prev_node == NULL){
        inuse_area_list_head = next_node ;
        next_node->prev_ptr = NULL ;
        area_node->next_ptr = NULL ;
        return ;
    }

    // 如果 area_node是end ,prev node變成end
    if(next_node == NULL){
        prev_node->next_ptr = NULL ;
        area_node->prev_ptr = NULL ;
        return ;
    }

    prev_node->next_ptr = next_node ;
    next_node->prev_ptr = prev_node ;
    area_node->next_ptr = NULL ;
    area_node->prev_ptr = NULL ;

}


void clean_mem_area_content(void *start)
{
    uint32_t *startPtr = (uint32_t *)start ;

    for(uint32_t i=0 ;i<(AREA_SIZE/4) ;i++)
    {
        *startPtr = 0 ;
        startPtr += 1 ;
    }
}


uint32_t atleast_a_memarea_alloc(void)
{
    if(inuse_area_list_head == NULL){
        return FALSE ;
    }else{
        return TRUE ;
    }
}

MEM_AREA_INFO_t *find_ma_end(MEM_AREA_INFO_t *headnode)
{
    MEM_AREA_INFO_t *head =headnode ;
    while(head != NULL){
        head = head->next_ptr ;
    }
    return head ;
}


MEM_AREA_INFO_t *find_aval_inuse_memarea(void)
{
    MEM_AREA_INFO_t *head = inuse_area_list_head ;

    while(head != NULL){
        if(head->area_status = INUSE_PARTIALLY_FREE){
            return head;
        }
        head = head->next_ptr ;
    }
    return NULL ;
}

/***********************************************************************************************/
//Blocks
//    A memoy Area
//  |-------|<-
//  | blk   | ^
//  |-------|<-->
//  | blk   | ^
//  |-------|<-->
//  | blk   | ^   每個blk前四個byte為一個pointer, 指向下一個blk的起始位址 ,形成一個link list
//  |-------|->
//  |       |
/***********************************************************************************************/
// Need to alloc a mem area fitst
// (call memAreaAlloc() first)
// arg1 : 已經allocate的memory area (memAreaAlloc()的回傳值)
// arg2 : number of bytes
MEM_AREA_INFO_t 
*memblks_init(MEM_AREA_INFO_t *ma ,uint32_t blk_aval_size ,uint32_t num_blks)
{
    //已經初始化過
    if(ma->blksize >4) return ma ;
    if(blk_aval_size <= 0) NULL ;

    uint32_t blk_size = blk_aval_size + 4 ;

    //一個blk最少要四個bytes ,前四個byte放指標 ,指向下一個可用的blk
    ma->blksize = blk_size ;
    ma->n_blk = num_blks ;

    uint32_t *head = ma->blk_head_ptr ;
    uint32_t node_addr = (uint32_t)head ;
    
    for(int32_t i = 1 ; i<=num_blks; i++)
    {
        //the last blk
        if(i == num_blks){
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
// mem_areas_list_init() -> memAreaAlloc() -> memblks_init() 
// -> blk_alloc()
void *blk_alloc(MEM_AREA_INFO_t *ma)
{
    // 如果 ma是 task ,且task還有空間為分配, 那就先從task area分配一個blk
    if( (ma->area_status == TASK_AREA) && (no_blks(ma) == FALSE) )
    {
        void *ret = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ret ;          
    }

    if(ma==NULL)
    {
        ma = memAreaAlloc();
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *ptr_return = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ma->blk_head_ptr+1 ;
    }

    if(ma->blksize <= 4)
    {
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *ptr_return = (void *)(ma->blk_head_ptr + 1) ;
        ma->blk_head_ptr = (uint32_t *)*ma->blk_head_ptr ;

        return ma->blk_head_ptr+1 ;        
    }

    if(ma->blk_head_ptr == NULL){
        ma->area_status = INUSE_FULL ;

        ma = memAreaAlloc();
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


MEM_AREA_INFO_t *which_mem_area(void *address)
{
    uint32_t *memarea = (uint32_t *)ROUNDDOWN((uint32_t)address ,AREA_SIZE) ;

    //判斷 memarea這個 addr是那個area
    MEM_AREA_INFO_t *area_head = inuse_area_list_head ;

    while(area_head->next_ptr != NULL)
    {
        if(area_head->m_start == memarea) break ;
        area_head = area_head->next_ptr ;
    }
    MEM_AREA_INFO_t *target = area_head ;

    return target ;
}


// 找前一個 blk空間 (不是link list的 prev node)
// 目前沒有用到的函式
uint32_t *find_prev_blk(MEM_AREA_INFO_t *memarea ,uint32_t *blk_start)
{
    uint32_t * prevblk = (uint32_t *)((uint8_t *)blk_start - memarea->blksize) ;

    return prevblk ;
}



void put_to_blklist_end(MEM_AREA_INFO_t *ma ,uint32_t *blkstart)
{
    uint32_t *head = ma->blk_head_ptr ;
    while(*head != 0){
        head = (uint32_t *)*head ;
    }

    uint32_t *end = head ;
    *end = (uint32_t)blkstart ;

    *blkstart = 0 ;
}


void free_blk(void *blk_aval_start)
{
    uint32_t *blk_start= (uint32_t *)blk_aval_start -1 ;
    MEM_AREA_INFO_t *ma = which_mem_area(blk_start) ;

    //將原本的blk放到list最後
    put_to_blklist_end(ma ,blk_start) ;
}

uint32_t is_blk_init(MEM_AREA_INFO_t *ma)
{
    if(ma->blksize <= 4){
        return FALSE ;
    }else{
        return TRUE ;
    }
}


uint32_t no_blks(MEM_AREA_INFO_t *ma)
{
    if(ma->blk_head_ptr == NULL){
        return TRUE ;
    }else{
        return FALSE ;
    }
}


/***********************************************************************************************/
// alloc 小塊記憶體相關function
// 都沒空間的話,要先呼叫 
// mem_areas_list_init()
// memAreaAlloc()
// memblks_init();
// blk_alloc() ;
/***********************************************************************************************/
void *demand_a_blk()
{
    MEM_AREA_INFO_t *ma ;
    // 尚未 alloc 一個 mem area
    if(atleast_a_memarea_alloc() == FALSE)
    {
        ma = memAreaAlloc() ;
        ma = memblks_init(ma
                        ,DEFAULT_AVAL_BLK_SIZE 
                        ,DEFAULT_TASK_MA_BLKNUM) ;

        void *p = blk_alloc(ma);
        return p ;
    }

    //已經至少alloc過一個area
    ma = find_aval_inuse_memarea();

    //沒有可用的 mem area
    //預設1個block 64bytes(前4個用作blks linklist指標)
    if(ma == NULL){
        ma = memAreaAlloc() ;
        ma = memblks_init(ma
                            ,DEFAULT_AVAL_BLK_SIZE 
                            ,DEFAULT_TASK_MA_BLKNUM) ;

        void *p = blk_alloc(ma);
        return p ;
    }

    //有可用的 mem area
    //判斷有無 init blocks
    if(is_blk_init(ma) == FALSE)
    {
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

/***********************************************************************************************/