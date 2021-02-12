

#include "debug.h"


void print_LR(uint32_t data){
	printk("LR : %x\r\n", data) ;
	for(;;);
}



void print_SP(uint32_t data){
	printk("SP : %x\r\n", data) ;
}



void print_here(){
	printk("HERE\r\n") ;
}



void print_cpsr(){
	printk("cpsr : %x \r\n",READ_CPSR());
}



void stk_display(uint32_t stk_top ,int num){
	uint32_t *stktop_ptr = (uint32_t *)stk_top ;
	for (int i = 0 ; i < num ; i++) {
		printk("MEM ADDR : %p --- CONTENT : %x\r\n", stktop_ptr-i,*(stktop_ptr-i)) ;
	}
}



int32_t get_cpsr_mode(){
	int32_t cpsr = READ_CPSR() ;
	int32_t mode = cpsr & 0x1F ;

	switch (mode) {
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



void print_R0_R1_R2_R3(uint32_t r0 ,uint32_t r1 ,uint32_t r2 ,uint32_t r3)
{
	printk("R0 : %x---R1 : %x---R2 : %x---R3 : %x\r\n", r0,r1,r2,r3) ;
}



void print_free_area_list_from_head()
{
	struct PAGE_INFO *head = free_page_head ;

	/* Just print first 10 nodes for test */
	printk("Print free list from head\r\n");
	for(int i =0 ;i<10;i++){
		printk("%p  ",head->pgstart) ;
		printk("mem page id:%d  ",head->page_id) ;
		printk("mem page status:%d\r\n",head->page_status) ;
		head = head->next ;
	}
}



void print_free_area_list_from_end()
{
	struct PAGE_INFO *head = free_page_head ;

	while(head->next != NULL){
		head = head->next ;
	}
	struct PAGE_INFO *end = head ;

	/* Just print first 10 nodes for test */
	printk("Print free list from end\r\n");
	for(int i =0 ;i<10;i++){
		printk("%p  ",end->pgstart) ;
		printk("mem page id:%d  ",end->page_id) ;
		printk("mem page status:%d\r\n",end->page_status) ;
		end = end->prev ;
	}
}



void print_inuse_area_list_from_head()
{
	struct PAGE_INFO *head = inuse_page_head ;

	/* Just print first 10 nodes for test */
	printk("Print inuse list\r\n");
	for (int i =0 ;i<10;i++) {
		printk("%p  ",head->pgstart) ;
		printk("mem page id:%d  ",head->page_id) ;
		printk("mem page status:%d\r\n",head->page_status) ;

		if (head->next == NULL) break ;
		head = head->next ;
	}
}



void print_from_blk_head(struct PAGE_INFO *pg)
{
	struct BLK_INFO *blkstart = (struct BLK_INFO *)pg->blk_list_head ;

	while (blkstart->next != NULL) {
		printk("addr=%p ,content=%p\r\n",blkstart ,blkstart->start) ;
		blkstart = blkstart->next ;
	}
	printk("addr=%p ,content=%p\r\n",blkstart ,blkstart->start) ;
}

