
#include "memory.h"

uint32_t *kernal_end = &_end ;

PART_INFO_t *free_part_list_head = NULL;
PART_INFO_t *inuse_part_list_head = NULL;
PART_INFO_t parts_list[TOTAL_PART_NUM] ;   //存放在 kernel binary的bss segment中

/***********************************************************************************************/
// Functions
/***********************************************************************************************/
void mem_parts_list_init()
{
    free_part_list_head = &parts_list[0] ;
    
    for(int32_t i=0; i<TOTAL_PART_NUM ;i++)
    {
        parts_list[i].block_info_ptr = NULL ;
        parts_list[i].part_status = FREE ; 
        parts_list[i].part_id = i ;
        parts_list[i].part_mem_start_ptr = FIRST_PART_PTR + i*1024 ;


        if(i == TOTAL_PART_NUM-1){
            parts_list[i].next_ptr = NULL ;
        }else{
            parts_list[i].next_ptr = &parts_list[i+1] ;
        }
        

        if(i == 0){
            parts_list[i].prev_ptr = NULL ;
        }else{
            parts_list[i].prev_ptr = &parts_list[i-1] ;
        }
    }   
}


PART_INFO_t *alloc_one_mem_part(void)
{
    //保存原有的 head node
    PART_INFO_t *prev_head = free_part_list_head ;
    //修改現在的 head_ptr
    free_part_list_head = prev_head->next_ptr ;
    free_part_list_head->prev_ptr = NULL ;

    //設定原來的 head node
    prev_head->next_ptr =NULL ;
    prev_head->part_status = INUSE_PARTIALLY_FREE ;

    insert_to_inuse_list(prev_head) ;

    //回傳原來 head part的記憶體位址
    return prev_head ;
}


void free_part_mem(PART_INFO_t *part_node)
{
    if(part_node->part_status != FREE ){
        delete_from_inuse_list(part_node) ;
        add_to_free_list_end(part_node) ;
    }else{
        kprintf("The part info node is already in free list.\r\n") ;
    }
}


void add_to_free_list_end(PART_INFO_t *part_node)
{
    //如果還沒有 head node
    if(free_part_list_head == NULL){
        free_part_list_head = part_node ;
        part_node->next_ptr = NULL ;
        return ;
    }

    PART_INFO_t *curr_head = free_part_list_head ;

    /** Move pointer from head Node to End node */
    while(curr_head->next_ptr != NULL){
        curr_head = curr_head->next_ptr ;
    }
    PART_INFO_t *end_ptr = curr_head ;

    end_ptr->next_ptr = part_node ;
    part_node->next_ptr = NULL ;
    part_node->prev_ptr = end_ptr ;

}


void insert_to_inuse_list(PART_INFO_t *part_node)
{
    //如果還沒有 head node
    if(inuse_part_list_head == NULL){
        inuse_part_list_head = part_node ;
        part_node->next_ptr = NULL ;
        part_node->prev_ptr = NULL ;
        return ;
    }

    PART_INFO_t *curr_head = inuse_part_list_head ;

    /** Move pointer from head Node to End node */
    while(curr_head->next_ptr != NULL){
        curr_head = curr_head->next_ptr ;
    }
    PART_INFO_t *end_ptr = curr_head ;

    end_ptr->next_ptr = part_node ;
    part_node->next_ptr = NULL ;
    part_node->prev_ptr = end_ptr ;

}


void delete_from_inuse_list(PART_INFO_t *part_node)
{

    PART_INFO_t *prev_node = part_node->prev_ptr ;
    PART_INFO_t *next_node = part_node->next_ptr ;

    //如果 part_node是head ,next node變成 head
    if(prev_node == NULL){
        inuse_part_list_head = next_node ;
        next_node->prev_ptr = NULL ;
        part_node->next_ptr = NULL ;
        return ;
    }

    // 如果 part_node是end ,prev node變成end
    if(next_node == NULL){
        prev_node->next_ptr = NULL ;
        part_node->prev_ptr = NULL ;
        return ;
    }

    prev_node->next_ptr = next_node ;
    next_node->prev_ptr = prev_node ;
    part_node->next_ptr = NULL ;
    part_node->prev_ptr = NULL ;

}


void clean_part_mem_content(void *start)
{
    uint32_t *startPtr = (uint32_t *)start ;

    for(uint32_t i=0 ;i<1024 ;i++)
    {
        *startPtr = 0 ;
        startPtr += 1 ;
    }
}




/***********************************************************************************************/



