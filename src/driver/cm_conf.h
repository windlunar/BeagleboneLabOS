/**
 * Define Control Module pin config registers
 */  

#ifndef __CM_CONF_H_
#define __CM_CONF_H_

#include "../common.h"

//Control module base address is 0x44E10000
//And the pffset of pin config registers are 0x800
#define CM_CONF_BASE    0x44E10800

#define CONF_GPMC_PTR           ((volatile CONF_GPMC_T *)CM_CONF_BASE)
#define CONF_LCD_PTR            ((volatile CONF_LCD_T *)CM_CONF_BASE + 0x8A0)
#define CONF_MMC0_PTR           ((volatile CONF_MMC0_T *)CM_CONF_BASE + 0x8F0)
#define CONF_MII_PTR            ((volatile CONF_MII_T *)CM_CONF_BASE + 0x908)
#define CONF_MD_PTR             ((volatile CONF_MD_T *)CM_CONF_BASE + 0x948)
#define CONF_SPI0_PTR           ((volatile CONF_SPI0_T *)CM_CONF_BASE + 0x950)
#define CONF_ECAP0_PTR          ((volatile CONF_ECAP0_T *)CM_CONF_BASE + 0x964)
#define CONF_UART0_PTR          ((volatile CONF_UART0_T *)CM_CONF_BASE + 0x968)
#define CONF_UART1_PTR          ((volatile CONF_UART1_T *)CM_CONF_BASE + 0x978)
#define CONF_I2C0_PTR           ((volatile CONF_I2C0_T *)CM_CONF_BASE + 0x988)
#define CONF_MCASP0_PTR         ((volatile CONF_MCASP0_T *)CM_CONF_BASE + 0x990)
#define CONF_XDMA_PTR           ((volatile CONF_XDMA_T *)CM_CONF_BASE + 0x9B0)
#define CONF_WARM_RSTN_PTR      ((volatile CONF_WARM_RSTN_T *)CM_CONF_BASE + 0x9B8)
#define CONF_NNMI_PTR           ((volatile CONF_NNMI_T *)CM_CONF_BASE + 0x9C0)
#define CONF_T_PTR              ((volatile CONF_T_T *)CM_CONF_BASE + 0x9D0)
#define CONF_EMU_PTR            ((volatile CONF_EMU_T *)CM_CONF_BASE + 0x9E4)
#define CONF_RTC_PWRONRSTN_PTR  ((volatile CONF_RTC_PWRONRSTN_T *)CM_CONF_BASE + 0x9F8)
#define CONF_PMIC_POWER_EN_PTR  ((volatile CONF_PMIC_POWER_EN_T *)CM_CONF_BASE + 0x9FC)
#define CONF_EXT_WAKEUP_PTR     ((volatile CONF_EXT_WAKEUP_T *)CM_CONF_BASE + 0xA00)
#define CONF_USB0_DRVVBUS_PTR   ((volatile CONF_USB0_DRVVBUS_T *)CM_CONF_BASE + 0xA1C)
#define CONF_USB1_DRVVBUS_PTR   ((volatile CONF_USB1_DRVVBUS_T *)CM_CONF_BASE + 0xA34)


#define    MODE0  0 
#define    MODE1  1 
#define    MODE2  2 
#define    MODE3  3 
#define    MODE4  4 
#define    MODE5  5 
#define    MODE6  6 
#define    MODE7  7 


typedef struct{
    uint32 conf_gpmc_ad0 ;
    uint32 conf_gpmc_ad1 ;
    uint32 conf_gpmc_ad2 ;
    uint32 conf_gpmc_ad3 ;
    uint32 conf_gpmc_ad4 ;
    uint32 conf_gpmc_ad5 ;
    uint32 conf_gpmc_ad6 ;
    uint32 conf_gpmc_ad7 ;
    uint32 conf_gpmc_ad8 ;
    uint32 conf_gpmc_ad9 ;
    uint32 conf_gpmc_ad10 ;
    uint32 conf_gpmc_ad11 ;
    uint32 conf_gpmc_ad12 ;
    uint32 conf_gpmc_ad13 ;
    uint32 conf_gpmc_ad14 ;
    uint32 conf_gpmc_ad15 ;
    uint32 conf_gpmc_a0 ;
    uint32 conf_gpmc_a1 ;
    uint32 conf_gpmc_a2 ;
    uint32 conf_gpmc_a3 ;
    uint32 conf_gpmc_a4 ;
    uint32 conf_gpmc_a5 ;
    uint32 conf_gpmc_a6 ;
    uint32 conf_gpmc_a7 ;
    uint32 conf_gpmc_a8 ;
    uint32 conf_gpmc_a9 ;
    uint32 conf_gpmc_a10 ;
    uint32 conf_gpmc_a11 ;
    uint32 conf_gpmc_wait0 ;
    uint32 conf_gpmc_wpn ;
    uint32 conf_gpmc_ben1 ;
    uint32 conf_gpmc_csn0 ;
    uint32 conf_gpmc_csn1 ;
    uint32 conf_gpmc_csn2 ;
    uint32 conf_gpmc_csn3 ;
    uint32 conf_gpmc_clk ;
    uint32 conf_gpmc_advn_ale ;
    uint32 conf_gpmc_oen_ren ;
    uint32 conf_gpmc_wen ;
    uint32 conf_gpmc_ben0_cle ;
}CONF_GPMC_T ;


