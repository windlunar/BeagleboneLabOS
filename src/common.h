
#ifndef __COMMON_H_
#define __COMMON_H_

typedef unsigned char uint8 ;
typedef unsigned short uint16 ;
typedef unsigned int uint32 ;
typedef unsigned long uint64 ;

typedef char int8 ;
typedef short int16 ;
typedef int int32 ;


typedef unsigned char uint8_t ;
typedef unsigned short uint16_t ;
typedef unsigned int uint32_t ;
typedef unsigned long uint64_t ;

typedef char int8_t ;
typedef short int16_t ;
typedef int int32_t ;



extern uint32 _estack;
extern void WRITE_REG32 (unsigned int address, unsigned int value);
extern unsigned int READ_REG32 (unsigned int address);

static inline void dataSyncBarrier(void)
{
	asm volatile("dsb\n");
}


extern void __attribute__((optimize("O0"))) delay(uint32_t nr_of_nops);


#define NULL ((void *)(0))

// ARM has 7 modes and banked registers
#define MODE_MASK   0x1f
#define USR_MODE    0x10
#define FIQ_MODE    0x11
#define IRQ_MODE    0x12
#define SVC_MODE    0x13
#define ABT_MODE    0x17
#define UND_MODE    0x1b
#define SYS_MODE    0x1f


#endif