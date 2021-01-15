
#include "memory.h"

uint32_t *kernal_end = &_end ;

void cleanPageConntent(void *start)
{
    uint32_t *startPtr = (uint32_t *)start ;

    for(uint32_t i=0 ;i<1024 ;i++)
    {
        *startPtr = 0 ;
        startPtr += 1 ;
    }
}





