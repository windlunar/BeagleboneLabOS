
#include "../klib/mem.h"
#include "../kernel/printk.h"
#include "../kernel/memory.h"

/*************************************************************************/

void _memset(void *start ,uint8_t value ,uint32_t SizeofBytes)
{
    uint8_t *startPtr = (uint8_t *)start ;

    for (uint32_t i=0 ;i<SizeofBytes ;i++) {
        *startPtr = value ;
        startPtr += 1 ;
    }
}



void _showmm(void *start ,uint32_t SizeofBytes)
{
    uint8_t *startPtr = (uint8_t *)start ;

    printk("\r\nShow memory content:");
    for (uint32_t i=0 ;i<SizeofBytes ;i++) {   
        if (i % 4 == 0) {
            printk("\r\n") ;
            printk("%x : ",startPtr) ;
        }
        
        printk("%x  ",*startPtr) ;
        startPtr += 1 ;
    }    
}



void _memcpy(void *dest ,void *src ,uint32_t nbytes)
{
    uint8_t *d = (uint8_t *)dest ;
    uint8_t *s = (uint8_t *)src ;
    while (nbytes != 0) {
        *d = *s ;
        d++ ;
        s++ ;
        nbytes-- ;
    }
}