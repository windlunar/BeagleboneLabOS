
#ifndef __MEM_H_
#define __MEM_H_

#include "../common.h"

void _memset(void *start ,uint8_t value ,uint32_t SizeofBytes);
void _showmm(void *start ,uint32_t SizeofBytes);
void _memcpy(void *dest ,void *src ,uint32_t nbytes);
#endif