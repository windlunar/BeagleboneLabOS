

#ifndef __MEMORY_H_
#define __MEMORY_H_

#include "../common.h"
#include "../driver/uart.h"
#include "../lib/print.h"
#include "../lib/mem.h"

extern uint32_t _end ;	//_end is define in linker.ld
extern uint32_t *kernal_end ;

#define PAGE_SIZE	        4096	//bytes
#define KB_SIZE             1024
#define MB_SIZE             1024 * 1024
#define VALUABLE_MEM_SIZE   128 * 1024 * 1024  
#define TOTAL_PAGE_NUM      VALUABLE_MEM_SIZE/4096  //32*1024個pages

// 如果第一個page的adde = 0x82004000
// 那128MB就是到 0x8a000000
#define FIRST_PAGE_ADDR     (ROUNDUP_4K((uint32_t)kernal_end ,PAGE_SIZE))
#define FIRST_PAGE_PTR      (uint32_t *)FIRST_PAGE_ADDR      
/**********************************************************************************************/
void cleanPageConntent(void *start);

#endif