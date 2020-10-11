

#include "debug.h"


void print_LR(uint32 data){
    kprintf("LR : %x\r\n", data) ;
}

void print_SP(uint32 data){
    kprintf("SP : %x\r\n", data) ;
}

void stackDisplay(uint32 stackTop ,int num){
    uint32 *stackTopAddr = (uint32 *)stackTop ;
    for(int i = 0 ; i < num ; i++){
        kprintf("MEM ADDR : %p --- CONTENT : %x\r\n", stackTopAddr-i,*(stackTopAddr-i)) ;
    }
}