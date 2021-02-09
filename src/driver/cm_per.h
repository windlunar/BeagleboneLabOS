
#ifndef __CM_PER_H_
#define __CM_PER_H_


#include "../common.h"


#define CM_PER_BASE                 (0x44e00000)
#define CM_PER_BASE_PTR             ((volatile CM_PER_T *)CM_PER_BASE)

typedef struct{
    uint32_t CM_PER_L4LS_CLKSTCTRL ;      /* 0x0 */ 
    uint32_t CM_PER_L3S_CLKSTCTRL;        /* 0x4 */
    uint32_t NON_USE_1;                   /* 0x8 */
    uint32_t CM_PER_L3_CLKSTCTRL;         /* 0xc */
    uint32_t NON_USE_2;                   /* 0x10 */
    uint32_t CM_PER_CPGMAC0_CLKCTRL ;     /* 0x14 */
    uint32_t CM_PER_LCDC_CLKCTRL;         /* 0x18 */
    uint32_t CM_PER_USB0_CLKCTRL;         /* 0x1c */
    uint32_t NON_USE_3;                   /* 0x20 */
    uint32_t CM_PER_TPTC0_CLKCTRL;        /* 0x24 */
    uint32_t CM_PER_EMIF_CLKCTRL;         /* 0x28 */
    uint32_t CM_PER_OCMCRAM_CLKCTRL;      /* 0x2c */
    uint32_t CM_PER_GPMC_CLKCTRL;         /* 0x30 */
    uint32_t CM_PER_MCASP0_CLKCTRL;       /* 0x34 */
    uint32_t CM_PER_UART5_CLKCTRL;        /* 0x38 */
    uint32_t CM_PER_MMC0_CLKCTRL;         /* 0x3c */
    uint32_t CM_PER_ELM_CLKCTRL;          /* 0x40 */
    uint32_t CM_PER_I2C2_CLKCTRL;         /* 0x44 */
    uint32_t CM_PER_I2C1_CLKCTRL;         /* 0x48 */
    uint32_t CM_PER_SPI0_CLKCTRL;         /* 0x4c */
    uint32_t CM_PER_SPI1_CLKCTRL;         /* 0x50 */
    uint32_t NON_USE_4;                   /* 0x54 */
    uint32_t NON_USE_5;                   /* 0x58 */
    uint32_t NON_USE_6;                   /* 0x5c */
    uint32_t CM_PER_L4LS_CLKCTRL ;        /* 0x60 */
    uint32_t NON_USE_28;                  /* 0x64 */
    uint32_t CM_PER_MCASP1_CLKCTRL;       /* 0x68 */
    uint32_t CM_PER_UART1_CLKCTRL;        /* 0x6c */
    uint32_t CM_PER_UART2_CLKCTRL ;       /* 0x70 */
    uint32_t CM_PER_UART3_CLKCTRL;        /* 0x74 */
    uint32_t CM_PER_UART4_CLKCTRL;        /* 0x78 */
    uint32_t CM_PER_TIMER7_CLKCTRL;       /* 0x7c */
    uint32_t CM_PER_TIMER2_CLKCTRL;       /* 0x80 */
    uint32_t CM_PER_TIMER3_CLKCTRL;       /* 0x84 */
    uint32_t CM_PER_TIMER4_CLKCTRL;       /* 0x88 */
    uint32_t NON_USE_7;                   /* 0x8c */
    uint32_t NON_USE_8;                   /* 0x90 */
    uint32_t NON_USE_9;                   /* 0x94 */
    uint32_t NON_USE_10;                  /* 0x98 */
    uint32_t NON_USE_11;                  /* 0x9c */
    uint32_t NON_USE_12;                  /* 0xa0 */
    uint32_t NON_USE_13;                  /* 0xa4 */
    uint32_t NON_USE_14;                  /* 0xa8 */
    uint32_t CM_PER_GPIO1_CLKCTRL;        /* 0xac */
    uint32_t CM_PER_GPIO2_CLKCTRL;        /* 0xb0 */
    uint32_t CM_PER_GPIO3_CLKCTRL;        /* 0xb4 */
    uint32_t NON_USE_15;                  /* 0xb8 */
    uint32_t CM_PER_TPCC_CLKCTRL;         /* 0xbc */
    uint32_t CM_PER_DCAN0_CLKCTRL;        /* 0xc0 */
    uint32_t CM_PER_DCAN1_CLKCTRL;        /* 0xc4 */
    uint32_t NON_USE_16;                  /* 0xc8 */    
    uint32_t CM_PER_EPWMSS1_CLKCTRL;      /* 0xcc */
    uint32_t NON_USE_18;                  /* 0xd0 */
    uint32_t CM_PER_EPWMSS0_CLKCTRL;      /* 0xd4 */
    uint32_t CM_PER_EPWMSS2_CLKCTRL;      /* 0xd8 */
    uint32_t CM_PER_L3_INSTR_CLKCTRL;     /* 0xdc */
    uint32_t CM_PER_L3_CLKCTRL;           /* 0xe0 */
    uint32_t CM_PER_IEEE5000_CLKCTRL;     /* 0xe4 */
    uint32_t CM_PER_PRU_ICSS_CLKCTRL;     /* 0xe8 */
    uint32_t CM_PER_TIMER5_CLKCTRL;       /* 0xec */
    uint32_t CM_PER_TIMER6_CLKCTRL;       /* 0xf0 */
    uint32_t CM_PER_MMC1_CLKCTRL;         /* 0xf4 */
    uint32_t CM_PER_MMC2_CLKCTRL;         /* 0xf8 */
    uint32_t CM_PER_TPTC1_CLKCTRL;        /* 0xfc */
    uint32_t CM_PER_TPTC2_CLKCTRL;        /* 0x100 */
    uint32_t NON_USE_19 ;                 /* 0x104 */
    uint32_t NON_USE_20 ;                 /* 0x108 */
    uint32_t CM_PER_SPINLOCK_CLKCTRL;     /* 0x10c */
    uint32_t CM_PER_MAILBOX0_CLKCTRL;     /* 0x110 */
    uint32_t NON_USE_21;                  /* 0x114 */
    uint32_t NON_USE_22;                  /* 0x118 */
    uint32_t CM_PER_L4HS_CLKSTCTRL;       /* 0x11c */
    uint32_t CM_PER_L4HS_CLKCTRL;         /* 0x120 */
    uint32_t NON_USE_23;                  /* 0x124 */
    uint32_t NON_USE_24;                  /* 0x128 */
    uint32_t CM_PER_OCPWP_L3_CLKSTCTRL;   /* 0x12c */
    uint32_t CM_PER_OCPWP_CLKCTRL;        /* 0x130 */
    uint32_t NON_USE_25;                  /* 0x134 */
    uint32_t NON_USE_26;                  /* 0x138 */
    uint32_t NON_USE_27;                  /* 0x13c */
    uint32_t CM_PER_PRU_ICSS_CLKSTCTRL;   /* 0x140 */
    uint32_t CM_PER_CPSW_CLKSTCTRL;       /* 0x144 */
    uint32_t CM_PER_LCDC_CLKSTCTRL;       /* 0x148 */
    uint32_t CM_PER_CLKDIV32K_CLKCTRL;    /* 0x14c */
    uint32_t CM_PER_CLK_24MHZ_CLKSTCTRL;  /* 0x150 */
}CM_PER_T;


#endif