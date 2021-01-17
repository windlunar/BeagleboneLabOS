/**
 * Queue Implementation
 * 
 * Date : 2020/11/25
 * Author : Yi-Ying Lin
 */ 

#include "queue.h"




/** 初始化queue
 * argument 1 : 指向 Queue結構的pointer
 * argument 2 : queue最大可存放的元素數 
 * 作用 : 將頭尾的index,queue內的元素數目初始化為0, 分配一個記憶體空間給queue存放data
 * 回傳值 : 無
 */
//還沒實做malloc與memset 
void queueInit(QUEUE_TASK_t *qPtr ,int qSize)
{
    qPtr->idxHead = 0 ;
    qPtr->idxBack = 0 ;
    qPtr->NumDataInQueue = 0 ;

    qPtr->size = qSize;

    for(int32_t i =0 ;i<qSize ;i++) qPtr->qDataTaskStructPtr[i] = NULL ; 
}

/** 
 * argument 1 : 指向 Queue結構的pointer
 * 作用 : 釋放 在queueInit()中透過 malloc分配的記憶體空間
 * 回傳值 : 無
*/
void queueDelete(QUEUE_TASK_t *qPtr){
    //Not Implement Yet
    //free(qPtr->qDataTaskStructPtr);
}


/** 
 * argument 1 : 指向 Queue結構的pointer
 * 作用 : 判斷該queue是否為 empty
 * 回傳值 : 回傳1, 代表 queue為空沒有data, 反之回傳 0
*/
int32_t queueIsEmpty(QUEUE_TASK_t *qPtr){
    if(qPtr->NumDataInQueue == 0){
        return 1;
    }else{
        return 0 ;
    }
}


/** 
 * argument 1 : 指向 Queue結構的pointer
 * 作用 : 判斷該queue是否 full
 * 回傳值 : 回傳1, 代表 queue為 full, 反之回傳 0
*/
int32_t queueIsFull(QUEUE_TASK_t *qPtr){
    if(qPtr->NumDataInQueue == qPtr->size){
        return 1;
    }else{
        return 0 ;
    }
}


/** 
 * argument 1 : 指向 Queue結構的pointer
 * argument 2 : 欲存放進queue的 data 
 * 作用 : 將data從尾部放入queue
 * 回傳值 : 0代表成功, -1代表失敗(因為queue已經滿了)
*/
int32_t enQueue(QUEUE_TASK_t *qPtr, USERTASK_t *data)
{
    if(queueIsFull(qPtr) == 1){
        kprintf("Warning : Can't not put data to queue. Queue is Full. Return -1.\r\n");
        return -1 ;
    }

    qPtr->qDataTaskStructPtr[qPtr->idxBack] = data ;
    qPtr->idxBack = ( (qPtr->idxBack)+1 ) % (qPtr->size) ;
    qPtr->NumDataInQueue++ ;
    return 0 ;
}


/** 
 * argument 1 : 指向 Queue結構的pointer
 * 作用 : 將data從頭部取出
 * 回傳值 : 0代表成功, -1代表失敗(因為queue為空)
*/
USERTASK_t *deQueue(QUEUE_TASK_t *qPtr)
{
    if(queueIsEmpty(qPtr) == 1){
        kprintf("Warning : Can't not get data from queue. Queue is Empty. Return -1.\r\n");
        return NULL ;
    }

    USERTASK_t *data = qPtr->qDataTaskStructPtr[qPtr->idxHead] ;
    qPtr->qDataTaskStructPtr[qPtr->idxHead] = NULL ;
    qPtr->NumDataInQueue--;

    qPtr->idxHead = ( (qPtr->idxHead)+1 ) % (qPtr->size) ;
    return data ;
}



/** 
 * argument 1 : 指向 Queue結構的pointer
 * 作用 : 從頭到尾將queue存放的data印出來
 * 回傳值 : 無
*/
void printDataInQueue(QUEUE_TASK_t *qPtr){
    int idx = qPtr->idxHead ;

    kprintf("\n++++++++++++++++++++++++++++++++++\r\n");
    kprintf("Print data from head.\r\n");

    if(queueIsEmpty(qPtr) == 1){
        kprintf("No data in queue.\r\n");
    }else{
        for(int i = 0 ; i<qPtr->NumDataInQueue ;i++){

            kprintf("queue[%d] : id=%d ,status=%d\r\n",idx
            ,qPtr->qDataTaskStructPtr[idx]->taskID
            ,qPtr->qDataTaskStructPtr[idx]->taskStatus);

            idx = (idx+1) % (qPtr->size);
        }

        kprintf("queue[%d] is the back ,id=%d ,status=%d\r\n" 
        ,qPtr->idxBack
        ,qPtr->qDataTaskStructPtr[qPtr->idxBack]->taskID
        ,qPtr->qDataTaskStructPtr[idx]->taskStatus);
        
    }
    kprintf("++++++++++++++++++++++++++++++++++\r\n\r\n");
}