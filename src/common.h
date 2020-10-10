
#ifndef __COMMON_H_
#define __COMMON_H_

typedef unsigned char uint8 ;
typedef unsigned short uint16 ;
typedef unsigned int uint32 ;

typedef char int8 ;
typedef short int16 ;
typedef int int32 ;



extern uint32 _estack;



extern void WRITE_REG32 (unsigned int address, unsigned int value);
extern unsigned int READ_REG32 (unsigned int address);

extern unsigned int READ_CPSR ();
extern unsigned int READ_SP ();
extern unsigned int READ_CP15_c1 ();
extern unsigned int READ_VECTOR_BASE ();


#endif