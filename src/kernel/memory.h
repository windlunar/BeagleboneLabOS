

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../common.h"
#include "../driver/uart.h"
#include "../klib/print.h"
#include "../klib/mem.h"

extern uint32_t _end ;	//_end is define in linker.ld
extern uint32_t *kernal_end ;

#define PART_SIZE	        4096	//bytes
#define KB_SIZE             1024
#define MB_SIZE             1024 * 1024
#define VALUABLE_MEM_SIZE   128 * 1024 * 1024  
#define TOTAL_PART_NUM      VALUABLE_MEM_SIZE/4096  //32*1024個parts

// 如果第一個part的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define FIRST_PART_ADDR     (ROUNDUP_4K((uint32_t)kernal_end ,PART_SIZE))
#define FIRST_PART_PTR      (uint32_t *)FIRST_PART_ADDR     

/***********************************************************************************************/
// Structs
/***********************************************************************************************/
// Define part_status
#define FREE    0
#define INUSE_PARTIALLY_FREE    1
#define INUSE_FULL  2


//將 PART_INFO_STRUCT_t 串起來
typedef struct
{
    uint8_t block_size ;    //單位為 bytes ,最大256個bytes
    uint16_t block_num ;    //一個part block數量 ,最多16*256個(一個part的byte大小)
    
}BLOCK_INFO_t;

// 如果是last node ,則 nextnode_ptr = NULL
// next_ptr 指向下一個node address
// 
// 該結構體總共 12個bytes ,1個part可以容納 341個結構體
// 共有 32 * 1024個 parts(128MB) ,
// 所以需要 32 * 1024 * 12 bytes的空間存放 所有的 PART_INFO結構體
// 這樣需要 (32 * 1024 * 12)/(4 * 1024) = 96 個 parts來存放
struct PART_INFO{
    struct PART_INFO *next_ptr ;
    struct PART_INFO *prev_ptr ;
    uint32_t part_status;
    uint32_t part_id ;
    uint32_t *part_mem_start_ptr ;

    BLOCK_INFO_t *block_info_ptr ; //part中block的info     
};

typedef struct PART_INFO PART_INFO_t ;
extern PART_INFO_t *free_part_list_head;
extern PART_INFO_t *inuse_part_list_head;
extern PART_INFO_t parts_list[TOTAL_PART_NUM] ;

/***********************************************************************************************/
// Functions
/***********************************************************************************************/

void mem_parts_list_init();
PART_INFO_t *alloc_one_mem_part(void);
void free_part_mem(PART_INFO_t *part_node);
void add_to_free_list_end(PART_INFO_t *part_node);
void insert_to_inuse_list(PART_INFO_t *part_node);
void delete_from_inuse_list(PART_INFO_t *part_node);
void clean_part_mem_content(void *start);



#endif