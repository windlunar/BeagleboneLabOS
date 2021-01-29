


#include "../common.h"
#include "console.h"
#include "../kernel/kprint.h"
#include "../driver/uart.h"


// |---------->h(h)
// |
// |
// Vv(v)
//
// arg1 : input整個畫布陣列比如 24(v)*80(h)
// arg2 : h軸(橫軸)的長度
// arg3 : v軸(橫軸)的開始(要畫的起點)
// arg4 : h軸(橫軸)的開始(要畫的起點)
// arg5 : 長度(像素點)
// arg6 : 要填的字 (如果是像素點的話可以改成顏色)
void draw_horizental_line(char *input ,int n_h,int h ,int v ,char character ,int len)
{
	char *s = input ;
	s = s + (n_h*v + h) ;

	while(len != 0)
	{
		*(s+len-1) = character ;
		len--;
	}
}


// |---------->h
// |
// |
// Vv
//
// arg1 : input整個畫布陣列比如 24(v)*80(h)
// arg2 : h軸(橫軸)的長度(或說有幾個像素點)
// arg3 : v軸(橫軸)的開始(要畫的起點)
// arg4 : h軸(橫軸)的開始(要畫的起點)
// arg5 : 長度(像素點)
// arg6 : 要填的字 (如果是像素點的話可以改成顏色)
void draw_vertical_line(char *input ,int n_h ,int h ,int v ,char character ,int len)
{
	char *s = input ;
	s = s + n_h*v + h ;

	while(len != 0)
	{
		*(s + n_h*(len-1)) = character ;
		len--;
	}
}

void draw_whole(char *input ,int n_v ,int n_h ,char character)
{
	char *s = input ;
	int n_total = n_v * n_h ;
	for(int i=0 ; i<n_total; i++)
	{
		*(s+i) = character ;
	}
}


void draw_last_pix_every_line(char *input ,int n_v ,int n_h ,char character)
{
	draw_vertical_line(input ,n_h ,n_h-1 ,0 ,'\n' ,n_v) ;

	//最後一個字元放空字元
	draw_vertical_line(input ,n_h ,n_h-1 ,n_v-1 ,'\0' ,1) ;
}


void draw_console(void)
{
	
	char p[VERTICAL_LENTH][HORIZENTAL_LENTH] ;

	draw_whole(&p[0][0] ,VERTICAL_LENTH ,HORIZENTAL_LENTH ,' ') ;
	
	// draw 'O'
	draw_horizental_line(&p[0][0] ,HORIZENTAL_LENTH ,13 ,5+SHIFT_V ,H_CHAR ,19) ;
	draw_horizental_line(&p[0][0] ,HORIZENTAL_LENTH ,13 ,15+SHIFT_V ,H_CHAR ,19) ;

	draw_vertical_line(&p[0][0] ,HORIZENTAL_LENTH ,12 ,6+SHIFT_V ,V_CHAR ,9) ;
	draw_vertical_line(&p[0][0] ,HORIZENTAL_LENTH ,32 ,6+SHIFT_V ,V_CHAR ,9) ;

	//draw 'S'
	draw_horizental_line(&p[0][0] ,HORIZENTAL_LENTH ,42 ,5+SHIFT_V ,H_CHAR ,20) ;
	draw_horizental_line(&p[0][0] ,HORIZENTAL_LENTH ,42 ,10+SHIFT_V ,H_CHAR ,20) ;
	draw_horizental_line(&p[0][0] ,HORIZENTAL_LENTH ,42 ,15+SHIFT_V ,H_CHAR ,20) ;

	draw_vertical_line(&p[0][0] ,HORIZENTAL_LENTH ,41 ,6+SHIFT_V ,V_CHAR ,4) ;
	draw_vertical_line(&p[0][0] ,HORIZENTAL_LENTH ,61 ,11+SHIFT_V ,V_CHAR ,4) ;

	//畫每一行的最後兩個字元為 \r\n
	draw_last_pix_every_line(&p[0][0] ,VERTICAL_LENTH ,HORIZENTAL_LENTH ,'\n') ;
	draw_vertical_line(&p[0][0] ,HORIZENTAL_LENTH ,HORIZENTAL_LENTH-2 ,0 ,'\r' ,VERTICAL_LENTH) ;
	
	uart_tx_str(UART0_PTR ,&p[0][0] ,VERTICAL_LENTH*HORIZENTAL_LENTH) ;

}

