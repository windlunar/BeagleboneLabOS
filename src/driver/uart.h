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
        uint32_t THR ;
        uint32_t RHR ;
        uint32_t DLL ;
    };

    union{
        uint32_t IER_IRDA ;
        uint32_t IER_CIR ;
        uint32_t IER_UART ;
        uint32_t DLH ;
    };
    
    union{
        uint32_t EFR ;
        uint32_t IIR_UART ;
        uint32_t IIR_CIR ;
        uint32_t FCR ;
        uint32_t IIR_IRDA ;
    };

    uint32_t LCR ;

    union{
        uint32_t MCR ;
        uint32_t XON1_ADDR1 ;
    };

    union{
        uint32_t XON2_ADDR2 ;
        uint32_t LSR_CIR ;
        uint32_t LSR_IRDA ;
        uint32_t LSR_UART ;
    };

    union{
        uint32_t TCR ;
        uint32_t MSR ;
        uint32_t XOFF1 ;
    };

    union{
        uint32_t SPR ;
        uint32_t TLR ;
        uint32_t XOFF2 ;
    };

    uint32_t MDR1 ;
    uint32_t MDR2 ;

    union{
        uint32_t TXFLL ;
        uint32_t SFLSR ;
    };

    union{
        uint32_t RESUME ;
        uint32_t TXFLH ;
    };
      
    union{
        uint32_t RXFLL ;
        uint32_t SFREGL ;
    };

    union{
        uint32_t SFREGH ;
        uint32_t RXFLH ;
    };

    union{
        uint32_t BLR ;
        uint32_t UASR ;
    };

    uint32_t ACREG ;
    uint32_t SCR ;
    uint32_t SSR ;
    uint32_t EBLR ;
    uint32_t MVR ;
    uint32_t SYSC ;
    uint32_t SYSS ;
    uint32_t WER ;
    uint32_t CFPS ;
    uint32_t RXFIFO_LVL ;
    uint32_t TXFIFO_LVL ;
    uint32_t IER2 ;
    uint32_t ISR2 ;
    uint32_t FREQ_SEL ;
    uint32_t MDR3 ;
    uint32_t TX_DMA_THRESHOLD ;
    
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
void uart_set_Baudrate(UART_REG_T *uartChannel, uint32_t baudrate);
void uart_putC(UART_REG_T *uartChannel ,char byte) ;
void uart_tx_str(UART_REG_T *uartChannel ,char *str ,int32_t len) ;

#endif