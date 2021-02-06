
#ifndef __COMMON_H_
#define __COMMON_H_


#define FALSE	0
#define TRUE	1
/***************************************************************************************/
typedef unsigned char uint8  ;
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

/***************************************************************************************/

//extern uint32_t _estack;
extern void WRITE_REG32 (unsigned int address, unsigned int value);
extern unsigned int READ_REG32 (unsigned int address);
extern void __attribute__((optimize("O0"))) delay(uint32_t nr_of_nops);

/***************************************************************************************/
static inline void dataSyncBarrier(void)
{
	asm volatile("dsb\n");
}

/***************************************************************************************/

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

/***************************************************************************************/

// Rounding operations (efficient when n is a power of 2)
// Round down to the nearest multiple of n
//捨去到最接近n的倍數
//cprintf("%d\n",ROUNDDOWN(4100,4096));
//印出4096
#define ROUNDDOWN(a, n)			\
({									\
	uint32_t __a = (uint32_t) (a);	\
	(typeof(a)) (__a - __a % (n));	\
})


// Round up to the nearest multiple of n
//進位到最接近n的倍數
//cprintf("%d\n",ROUNDUP(4100,4096));
//印出8192
#define ROUNDUP(a, n)						\
({												\
	uint32_t __n = (uint32_t) (n);				\
	(typeof(a)) (ROUNDDOWN((uint32_t) (a) + __n - 1, __n));	\
})
/***************************************************************************************/
#define kpa2va(pa)  ((uint32_t)pa + 0x30000000)
#define kva2pa(va)	((uint32_t)va - 0x30000000)


#endif