

#include "debug.h"


void print_LR(uint32_t data){
    printk("LR : %x\r\n", data) ;
    for(;;);
}

void print_SP(uint32_t data){
    printk("SP : %x\r\n", data) ;
    //for(;;);
}

void print_here(){
    printk("HERE\r\n") ;
}

void print_cpsr(){
    printk("cpsr : %x \r\n",READ_CPSR());
}

void stackDisplay(uint32_t stackTop ,int num){
    uint32_t *stackTopAddr = (uint32_t *)stackTop ;
    for(int i = 0 ; i < num ; i++){
        printk("MEM ADDR : %p --- CONTENT : %x\r\n", stackTopAddr-i,*(stackTopAddr-i)) ;
    }
}



int32_t readCpsrMode(){
    int32_t cpsr = READ_CPSR() ;
    int32_t mode = cpsr & 0x1F ;

    switch (mode)
    {
    case 0x10:
        printk("In user mode. cpsr mode bits :%x\r\n",mode) ;
        break;
    case 0x1f:
        printk("In system mode. cpsr mode bits :%x\r\n",mode) ;
        break;
    case 0x12:
        printk("In irq mode. cpsr mode bits :%x\r\n",mode) ;
        break;
    case 0x13:
        printk("In kernel mode. cpsr mode bits :%x\r\n",mode) ;
        break;
    default:
        break;
    }

    return mode ;
}


void print_R0_R1_R2_R3(uint32_t r0 ,uint32_t r1 ,uint32_t r2 ,uint32_t r3){
    printk("R0 : %x---R1 : %x---R2 : %x---R3 : %x\r\n", r0,r1,r2,r3) ;
}


void print_free_area_list_from_head()
{
    MEM_AREA_INFO_t *head = free_area_list_head ;

    //Just print first 10 nodes for test
    printk("Print free list from head\r\n");
    for(int i =0 ;i<10;i++){
        printk("%p  ",head->m_start) ;
        printk("mem area id:%d  ",head->area_id) ;
        printk("mem area status:%d\r\n",head->area_status) ;
        head = head->next_ptr ;
    }
}


void print_free_area_list_from_end()
{
    MEM_AREA_INFO_t *head = free_area_list_head ;

    while(head->next_ptr != NULL){
        head = head->next_ptr ;
    }
    MEM_AREA_INFO_t *end = head ;

    //Just print first 10 nodes for test
    printk("Print free list from end\r\n");
    for(int i =0 ;i<10;i++){
        printk("%p  ",end->m_start) ;
        printk("mem area id:%d  ",end->area_id) ;
        printk("mem area status:%d\r\n",end->area_status) ;
        end = end->prev_ptr ;
    }
}


void print_inuse_area_list_from_head()
{
    MEM_AREA_INFO_t *head = inuse_area_list_head ;

    //Just print first 10 nodes for test
    printk("Print inuse list\r\n");
    for(int i =0 ;i<10;i++){
        printk("%p  ",head->m_start) ;
        printk("mem area id:%d  ",head->area_id) ;
        printk("mem area status:%d\r\n",head->area_status) ;

        if(head->next_ptr == NULL) break ;
        head = head->next_ptr ;
    }
}

void mem_area_alloc_free_test()
{
	print_free_area_list_from_head();

	printk("Test alloc\r\n") ;

	MEM_AREA_INFO_t *mem_area0 = alloc_mem_area() ;
	MEM_AREA_INFO_t *mem_area1 = alloc_mem_area() ;
	MEM_AREA_INFO_t *mem_area2 = alloc_mem_area() ;

	print_free_area_list_from_head();
	print_inuse_area_list_from_head();

	printk("Test free\r\n") ;

	free_mem_area(mem_area0) ;
	free_mem_area(mem_area1) ;

	print_free_area_list_from_end();
	print_inuse_area_list_from_head();   

    printk("Area's blk_head_ptr addr =%p\r\n",mem_area2->blk_head_ptr) ;

    uint32_t *p= kmalloc() ;
    printk("p =%p\r\n" ,p) ;
    printk("Area's start addr =%p\r\n" ,mem_area2->m_start) ;
    printk("Area's blk_head_ptr addr =%p\r\n",mem_area2->blk_head_ptr) ;

}


void mem_area_blk_init_test()
{
	print_free_area_list_from_head();

	printk("Test alloc\r\n") ;

	MEM_AREA_INFO_t *mem_area = alloc_mem_area() ;

	print_free_area_list_from_head();
	print_inuse_area_list_from_head();

    memblks_init(mem_area
                ,DEFAULT_AVAL_BLK_SIZE 
                ,DEFAULT_TASK_MA_BLKNUM) ;

    printk("Area's blk_head_ptr addr =%p\r\n",mem_area->blk_head_ptr) ;

    uint32_t *head = mem_area->blk_head_ptr ;
    while(head != NULL)
    {
        printk("Area's blks link addr =%p ,%x\r\n",head ,*head) ;
        head = (uint32_t *)*head ;
    }
    printk("Area's blks link addr =%p\r\n",head) ;

    uint32_t *p1 = blk_alloc(mem_area) ;
    printk("return ptr =%p\r\n" ,p1) ;
    printk("blk head =%p\r\n",mem_area->blk_head_ptr) ;

    uint32_t *p2 = blk_alloc(mem_area) ;
    printk("return ptr =%p\r\n" ,p2) ;
    printk("blk head =%p\r\n",mem_area->blk_head_ptr) ;


    printk("Test free_blk\r\n") ;
    free_blk(p2) ;
    print_from_blk_head(mem_area) ;

}


void print_from_blk_head(MEM_AREA_INFO_t *mem_area)
{
    uint32_t *blkstart = mem_area->blk_head_ptr ;

    while(*blkstart != 0)
    {
        printk("addr=%p ,content=%x\r\n",blkstart ,*blkstart) ;
        blkstart = (uint32_t *)*blkstart ;
    }
    printk("addr=%p ,content=%x\r\n",blkstart ,*blkstart) ;
}

