/**
 * Define Control Module pin config registers
 */  


#include "uart.h"

// The bootloader u-boot will initialize it, no need to call this function.                
void uart_Init(UART_REG_T *uartChannel ,STOP_BIT_T stopBit, PARITY_T parity, FLOW_CTRL_T flowControl){
    CONF_UART0_PTR->conf_uart0_txd = 00000000 | MODE0 ;

    //Setting conf_uart0_rxd :
    // bit 5 RXACTIVE = 1 : Receiver enabled
    // bit 4 PULLTYPESEL = 1 : Pullup selected
    CONF_UART0_PTR->conf_uart0_rxd = 00000000 | 1 << 5 | 1 << 4 | MODE0 ;
}


/** The bootloader  will initialize UART0 baudrate as 115200, no need to call this function 
 * after bootup.
 */                       
void uart_set_Baudrate(UART_REG_T *uartChannel, uint32_t baudrate){}


void uart_putC(UART_REG_T *uartChannel ,char byte){
    /** LSR_UART Register bit 5 : TXFIFOE
     * 
     * TXFIFOE.
     * 0h = Transmit hold register (TX FIFO) is not empty.
     * 1h = Transmit hold register (TX FIFO) is empty. The transmission is
     * not necessarily completed.
     */ 
    //If tx fifo is not empty, then wait.
    while( ( (uartChannel->LSR_UART) & (1 << 5) ) != (1 << 5) ) ;

    //Put a byte on THR register to send out the data
    uartChannel->THR = byte ;
}


// LSR_UART Register bit 0
// RXFIFOE R 0h RXFIFOE.
// 0h = No data in the receive FIFO.
// 1h = At least one data character in the RX FIFO.
uint8_t uart_getC(UART_REG_T *uartChannel)
{
    while( ((uartChannel->LSR_UART) & (0x01 << 0)) == 0 ) ;
    uint8_t byte = (uint8_t)uartChannel->RHR  ;
    
    return byte ;
}


void uart_tx_str(UART_REG_T *uartChannel ,char *str ,int32_t len){
    for(int32_t i = 0 ; i < len ; i++ ){
        uart_putC(uartChannel ,*(str + i)) ;
    }
}

