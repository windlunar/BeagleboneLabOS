

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
#define VALUABLE_MEM_SIZE   128 * 1024 * 1024  //128 MB
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


// 如果是last node ,則 next_ptr = NULL
// next_ptr 指向下一個node address
struct PART_INFO{
    struct PART_INFO *next_ptr ;
    struct PART_INFO *prev_ptr ;
    uint32_t part_status;
    uint32_t part_id ;
    uint32_t *part_mem_start_ptr ;

    uint8_t *available_ptr ; //在 page中可用的起始位址
};

typedef struct PART_INFO MEM_PART_INFO_t ;
extern MEM_PART_INFO_t *free_part_list_head;
extern MEM_PART_INFO_t *inuse_part_list_head;
extern MEM_PART_INFO_t parts_list[TOTAL_PART_NUM] ;

/***********************************************************************************************/
// Functions
/***********************************************************************************************/

void mem_parts_list_init();
MEM_PART_INFO_t *alloc_one_mem_part(void);
void free_part_mem(MEM_PART_INFO_t *part_node);
void add_to_free_list_end(MEM_PART_INFO_t *part_node);
void insert_to_inuse_list(MEM_PART_INFO_t *part_node);
void delete_from_inuse_list(MEM_PART_INFO_t *part_node);
void clean_part_mem_content(void *start);

/***********************************************************************************************/
// alloc 小塊記憶體相關function
// 感覺應該要在把1個part的記憶體空間在切成一小塊一小塊的block
/***********************************************************************************************/
void *kmalloc(uint32_t size_in_bytes);
void kfree(void *p);

#endif