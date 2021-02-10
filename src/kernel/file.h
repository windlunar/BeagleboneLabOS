
#ifndef __FILE_H_
#define __FILE_H_

#include "../common.h"
#include "memory.h"
#include "printk.h"
#include "../driver/uart.h"
#include "../klib/string.h"
#include "../klib/mem.h"


/***************************************************************************************/

#define PATH_root   "/root\0"
#define PATH_dev    "/dev\0"
#define PATH_ipc   "/ipc\0"

/***************************************************************************************/

#define CONSOLE_IN_FD      0
#define CONSOLE_OUT_FD     1

/***************************************************************************************/

#define FILE_CONSOLE_IN      "/console_in\0"
#define FILE_CONSOLE_OUT     "/console_out\0"
#define FILE_TTY0            "/tty0\0"
#define FILE_IPC0            "/ipc0\0"

/***************************************************************************************/

#define CONSOLE_IN_TYPE     0
#define CONSOLE_OUT_TYPE    1
#define TTY0_TYPE           2
#define IPC_TYPE            3
#define TEXT_TYPE           4

/***************************************************************************************/

#define NAME_BUF_SIZE       16
typedef int fd_t ;

/***************************************************************************************/
// Structures
/***************************************************************************************/

struct FILE
{
	struct DIR_NODE *parent ;
	struct FILE *next_sibling ; /* under same dir */
	struct FILE *list_next ;    /* All file */
	struct FILE *list_prev ;    /* All file */
	uint32_t type ;
	char namebuf[NAME_BUF_SIZE] ;
	char *name ;
	int (*file_read)(uint8_t *rdbuf ,uint32_t n_bytes) ;
	int (*file_write)(uint8_t *wrbuf ,uint32_t n_bytes) ;
};



struct DIR_NODE
{
	struct DIR_NODE *firstchild ;   /*list head */
	struct DIR_NODE *parent ;
	struct DIR_NODE *next_sibling ;
	struct FILE    *firstfile;      /*list head */
	char namebuf[NAME_BUF_SIZE] ;
	char *name ;
	
};



struct DIRTREE_INFO
{
	struct PAGE_INFO *ma ;
	void *ma_aval_start ;
	uint32_t n_nodes ;          /* Number of nodes */
	struct DIR_NODE *root ;
};



struct FILE_RDWR_ARGS
{
	int fd ;
	uint8_t *buf ;
	uint32_t n_bytes;
};

/***************************************************************************************/
// Variables
/***************************************************************************************/

extern struct DIRTREE_INFO path_tree ;
extern struct DIR_NODE *root ;
extern struct FILE *file_list_head;

/***************************************************************************************/
// Init related
/***************************************************************************************/

int file_in_ram_init() ;
int path_tree_init() ;


/***************************************************************************************/
// create node
/***************************************************************************************/

int create_root_path() ;
struct DIR_NODE *create_path_node(struct DIR_NODE *parent ,char *name);

/***************************************************************************************/
// create file
/***************************************************************************************/

void file_list_push(struct FILE *file) ;
struct FILE *create_file_under_node(struct DIR_NODE *node ,char * filename ,int type) ;
struct FILE *find_file(char *filename) ;

/***************************************************************************************/
// file operation
/***************************************************************************************/

int console_read_func(uint8_t *rdbuf ,uint32_t n_bytes);
int console_write_func(uint8_t *wrbuf ,uint32_t n_bytes);
fd_t file_open(char *filename ,void *_task) ;
void file_close(fd_t fd ,void *_task) ;

/***************************************************************************************/
// Others
/***************************************************************************************/

struct DIR_NODE *find_end_sibling(struct DIR_NODE *node) ;
struct FILE *find_end_filesibling(struct FILE *node) ;
struct DIR_NODE *find_target_subdir(struct DIR_NODE *curdir ,char *subdir_name) ;

/***************************************************************************************/
// For debug
/***************************************************************************************/

void print_under_node(struct DIR_NODE *node) ;
void print_file_list() ;

/***************************************************************************************/
#endif