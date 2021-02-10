
#include "file.h"
#include "task.h"
#include "ipc.h"
#include "../klib/string.h"


/***************************************************************************************/
struct DIRTREE_INFO path_tree ;
struct DIR_NODE *root = NULL ;
struct FILE *file_list_head = NULL;
/***************************************************************************************/

int file_in_ram_init()
{
	if (path_tree_init() < 0) return -1 ;
	if (create_root_path() < 0) return -1 ;

	struct DIR_NODE *dev = create_path_node(root ,PATH_dev) ;
	if (dev == NULL) return -1 ;
	
	struct DIR_NODE *ipc = create_path_node(root ,PATH_ipc) ;
	if (ipc == NULL) return -1 ;

	create_file_under_node(dev ,FILE_CONSOLE_IN ,CONSOLE_IN_TYPE) ;
	create_file_under_node(dev ,FILE_CONSOLE_OUT ,CONSOLE_OUT_TYPE) ;
	create_file_under_node(dev ,FILE_TTY0 ,TTY0_TYPE) ;

	create_file_under_node(ipc ,FILE_IPC0 ,IPC_TYPE) ;
	ipc_buf_alloc() ;
	
	return 0 ;
}



int path_tree_init()
{
	path_tree.ma = page_alloc() ;

	if (path_tree.ma == NULL) return -1 ;

	path_tree.ma_aval_start = path_tree.ma->pgstart ;
	path_tree.ma->page_status = FULL ;

	path_tree.n_nodes = 0 ;
	path_tree.root = NULL ;

	return 0 ;
}



int create_root_path()
{
	if (root != NULL){
		return -1 ;
	}

	root = (struct DIR_NODE *)path_tree.ma_aval_start ;
	path_tree.ma_aval_start += sizeof(struct DIR_NODE) ;

	root->firstchild = NULL ;
	root->parent = NULL ;
	root->next_sibling = NULL ;
	root->firstfile = NULL ;
	root->name = root->namebuf ;
	root->name = PATH_root ;
	return 0 ;
}



struct DIR_NODE *create_path_node(struct DIR_NODE *parent ,char *name)
{
	if ((root == NULL) || (parent == NULL)) return NULL ;

	struct DIR_NODE *node = (struct DIR_NODE *)path_tree.ma_aval_start ;
	path_tree.ma_aval_start += sizeof(struct DIR_NODE) ;

	node->firstchild = NULL ;
	node->parent = parent ;
	node->next_sibling = NULL ;
	node->firstfile = NULL ;

	/* Setup node name */
	_memset((void *)node->namebuf ,0 ,NAME_BUF_SIZE) ;
	node->name = &node->namebuf[0] ;
	char *dest = node->name ;

	while (*name != '\0'){
		*dest =*name ;        
		dest++ ;
		name++ ; 
	}

	/* Set parent node's child */
	if (node->parent->firstchild != NULL){
		struct DIR_NODE *end = find_end_sibling(node->parent->firstchild) ;
		end->next_sibling = node ;

	} else {
		node->parent->firstchild = node ;
	}

	return node ;

}



struct DIR_NODE *find_end_sibling(struct DIR_NODE *node)
{
	struct DIR_NODE *start = node ;
	while (start->next_sibling != NULL) {
		start = start->next_sibling ;
	}
	return start ;
}



/* 印出node下一階 */
void print_under_node(struct DIR_NODE *node)
{
	if (node == NULL) return ;

	struct DIR_NODE *node_head = node->firstchild ;
	struct FILE *file_head = node->firstfile ;

	while (node_head != NULL) {
		uart_tx_str(CONSOLE ,node_head->name ,_strlen(node_head->name)) ;
		uart_putC(CONSOLE ,'\r') ;
		uart_putC(CONSOLE ,'\n') ;
		
		node_head = node_head->next_sibling ;
	}
	while (file_head != NULL) {
		uart_tx_str(CONSOLE ,file_head->name ,_strlen(file_head->name)) ;
		uart_putC(CONSOLE ,'\r') ;
		uart_putC(CONSOLE ,'\n') ;
		
		file_head = file_head->next_sibling ;
	}
}

/***************************************************************************************/

struct FILE *find_end_filesibling(struct FILE *f)
{
	struct FILE *start = f ;
	while (start->next_sibling != NULL) {
		start = start->next_sibling ;
	}
	return start ;
}



struct FILE *create_file_under_node(struct DIR_NODE *node ,char * filename ,int type)
{
	if ((root == NULL) || (node == NULL)) return NULL ;

	/* 分配記憶體空間 */
	struct FILE *file = (struct FILE *)path_tree.ma_aval_start ;
	path_tree.ma_aval_start += sizeof(struct FILE) ;

	file->parent = node ;

