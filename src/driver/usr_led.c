
#include "usr_led.h"
#include "../common.h"
#include "cm_per.h"
#include "gpio_reg.h"


volatile uint32_t read_val;


void init_usrled_asm(){
	
	/** CM_PER_GPIO1_CLKCTRL register bit[1:0] : 
	 * 0x2 = MODULEMODE : ENABLE : Module is explicitly enabled.
	 * 
	 * CM_PER_GPIO1_CLKCTRL register bit[18] : 0x1 = 
	 * FCLK_EN : Optional functional clock is enabled
	 */
	WRITE_REG32((unsigned int)&(CM_PER_BASE_PTR->CM_PER_GPIO1_CLKCTRL) , 1 << 18 | 2);

	read_val = READ_REG32((unsigned int)&(GPIO1_PTR->GPIO_OE));
	read_val &= ~(15 << 21);

	WRITE_REG32((unsigned int)&(GPIO1_PTR->GPIO_OE),read_val);
}



void blink_usrled_asm(){
	WRITE_REG32((unsigned int)&(GPIO1_PTR->GPIO_SETDATAOUT), (15 << 21));
	for (read_val = 0; read_val < 1000000; read_val ++) ;

	WRITE_REG32((unsigned int)&(GPIO1_PTR->GPIO_CLEARDATAOUT) ,15 << 21);
	for (read_val = 0; read_val < 1000000; read_val ++) ;
}



void usrled_init(){
	
	/** CM_PER_GPIO1_CLKCTRL register bit[1:0] : 0x2 = 
	 * MODULEMODE : ENABLE : Module is explicitly enabled.
	 * 
	 * CM_PER_GPIO1_CLKCTRL register bit[18] : 0x1 = 
	 * FCLK_EN : Optional functional clock is enabled
	 */
	CM_PER_BASE_PTR->CM_PER_GPIO1_CLKCTRL = 0x40002 ;
	GPIO1_PTR->GPIO_OE &= ~(15 << 21); 
}



void usrled_blink(){
	GPIO1_PTR->GPIO_SETDATAOUT = (15<<21) ;
	for (read_val = 0; read_val < 500000; read_val ++) ;

	GPIO1_PTR->GPIO_CLEARDATAOUT = (15 << 21) ;
	for (read_val = 0; read_val < 500000; read_val ++) ;
}



void toggle_usrled(uint32_t usr_led_num)
{
	GPIO1_PTR->GPIO_DATAOUT ^= (1 << (21 + usr_led_num));
}