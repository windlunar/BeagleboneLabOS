
#include "usr_led.h"
#include "common.h"

void usrLed_Init_asm(){
    
	//CM_PER_GPIO1 register bit[1:0] : 0x2 = MODULEMODE : ENABLE : Module is explicitly enabled.
	//CM_PER_GPIO1 register bit[18] : 0x1 = FCLK_EN : Optional functional clock is enabled
	WR_REG32(CM_PER_BASE+CM_PER_GPIO1, 1<<18 | 2);

	ra = RD_REG32(GPIO1_BASE+GPIO_OE);
	ra &= ~(15<<21);
	WR_REG32(GPIO1_BASE+GPIO_OE,ra);
}

void usrLed_blink_asm(){
    WR_REG32(GPIO1_BASE+GPIO_SETDATAOUT, (15<<21));
	for(ra = 0; ra < TIME; ra ++);
	WR_REG32(GPIO1_BASE+GPIO_CLRDATAOUT,15<<21);
	for(ra = 0; ra < TIME; ra ++);
}

void usrLed_Init(){
    
	//CM_PER_GPIO1 register bit[1:0] : 0x2 = MODULEMODE : ENABLE : Module is explicitly enabled.
	//CM_PER_GPIO1 register bit[18] : 0x1 = FCLK_EN : Optional functional clock is enabled
	*(volatile uint32 *)(CM_PER_BASE + CM_PER_GPIO1) = 0x00040002 ;
	*(volatile uint32 *)(GPIO1_BASE + GPIO_OE) &= ~(15<<21); 
}

void usrLed_blink(){
    *(volatile uint32 *)(GPIO1_BASE + GPIO_SETDATAOUT) = (15<<21) ;
    for(ra = 0; ra < TIME; ra ++);

    *(volatile uint32 *)(GPIO1_BASE + GPIO_CLRDATAOUT) = (15<<21) ;
	for(ra = 0; ra < TIME; ra ++);
}