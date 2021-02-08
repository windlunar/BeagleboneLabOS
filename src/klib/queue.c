/**
 * Queue Implementation
 * 
 */ 

#include "queue.h"
#include "../kernel/memory.h"
#include "mem.h"


int kq_init(struct QUEUE *qPtr ,int qSize)
{
    qPtr->head = 0 ;
    qPtr->end = 0 ;
    qPtr->n_data = 0 ;

    qPtr->data = kblk_alloc(FOR_QUEUE) ; //16KB
    if (qPtr->data == NULL)
        return -1 ;

    qPtr->size = qSize;
    _memset((void *)(qPtr->data) ,0 ,qSize * sizeof(char)) ;

    return 0 ;
}



void kq_delete(struct QUEUE *qPtr)
{
    kblk_free(qPtr->data) ;
}



int queue_is_empty(struct QUEUE *qPtr)
{
    if (qPtr->n_data == 0) {
        return 1;
    } else {
        return 0 ;
    }
}



int queue_is_full(struct QUEUE *qPtr)
{
    if (qPtr->n_data == qPtr->size) {
        return 1;
    } else {
        return 0 ;
    }
}



int enqueue(struct QUEUE *qPtr, uint8_t byte)
{
    if (queue_is_full(qPtr) == 1) {
        printk("Warning : Can't not put data to queue. Queue is Full. Return -1.\r\n");
        return -1 ;
    }

    qPtr->data[qPtr->end] = byte ;
    qPtr->end = ( (qPtr->end) + 1 ) % (qPtr->size) ;
    qPtr->n_data++ ;
    return 0 ;
}



uint8_t dequeue(struct QUEUE *qPtr)
{
    if (queue_is_empty(qPtr) == 1) {
        printk("Warning : Can't not get data from queue. Queue is Empty. Return -1.\r\n");
        return -1 ;
    }

    uint8_t data = qPtr->data[qPtr->head] ;
    qPtr->data[qPtr->head] = 0 ;
    qPtr->n_data--;

    qPtr->head = ( (qPtr->head) + 1 ) % (qPtr->size) ;
    return data ;
}



void dump_queue (struct QUEUE *qPtr)
{
    if(qPtr == NULL)
        printk("qPtr is Null.\r\n");
        
    int idx = qPtr->head ;
    printk("\r\n++++++++++++++++++++++++++++++++++\r\n");
    if (queue_is_empty(qPtr) == 1) {
        printk("No data in queue.\r\n");
    } else {
        
        printk("Print data from head.\r\n");
        for (int i = 0 ; i<qPtr->n_data ;i++) {
            printk("queue[%d] : %c\r\n",idx,qPtr->data[idx]);
            idx = (idx + 1) % (qPtr->size);
        }
        
    }
    printk("++++++++++++++++++++++++++++++++++\r\n");
}