

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


int32_t readCpsrMode(){
    int32_t cpsr = readCpsr() ;
    int32_t mode = cpsr & 0x1F ;

    switch (mode)
    {
    case 0x10:
        kprintf("In user mode. cpsr mode :%x\r\n",mode) ;
        break;
    case 0x1f:
        kprintf("In system mode. cpsr mode :%x\r\n",mode) ;
        break;
    case 0x12:
        kprintf("In irq mode. cpsr mode :%x\r\n",mode) ;
        break;
    case 0x13:
        kprintf("In kernel mode. cpsr mode :%x\r\n",mode) ;
        break;
    default:
        break;
    }

    return mode ;
}


void print_R0_R1_R2_R3(uint32_t r0 ,uint32_t r1 ,uint32_t r2 ,uint32_t r3){
    kprintf("R0 : %x---R1 : %x---R2 : %x---R3 : %x\r\n", r0,r1,r2,r3) ;
}


void print_free_part_list_from_head()
{
    MEM_PART_INFO_t *head = free_part_list_head ;

    //Just print first 10 nodes for test
    kprintf("Print free list from head\r\n");
    for(int i =0 ;i<10;i++){
        kprintf("%p  ",head->part_mem_start_ptr) ;
        kprintf("mem part id:%d  ",head->part_id) ;
        kprintf("mem part status:%d\r\n",head->part_status) ;
        head = head->next_ptr ;
    }
}


void print_free_part_list_from_end()
{
    MEM_PART_INFO_t *head = free_part_list_head ;

    while(head->next_ptr != NULL){
        head = head->next_ptr ;
    }
    MEM_PART_INFO_t *end = head ;

    //Just print first 10 nodes for test
    kprintf("Print free list from end\r\n");
    for(int i =0 ;i<10;i++){
        kprintf("%p  ",end->part_mem_start_ptr) ;
        kprintf("mem part id:%d  ",end->part_id) ;
        kprintf("mem part status:%d\r\n",end->part_status) ;
        end = end->prev_ptr ;
    }
}


void print_inuse_part_list_from_head()
{
    MEM_PART_INFO_t *head = inuse_part_list_head ;

    //Just print first 10 nodes for test
    kprintf("Print inuse list\r\n");
    for(int i =0 ;i<10;i++){
        kprintf("%p  ",head->part_mem_start_ptr) ;
        kprintf("mem part id:%d  ",head->part_id) ;
        kprintf("mem part status:%d\r\n",head->part_status) ;

        if(head->next_ptr == NULL) break ;
        head = head->next_ptr ;
    }
}

void mem_part_alloc_free_test()
{
	print_free_part_list_from_head();

	kprintf("Test alloc\r\n") ;

	MEM_PART_INFO_t *mem_part0 = alloc_one_mem_part() ;
	MEM_PART_INFO_t *mem_part1 = alloc_one_mem_part() ;
	MEM_PART_INFO_t *mem_part2 = alloc_one_mem_part() ;

	print_free_part_list_from_head();
	print_inuse_part_list_from_head();

	kprintf("Test free\r\n") ;

	free_part_mem(mem_part0) ;
	free_part_mem(mem_part1) ;

	print_free_part_list_from_end();
	print_inuse_part_list_from_head();   

    uint32_t *p= kmalloc(4) ;
    kprintf("Part's start addr =%p\r\n" ,mem_part2->part_mem_start_ptr) ;
    kprintf("Part's blk_head_ptr addr =%p\r\n",mem_part2->blk_head_ptr) ;

}


void mem_part_blk_init_test()
{
	print_free_part_list_from_head();

	kprintf("Test alloc\r\n") ;

	MEM_PART_INFO_t *mem_part = alloc_one_mem_part() ;

	print_free_part_list_from_head();
	print_inuse_part_list_from_head();

    memblks_init(mem_part ,28) ;
    kprintf("Part's blk_head_ptr addr =%p\r\n",mem_part->blk_head_ptr) ;

    uint32_t *head = mem_part->blk_head_ptr ;
    while(head != NULL)
    {
        kprintf("Part's blks link addr =%p ,%x\r\n",head ,*head) ;
        head = (uint32_t *)*head ;
    }
    kprintf("Part's blks link addr =%p\r\n",head) ;

    uint32_t *p1 = blk_alloc(mem_part) ;
    kprintf("return ptr =%p\r\n" ,p1) ;
    kprintf("blk head =%p\r\n",mem_part->blk_head_ptr) ;

    uint32_t *p2 = blk_alloc(mem_part) ;
    kprintf("return ptr =%p\r\n" ,p2) ;
    kprintf("blk head =%p\r\n",mem_part->blk_head_ptr) ;


    kprintf("Test free_blk\r\n") ;
    free_blk(p2) ;
    print_from_blk_head(mem_part) ;

}


void print_from_blk_head(MEM_PART_INFO_t *mem_part)
{
    uint32_t *blkstart = mem_part->blk_head_ptr ;

    while(*blkstart != 0)
    {
        kprintf("addr=%p ,content=%x\r\n",blkstart ,*blkstart) ;
        blkstart = (uint32_t *)*blkstart ;
    }
}