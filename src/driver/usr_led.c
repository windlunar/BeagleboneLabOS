
#include "usr_led.h"
#include "../common.h"
#include "cm_per.h"
#include "gpio_reg.h"

volatile uint32 read_val;

void usrLedInitAsm(){
    
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

void usrLedBlinkAsm(){
    WRITE_REG32((unsigned int)&(GPIO1_PTR->GPIO_SETDATAOUT), (15 << 21));
	for(read_val = 0; read_val < 1000000; read_val ++);

	WRITE_REG32((unsigned int)&(GPIO1_PTR->GPIO_CLEARDATAOUT) ,15 << 21);
	for(read_val = 0; read_val < 1000000; read_val ++);
}

void usrLedInit(){
    
	/** CM_PER_GPIO1_CLKCTRL register bit[1:0] : 0x2 = 
	 * MODULEMODE : ENABLE : Module is explicitly enabled.
	 * 
	 * CM_PER_GPIO1_CLKCTRL register bit[18] : 0x1 = 
	 * FCLK_EN : Optional functional clock is enabled
	 */
	CM_PER_BASE_PTR->CM_PER_GPIO1_CLKCTRL = 0x40002 ;
	GPIO1_PTR->GPIO_OE &= ~(15 << 21); 
}

void usrLedBlink(){
    GPIO1_PTR->GPIO_SETDATAOUT = (15<<21) ;
    for(read_val = 0; read_val < 500000; read_val ++);

    GPIO1_PTR->GPIO_CLEARDATAOUT = (15 << 21) ;
	for(read_val = 0; read_val < 500000; read_val ++);
}


/*
#define LED_MASK (0xF << LED_GPIO_START_BIT)

void leds_init(void)
{
	// Enable the GPIO1 clock
	CM_PER_BASE_PTR->CM_PER_GPIO1_CLKCTRL = (0x1 << 18) | (0x2 << 0);

	// Enable output
	GPIO1_PTR->GPIO_OE &= ~LED_MASK;
}

*/
void usrLedToggle(uint32_t usr_led_num)
{
	GPIO1_PTR->GPIO_DATAOUT ^= (1 << (21 + usr_led_num));
}