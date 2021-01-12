

#include "debug.h"


void print_LR(uint32_t data){
    kprintf("LR : %x\r\n", data) ;
}

void print_SP(uint32_t data){
    kprintf("SP : %x\r\n", data) ;
}

void print_here(){
    kprintf("HERE\r\n") ;
}

void print_cpsr(){
    kprintf("cpsr : %x \r\n",READ_CPSR());
}

void stackDisplay(uint32_t stackTop ,int num){
    uint32_t *stackTopAddr = (uint32_t *)stackTop ;
    for(int i = 0 ; i < num ; i++){
        kprintf("MEM ADDR : %p --- CONTENT : %x\r\n", stackTopAddr-i,*(stackTopAddr-i)) ;
    }
}


int32_t readCpsr(){
	int32_t cpsr;

    asm volatile("mrs %[v], CPSR" : : [v] "r" (cpsr));
	return cpsr ;
}