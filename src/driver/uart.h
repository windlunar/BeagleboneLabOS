/**
 * Define Control Module pin config registers
 */  

#ifndef __UART_H_
#define __UART_H_

#include "../common.h"
#include "cm_conf.h"

#define UART0_BASE      0x44E09000
#define UART1_BASE      0x48022000
#define UART2_BASE      0x48024000
#define UART3_BASE      0x481A6000
#define UART4_BASE      0x481A8000
#define UART5_BASE      0x481AA000

#define UART0_PTR       ((UART_REG_T *)UART0_BASE)
#define UART1_PTR       ((UART_REG_T *)UART1_BASE)
#define UART2_PTR       ((UART_REG_T *)UART2_BASE)
#define UART3_PTR       ((UART_REG_T *)UART3_BASE)
#define UART4_PTR       ((UART_REG_T *)UART4_BASE)
#define UART5_PTR       ((UART_REG_T *)UART5_BASE)

/** Define UART related structure*/
typedef struct{
    union{
        uint32 THR ;
        uint32 RHR ;
        uint32 DLL ;
    };

    union{
        uint32 IER_IRDA ;
        uint32 IER_CIR ;
        uint32 IER_UART ;
        uint32 DLH ;
    };
    
    union{
        uint32 EFR ;
        uint32 IIR_UART ;
        uint32 IIR_CIR ;
        uint32 FCR ;
        uint32 IIR_IRDA ;
    };

    uint32 LCR ;

    union{
        uint32 MCR ;
        uint32 XON1_ADDR1 ;
    };

    union{
        uint32 XON2_ADDR2 ;
        uint32 LSR_CIR ;
        uint32 LSR_IRDA ;
        uint32 LSR_UART ;
    };

    union{
        uint32 TCR ;
        uint32 MSR ;
        uint32 XOFF1 ;
    };

    union{
        uint32 SPR ;
        uint32 TLR ;
        uint32 XOFF2 ;
    };

    uint32 MDR1 ;
    uint32 MDR2 ;

    union{
        uint32 TXFLL ;
        uint32 SFLSR ;
    };

    union{
        uint32 RESUME ;
        uint32 TXFLH ;
    };
      
    union{
        uint32 RXFLL ;
        uint32 SFREGL ;
    };

    union{
        uint32 SFREGH ;
        uint32 RXFLH ;
    };

    union{
        uint32 BLR ;
        uint32 UASR ;
    };

    uint32 ACREG ;
    uint32 SCR ;
    uint32 SSR ;
    uint32 EBLR ;
    uint32 MVR ;
    uint32 SYSC ;
    uint32 SYSS ;
    uint32 WER ;
    uint32 CFPS ;
    uint32 RXFIFO_LVL ;
    uint32 TXFIFO_LVL ;
    uint32 IER2 ;
    uint32 ISR2 ;
    uint32 FREQ_SEL ;
    uint32 MDR3 ;
    uint32 TX_DMA_THRESHOLD ;
    
}UART_REG_T;

typedef enum{
   STOP1,
   STOP1_5,
   STOP2
}STOP_BIT_T;

typedef enum
{
   NONE,
   EVEN,
   ODD
}PARITY_T;

typedef enum{
   OFF = 0,
   ON = 1
}FLOW_CTRL_T;

void uart_Init(UART_REG_T *uartChannel ,STOP_BIT_T stopBit, PARITY_T parity, FLOW_CTRL_T flowControl);
void uart_set_Baudrate(UART_REG_T *uartChannel, uint32 baudrate);
void uart_putC(UART_REG_T *uartChannel ,char byte) ;
void uart_tx_str(UART_REG_T *uartChannel ,char *str ,int32 len) ;

#endif