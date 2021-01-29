
#include "file.h"

/***************************************************************************************/
PATH_TREE_INFO path_tree ;
PATH_NODE *root = NULL ;
/***************************************************************************************/

int file_in_ram_init()
{
    if(path_tree_init() < 0) return -1 ;
    if(create_root_path() < 0) return -1 ;

    PATH_NODE *dev = create_path_node(root ,"/dev\0") ;
    if(dev == NULL) return -1 ;

    PATH_NODE *fifo = create_path_node(root ,"/fifo\0") ;
    if(fifo == NULL) return -1 ;


    create_file_under_node(dev ,"/console_in\0" ,CONSOLE_IN_TYPE) ;
    create_file_under_node(dev ,"/console_out\0" ,CONSOLE_OUT_TYPE) ;
    create_file_under_node(dev ,"/tty0\0" ,TTY0_TYPE) ;
}


int path_tree_init()
{
    path_tree.ma = memAreaAlloc() ;
    if(path_tree.ma == NULL) return -1 ;

    path_tree.ma_aval_start = path_tree.ma->m_start ;
    path_tree.ma->area_status = INUSE_FULL ;

    path_tree.n_nodes = 0 ;
    path_tree.root = NULL ;

    return 0 ;
}


int create_root_path()
{
    if(root != NULL)
    {
        return -1 ;
    }

    root = (PATH_NODE *)path_tree.ma_aval_start ;
    path_tree.ma_aval_start += sizeof(PATH_NODE) ;

    root->firstchild = NULL ;
    root->parent = NULL ;
    root->next_sibling = NULL ;
    root->firstfile = NULL ;
    root->name = root->namebuf ;
    root->name = "/root\0" ;
    return 0 ;
}


PATH_NODE *create_path_node(PATH_NODE *parent ,char *name)
{
    if( (root == NULL) || (parent == NULL) ) return NULL ;

    PATH_NODE *node = (PATH_NODE *)path_tree.ma_aval_start ;
    path_tree.ma_aval_start += sizeof(PATH_NODE) ;

    node->firstchild = NULL ;
    node->parent = parent ;
    node->next_sibling = NULL ;
    node->firstfile = NULL ;

    // Setup node name
    _memset((void *)node->namebuf ,0 ,NAME_BUF_SIZE) ;
    node->name = &node->namebuf[0] ;
    char *dest = node->name ;

    while(*name != '\0')
    {
        *dest =*name ;        
        dest++ ;
        name++ ; 
    }

    // Set the parent node's child
    // Should be link list
    if(node->parent->firstchild != NULL){

        PATH_NODE *end = find_end_sibling(node->parent->firstchild) ;
        end->next_sibling = node ;

    }else{

        node->parent->firstchild = node ;

    }

    return node ;

}


PATH_NODE *find_end_sibling(PATH_NODE *node)
{
    PATH_NODE *start = node ;
    while(start->next_sibling != NULL)
    {
        start = start->next_sibling ;
    }
    return start ;
}




// 目前只能印出root下第一階
void print_under_node(PATH_NODE *node)
{
    if(node == NULL) return ;

    PATH_NODE *node_head = node->firstchild ;
    FILE *file_head = node->firstfile ;

    while(node_head != NULL)
    {
        uart_tx_str(CONSOLE ,node_head->name ,strlen(node_head->name)) ;
        uart_putC(CONSOLE ,'\r') ;
        uart_putC(CONSOLE ,'\n') ;
        

        node_head = node_head->next_sibling ;
    }
    while(file_head != NULL)
    {
        uart_tx_str(CONSOLE ,file_head->name ,strlen(file_head->name)) ;
        uart_putC(CONSOLE ,'\r') ;
        uart_putC(CONSOLE ,'\n') ;
        

        file_head = file_head->next_sibling ;
    }
}

/***************************************************************************************/

FILE *find_end_filesibling(FILE *f)
{
    FILE *start = f ;
    while(start->next_sibling != NULL)
    {
        start = start->next_sibling ;
    }
    return start ;
}



FILE *create_file_under_node(PATH_NODE *node ,char * filename ,int type)
{
    if( (root == NULL) || (node == NULL) ) return NULL ;

    //分配記憶體空間
    FILE *file = (FILE *)path_tree.ma_aval_start ;
    path_tree.ma_aval_start += sizeof(FILE) ;

    file->parent = node ;

    // Setup file name
    _memset((void *)file->namebuf ,0 ,NAME_BUF_SIZE) ;
    file->name = &file->namebuf[0] ;
    char *dest = file->name ;

    while(*filename != '\0')
    {
        *dest =*filename ;        
        dest++ ;
        filename++ ; 
    }

    // setup file type
    file->type = type ;

    switch(file->type)
    {
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

        default:
            file->file_read = NULL ;
            file->file_write = NULL ;
            break;
    }

    // Setup list
    file->next_sibling = NULL ;

    if(node->firstfile == NULL){
        node->firstfile = file ;
        
    }else{
        FILE *end = find_end_filesibling(node->firstfile) ;
        end->next_sibling = file ;

    }

    return file ;
}



int console_read_func(uint8_t *rdbuf ,uint32_t n_bytes)
{
    uint8_t *s = rdbuf ;
    int n_rd = 0;
    while(n_bytes != 0)
    {
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
    while(n_bytes != 0)
    {
        uart_putC(CONSOLE ,*s) ;
        s++ ;
        n_wr++ ;
        n_bytes-- ;
    }

    return n_wr ;
}