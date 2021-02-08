#ifndef __COMMANDS_H_
#define __COMMANDS_H_


#include "../common.h"


#define SIZE_OF_CMDBUF	16
#define SIZE_OF_CWD		64
#define SIZE_OF_CMD_TOKEN		SIZE_OF_CMDBUF


void lsdir() ;
int cd(char *subdir) ;
int pwd(char *cwd ,int cwdsz);


#endif