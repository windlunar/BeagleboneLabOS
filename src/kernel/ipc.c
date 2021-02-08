

#include "ipc.h"
#include "../driver/uart.h"

struct QUEUE q ;

int ipc_buf_alloc()
{
    if (kq_init(&q ,128) != 0)
        return -1 ;
    else
        return 0 ;
}

int ipc_read_func(uint8_t *rbuf ,uint32_t n_bytes)
{
    uint8_t *s = rbuf ;
    int n_rd = 0;

    while (n_bytes != 0) {
        *s = dequeue(&q) ;
        s++ ;
        n_rd++ ;
        n_bytes-- ;
    }
    return n_rd ;
}

int ipc_write_func(uint8_t *wbuf ,uint32_t n_bytes)
{
    uint8_t *s = wbuf ;
    int n_wr = 0 ;
    while (n_bytes != 0) {
        if (enqueue(&q ,*s) != 0)
            return -1 ;

        s++ ;
        n_wr++ ;
        n_bytes-- ;
    }

    return 0 ;
}

// Not close
void free_ipc(void)
{
    kq_delete(&q) ;
}