typedef struct{
    uint32 conf_lcd_data0 ;
    uint32 conf_lcd_data1 ;
    uint32 conf_lcd_data2 ;
    uint32 conf_lcd_data3 ;
    uint32 conf_lcd_data4 ;
    uint32 conf_lcd_data5 ;
    uint32 conf_lcd_data6 ;
    uint32 conf_lcd_data7 ;
    uint32 conf_lcd_data8 ;
    uint32 conf_lcd_data9 ;
    uint32 conf_lcd_data10 ;
    uint32 conf_lcd_data11 ;
    uint32 conf_lcd_data12 ;
    uint32 conf_lcd_data13 ;
    uint32 conf_lcd_data14 ;
    uint32 conf_lcd_data15 ;
    uint32 conf_lcd_vsync ;
    uint32 conf_lcd_hsync ;
    uint32 conf_lcd_pclk ;
    uint32 conf_lcd_ac_bias_en ;
}CONF_LCD_T ;


typedef struct{
    uint32 conf_mmc0_dat3 ;
    uint32 conf_mmc0_dat2 ;
    uint32 conf_mmc0_dat1 ;
    uint32 conf_mmc0_dat0 ;
    uint32 conf_mmc0_clk ;
    uint32 conf_mmc0_cmd ;
}CONF_MMC0_T;


typedef struct{
    uint32 conf_mii1_col ;
    uint32 conf_mii1_crs ;
    uint32 conf_mii1_rx_er ;
    uint32 conf_mii1_tx_en ;
    uint32 conf_mii1_rx_dv ;
    uint32 conf_mii1_txd3 ;
    uint32 conf_mii1_txd2 ;
    uint32 conf_mii1_txd1 ;
    uint32 conf_mii1_txd0 ;
    uint32 conf_mii1_tx_clk ;
    uint32 conf_mii1_rx_clk ;
    uint32 conf_mii1_rxd3 ;
    uint32 conf_mii1_rxd2 ;
    uint32 conf_mii1_rxd1 ;
    uint32 conf_mii1_rxd0 ;
    uint32 conf_rmii1_ref_clk ;
}CONF_MII_T;


typedef struct{
    uint32 conf_mdio ;
    uint32 conf_mdc ;
}CONF_MD_T;


typedef struct{
    uint32 conf_spi0_sclk ;
    uint32 conf_spi0_d0 ;
    uint32 conf_spi0_d1 ;
    uint32 conf_spi0_cs0 ;
    uint32 conf_spi0_cs1 ;
}CONF_SPI0_T;


typedef struct{
    uint32 conf_ecap0_in_pwm0_out ;
}CONF_ECAP0_T;


typedef struct{
    uint32 conf_uart0_ctsn ;
    uint32 conf_uart0_rtsn ;
    uint32 conf_uart0_rxd ;
    uint32 conf_uart0_txd ;
}CONF_UART0_T;
    

typedef struct{
    uint32 conf_uart1_ctsn ;
    uint32 conf_uart1_rtsn ;
    uint32 conf_uart1_rxd ;
    uint32 conf_uart1_txd ;
}CONF_UART1_T;


typedef struct{
    uint32 conf_i2c0_sda ;
    uint32 conf_i2c0_scl ;
}CONF_I2C0_T;


typedef struct{
    uint32 conf_mcasp0_aclkx ;
    uint32 conf_mcasp0_fsx ;
    uint32 conf_mcasp0_axr0 ;
    uint32 conf_mcasp0_ahclkr ;
    uint32 conf_mcasp0_aclkr ;
    uint32 conf_mcasp0_fsr ;
    uint32 conf_mcasp0_axr1 ;
    uint32 conf_mcasp0_ahclkx ;
}CONF_MCASP0_T;


typedef struct{
    uint32 conf_xdma_event_intr0 ;
    uint32 conf_xdma_event_intr1 ;
}CONF_XDMA_T;


typedef struct{
    uint32 conf_warmrstn ;
}CONF_WARM_RSTN_T;


typedef struct{
    uint32 conf_nnmi ;
}CONF_NNMI_T;


typedef struct{
    uint32 conf_tms ;
    uint32 conf_tdi ;
    uint32 conf_tdo ;
    uint32 conf_tck ;
    uint32 conf_trstn ;
}CONF_T_T;


typedef struct{
    uint32 conf_emu0 ;
    uint32 conf_emu1 ;
}CONF_EMU_T;


typedef struct{
    uint32 conf_rtc_pwronrstn ;
}CONF_RTC_PWRONRSTN_T;


typedef struct{
    uint32 conf_pmic_power_en ;
}CONF_PMIC_POWER_EN_T;


typedef struct{
    uint32 conf_ext_wakeup ;
}CONF_EXT_WAKEUP_T;


typedef struct{
    uint32 conf_usb0_drvvbus ;
}CONF_USB0_DRVVBUS_T;


typedef struct{
    uint32 conf_usb1_drvvbus ;
}CONF_USB1_DRVVBUS_T;


#endif