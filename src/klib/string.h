
#ifndef __STRING_H_
#define __STRING_H_

#include "../common.h"

int _strlen(char *s) ;
int _strcmp(char *s ,char *b) ;
int _strcat(char *dest ,char *src);
void _strcpy(char *dest ,char *src);
char *strtok_fst(char *start ,char *delim ,int delim_sz ,char *token) ;
void _strncpy(char *dest ,char *src ,int count) ;

#endif