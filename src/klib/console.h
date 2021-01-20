
#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include "../common.h"

#define HORIZENTAL_LENTH	    80
#define VERTICAL_LENTH		24
#define SHOWCHAR	        '1'
#define SHIFT_V     5


void draw_horizental_line(char *input ,int n_h,int h ,int v ,char character ,int len);
void draw_vertical_line(char *input ,int n_h ,int h ,int v ,char character ,int len);

void draw_whole(char *input ,int n_v ,int n_h ,char character);
void draw_last_pix_every_line(char *input ,int n_v ,int n_h ,char character);
void draw_console(void);

#endif