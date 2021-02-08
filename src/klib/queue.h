/**
 * Queue Implementation
 * 
 */ 

#ifndef _QUEUE_H_
#define _QUEUE_H_


#include "stdio.h"
#include "string.h"
#include "../common.h"


struct QUEUE{
    int size ; 
    int head ;
    int end ;
    int n_data;
    uint8_t *data;
};


int kq_init(struct QUEUE *qPtr ,int qSize);
void kq_delete(struct QUEUE *qPtr);
int queue_is_empty(struct QUEUE *qPtr);
int queue_is_full(struct QUEUE *qPtr);
int enqueue(struct QUEUE *qPtr, unsigned char byte);
uint8_t dequeue(struct QUEUE *qPtr);
void dump_queue(struct QUEUE *qPtr);


#endif