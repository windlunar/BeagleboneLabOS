
#ifndef __FILE_H_
#define __FILE_H_

#include "../common.h"
#include "memory.h"
#include "../klib/print.h"
#include "../driver/uart.h"
#include "../klib/string.h"
#include "../klib/mem.h"

/***************************************************************************************/
#define CONSOLE_IN_TYPE     0
#define CONSOLE_OUT_TYPE     1
#define TTY0_TYPE        2
#define FIFO_TYPE       3
#define TEXT_TYPE       4

#define NAME_BUF_SIZE   16
/***************************************************************************************/
struct _FILE
{
    struct _FILE *next_sibling ;
    uint32_t type ;

    struct _TREE_NODE *parent ;

    int (*file_read)(uint8_t *rdbuf ,uint32_t n_bytes) ;
    int (*file_write)(uint8_t *wrbuf ,uint32_t n_bytes) ;

    char namebuf[NAME_BUF_SIZE] ;
    char *name ;
};
typedef struct _FILE FILE ;


struct _TREE_NODE
{
    struct _TREE_NODE *firstchild ;   //list head
    struct _TREE_NODE *parent ;
    struct _TREE_NODE *next_sibling ;
    FILE    *firstfile;  //list head
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
PATH_NODE *create_path_node(PATH_NODE *parent ,char *name);

PATH_NODE *find_end_sibling(PATH_NODE *node) ;

void print_under_node(PATH_NODE *node) ;



FILE *find_end_filesibling(FILE *node) ;
FILE *create_file_under_node(PATH_NODE *node ,char * filename ,int type) ;
int console_read_func(uint8_t *rdbuf ,uint32_t n_bytes);
int console_write_func(uint8_t *wrbuf ,uint32_t n_bytes);

#endif