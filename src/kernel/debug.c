

#include "debug.h"


void print_LR(uint32_t data){
    kprintf("LR : %x\r\n", data) ;
    for(;;);
}

void print_SP(uint32_t data){
    kprintf("SP : %x\r\n", data) ;
    //for(;;);
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


void print_R0_R1_R2_R3(uint32_t r0 ,uint32_t r1 ,uint32_t r2 ,uint32_t r3){
    kprintf("R0 : %x---R1 : %x---R2 : %x---R3 : %x\r\n", r0,r1,r2,r3) ;
}