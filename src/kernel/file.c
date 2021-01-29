
#include "file.h"

/***************************************************************************************/
PATH_TREE_INFO path_tree ;
PATH_NODE *root = NULL ;
/***************************************************************************************/

int file_in_ram_init()
{
    if(path_tree_init() < 0) return -1 ;
    if(create_root_path() < 0) return -1 ;
    if(create_path_node(root ,"/dev\0") < 0) return -1 ;
    if(create_path_node(root ,"/fifo\0") < 0) return -1 ;
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
    root->files = NULL ;
    root->name = root->namebuf ;
    root->name = "/root\0" ;
    return 0 ;
}


int create_path_node(PATH_NODE *parent ,char *name)
{
    if( (root == NULL) || (parent == NULL) ) return -1 ;

    PATH_NODE *node = (PATH_NODE *)path_tree.ma_aval_start ;
    path_tree.ma_aval_start += sizeof(PATH_NODE) ;

    node->firstchild = NULL ;
    node->parent = parent ;
    node->next_sibling = NULL ;
    node->files = NULL ;
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

    return 0 ;

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


void print_path_tree()
{
    PATH_NODE *head = root->firstchild ;

    while(head != NULL)
    {
        uart_tx_str(CONSOLE ,head->name ,strlen(head->name)) ;
        uart_putC(CONSOLE ,'\r') ;
        uart_putC(CONSOLE ,'\n') ;
        head = head->next_sibling ;
    }
}