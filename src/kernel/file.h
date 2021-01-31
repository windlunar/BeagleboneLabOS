
#ifndef __FILE_H_
#define __FILE_H_

#include "../common.h"
#include "memory.h"
#include "kprint.h"
#include "../driver/uart.h"
#include "../klib/string.h"
#include "../klib/mem.h"
/***************************************************************************************/
#define PATH_root   "/root\0"
#define PATH_dev    "/dev\0"
#define PATH_fifo   "/fifo\0"
/***************************************************************************************/
#define CONSOLE_IN_FD      0
#define CONSOLE_OUT_FD     1
/***************************************************************************************/
#define FILE_CONSOLE_IN      "/console_in\0"
#define FILE_CONSOLE_OUT     "/console_out\0"
#define FILE_TTY0            "/tty0\0"
/***************************************************************************************/
#define CONSOLE_IN_TYPE     0
#define CONSOLE_OUT_TYPE    1
#define TTY0_TYPE           2
#define FIFO_TYPE           3
#define TEXT_TYPE           4
/***************************************************************************************/
#define NAME_BUF_SIZE       16
typedef int FILE_DESCRIPTOR_t ;


/***************************************************************************************/
// Structures
/***************************************************************************************/
struct _FILE
{
    struct _FILE *next_sibling ;
    uint32_t type ;

    struct _DIR_NODE *parent ;

    int (*file_read)(uint8_t *rdbuf ,uint32_t n_bytes) ;
    int (*file_write)(uint8_t *wrbuf ,uint32_t n_bytes) ;

    char namebuf[NAME_BUF_SIZE] ;
    char *name ;

    struct _FILE *list_next ;
    struct _FILE *list_prev ;
};
typedef struct _FILE FILE ;


struct _DIR_NODE
{
    struct _DIR_NODE *firstchild ;   //list head
    struct _DIR_NODE *parent ;
    struct _DIR_NODE *next_sibling ;
    FILE    *firstfile;  //list head
    char namebuf[NAME_BUF_SIZE] ;
    char *name ;
    
};
typedef struct _DIR_NODE   DIR_NODE ;


struct _DIR_TREE_INFO_
{
    MEM_AREA_INFO_t *ma ;
    void *ma_aval_start ;
    uint32_t n_nodes ;  // Number of nodes
    DIR_NODE *root ;
};
typedef struct _DIR_TREE_INFO_   DIRTREE_INFO ;


struct __FILE_RDWR_ARGS
{
    int fd ;
    uint8_t *buf ;
    uint32_t n_bytes;
};
typedef struct __FILE_RDWR_ARGS FILE_RDWR_ARGS_t ;


/***************************************************************************************/
// Variables
/***************************************************************************************/
extern DIRTREE_INFO path_tree ;
extern DIR_NODE *root ;
extern FILE *file_list_head;

/***************************************************************************************/
// Init related
/***************************************************************************************/
int file_in_ram_init() ;
int path_tree_init() ;


/***************************************************************************************/
// create node
/***************************************************************************************/
int create_root_path() ;
DIR_NODE *create_path_node(DIR_NODE *parent ,char *name);


/***************************************************************************************/
// create file
/***************************************************************************************/
void file_list_push(FILE *file) ;
FILE *create_file_under_node(DIR_NODE *node ,char * filename ,int type) ;
FILE *find_file(char *filename) ;


/***************************************************************************************/
// file operation
/***************************************************************************************/
int console_read_func(uint8_t *rdbuf ,uint32_t n_bytes);
int console_write_func(uint8_t *wrbuf ,uint32_t n_bytes);
FILE_DESCRIPTOR_t file_open(char *filename ,void *_task) ;


/***************************************************************************************/
// Others
/***************************************************************************************/
DIR_NODE *find_end_sibling(DIR_NODE *node) ;
FILE *find_end_filesibling(FILE *node) ;


/***************************************************************************************/
// For debug
/***************************************************************************************/
void print_under_node(DIR_NODE *node) ;
void print_file_list() ;

/***************************************************************************************/
#endif