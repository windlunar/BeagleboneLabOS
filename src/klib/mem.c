
#include "mem.h"
#include "print.h"

void _memset(void *start ,uint8_t value ,uint32_t SizeofBytes)
{
    uint8_t *startPtr = (uint8_t *)start ;

    for(uint32_t i=0 ;i<SizeofBytes ;i++)
    {
        *startPtr = value ;
        startPtr += 1 ;
    }
}


void _showMem(void *start ,uint32_t SizeofBytes)
{
    uint8_t *startPtr = (uint8_t *)start ;

    kprintf("\r\nShow memory content:");
    for(uint32_t i=0 ;i<SizeofBytes ;i++)
    {   
        if(i % 4 == 0)
        {
            kprintf("\r\n") ;
            kprintf("%x : ",startPtr) ;
        }
        
        kprintf("%x  ",*startPtr) ;
        startPtr += 1 ;
    }    
}