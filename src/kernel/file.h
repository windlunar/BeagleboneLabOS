
#ifndef __FILE_H_
#define __FILE_H_

#include "../common.h"
#include "memory.h"
#include "../klib/print.h"
#include "../driver/uart.h"
#include "../klib/string.h"
#include "../klib/mem.h"

/***************************************************************************************/
#define DEVICE_FILE     0
#define FIFO_FILE       1
#define TEXT_FILE       2

#define NAME_BUF_SIZE   12
/***************************************************************************************/
struct _FILE
{
    uint32_t type ;
    int (*file_read)(unsigned char *rdbuf ,unsigned int n_bytes) ;
    int (*file_write)(unsigned char *wrbuf ,unsigned int n_bytes) ;

};
typedef struct _FILE FILE ;


struct _TREE_NODE
{
    struct _TREE_NODE *firstchild ;   //list head
    struct _TREE_NODE *parent ;
    struct _TREE_NODE *next_sibling ;
    FILE    *files;  //list head
    char namebuf[NAME_BUF_SIZE] ;
    char *name ;
};
typedef struct _TREE_NODE   PATH_NODE ;


struct TREE_INFO
{
    MEM_AREA_INFO_t *ma ;
    void *ma_aval_start ;
    uint32_t n_nodes ;  // Number of nodes
    PATH_NODE *root ;
};
typedef struct TREE_INFO   PATH_TREE_INFO ;
/***************************************************************************************/
extern PATH_TREE_INFO path_tree ;
extern PATH_NODE *root ;
/***************************************************************************************/
int file_in_ram_init() ;
int path_tree_init() ;
int create_root_path() ;
int create_path_node(PATH_NODE *parent ,char *name);

PATH_NODE *find_end_sibling(PATH_NODE *node) ;

void print_path_tree();

#endif