	/* Setup file name */
	_memset((void *)file->namebuf ,0 ,NAME_BUF_SIZE) ;
	file->name = &file->namebuf[0] ;
	char *dest = file->name ;

	while (*filename != '\0') {
		*dest =*filename ;        
		dest++ ;
		filename++ ; 
	}

	/* setup file type */
	file->type = type ;

	switch (file->type) {
		case CONSOLE_IN_TYPE:
			file->file_read = console_read_func ;
			file->file_write = NULL ;          
			break ;
			
		case CONSOLE_OUT_TYPE:
			file->file_read = NULL ;
			file->file_write = console_write_func ;          
			break ;

		case TTY0_TYPE:
			file->file_read = console_read_func ;
			file->file_write = console_write_func ;          
			break ;

		case IPC_TYPE:
			file->file_read = ipc_read_func ;
			file->file_write = ipc_write_func ;          
			break ;

		default:
			file->file_read = NULL ;
			file->file_write = NULL ;
			break;
	}

	/* Setup list */
	file->next_sibling = NULL ;

	if (node->firstfile == NULL) {
		node->firstfile = file ;
		
	} else {
		struct FILE *end = find_end_filesibling(node->firstfile) ;
		end->next_sibling = file ;

	}
	/* Add file to list */
	file_list_push(file) ;

	return file ;
}



void file_list_push(struct FILE *file)
{
	/* list內還沒有資料 */
	if (file_list_head == NULL) {
		file_list_head = file ;
		file->list_next = NULL ;
		file->list_prev = NULL ;
		return ;
	}

	/* 移動到end */
	struct FILE *head = file_list_head ;
	while (head->list_next != NULL) {
		head = head->list_next ;
	}
	struct FILE *end = head ;

	end->list_next = file ;
	file->list_prev = end ;
	file->list_next = NULL ;
}



void print_file_list()
{
	struct FILE *head = file_list_head ;
	while (head != NULL) {
		uart_tx_str(CONSOLE ,head->name ,_strlen(head->name)) ;
		uart_putC(CONSOLE ,'\r') ;
		uart_putC(CONSOLE ,'\n') ;

		head = head->list_next ;
	}
}



struct FILE *find_file(char *filename)
{
	struct FILE *head = file_list_head ;
	while (head->list_next != NULL) {
		if (_strcmp(filename ,head->name) == 0) return head ;

		head = head->list_next ;
	}
	if (_strcmp(filename ,head->name) == 0) return head ;

	return NULL ;    
}



struct DIR_NODE *find_target_subdir(struct DIR_NODE *curdir ,char *subdir_name)
{
	struct DIR_NODE *en = curdir->firstchild ;

	int neq = _strcmp(subdir_name ,"..\0") ;

	if (neq){ /* neq */
		while (en != NULL) {
			if (!_strcmp(en->name ,subdir_name)) return en ; /* eq */
			en = en->next_sibling ;
		}
		return NULL ;       /* No match */

	}else if (neq == 0) {   /*eq ,切換道上一層路徑 */
		if (curdir->parent == NULL) return NULL ; /* No parent dir */

		return curdir->parent ; /* return parent dir */
	} else {
		return NULL ;
	}    
}

/***********************************************************************************/
// File operation
/***********************************************************************************/

fd_t file_open(char *filename ,void *_task)
{
	struct TASK_INFO *task = (struct TASK_INFO *)_task ;

	int fd = -1;
	struct FILE *file = find_file(filename) ;

	if (file == NULL) 
		return -1 ;

	for (int i =0 ; i<MAX_FD; i++) {
		if ((task->openfiles[i] == NULL) && (i!=2)){
			task->openfiles[i] = file ;
			fd = i ;
			break ;
		} else if ((task->openfiles[i] == NULL) && (i==2)) {
			/* i = fd = 2 for stderr ,not implement yet. */
		}
	}
	return fd ;
}



void file_close(fd_t fd ,void *_task)
{
	struct TASK_INFO *task = (struct TASK_INFO *)_task ;
	task->openfiles[fd] = NULL ;
}



int console_read_func(uint8_t *rdbuf ,uint32_t n_bytes)
{
	uint8_t *s = rdbuf ;
	int n_rd = 0;
	while (n_bytes != 0) {
		*s = uart_getC(CONSOLE) ;
		s++ ;
		n_rd++ ;
		n_bytes-- ;
	}

	return n_rd ;
}



int console_write_func(uint8_t *wrbuf ,uint32_t n_bytes)
{
	uint8_t *s = wrbuf ;
	int n_wr = 0 ;
	while (n_bytes != 0) {
		uart_putC(CONSOLE ,*s) ;
		s++ ;
		n_wr++ ;
		n_bytes-- ;
	}

	return n_wr ;
}