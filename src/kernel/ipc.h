
#ifndef __IPC_H_
#define __IPC_H_

#include "../common.h"
#include "printk.h"
#include "../klib/queue.h"


extern struct QUEUE q ;


int ipc_buf_alloc() ;
int ipc_read_func(uint8_t *rbuf ,uint32_t n_bytes) ;
int ipc_write_func(uint8_t *wbuf ,uint32_t n_bytes) ;
void free_ipc(void) ;


#endif