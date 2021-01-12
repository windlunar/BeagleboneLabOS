
#ifndef __GPIO_REG_H_
#define __GPIO_REG_H_

#include "../common.h"


#define GPIO1_BASE		0x4804C000
#define GPIO1_PTR           ((volatile GPIO_T *)GPIO1_BASE)

typedef struct{
    uint32_t GPIO_REVISION ;          // 0x0 
    uint32_t NON_USE_1;               // 0x4
    uint32_t NON_USE_2;               // 0x8
    uint32_t NON_USE_3;               // 0xc
    uint32_t GPIO_SYSCONFIG;          // 0x10
    uint32_t NON_USE_4;               // 0x14
    uint32_t NON_USE_5;               // 0x18
    uint32_t NON_USE_6;               // 0x1c
    uint32_t GPIO_EOI;                // 0x20
    uint32_t GPIO_IRQSTATUS_RAW_0;    // 0x24
    uint32_t GPIO_IRQSTATUS_RAW_1;    // 0x28
    uint32_t GPIO_IRQSTATUS_0;        // 0x2c
    uint32_t GPIO_IRQSTATUS_1;        // 0x30
    uint32_t GPIO_IRQSTATUS_SET_0;    // 0x34
    uint32_t GPIO_IRQSTATUS_SET_1;    // 0x38
    uint32_t GPIO_IRQSTATUS_CLR_0;    // 0x3c
    uint32_t GPIO_IRQSTATUS_CLR_1;    // 0x40
    uint32_t GPIO_IRQWAKEN_0;         // 0x44
    uint32_t GPIO_IRQWAKEN_1;         // 0x48
    uint32_t NON_USE_7;               // 0x4c
    uint32_t NON_USE_8;               // 0x50
    uint32_t NON_USE_9;               // 0x54
    uint32_t NON_USE_10;              // 0x58
    uint32_t NON_USE_11;              // 0x5c
    uint32_t NON_USE_12;              // 0x60
    uint32_t NON_USE_13;              // 0x64
    uint32_t NON_USE_14;              // 0x68
    uint32_t NON_USE_15;              // 0x6c
    uint32_t NON_USE_16;              // 0x70
    uint32_t NON_USE_17;              // 0x74
    uint32_t NON_USE_18;              // 0x78
    uint32_t NON_USE_19;              // 0x7c
    uint32_t NON_USE_20;              // 0x80
    uint32_t NON_USE_21;              // 0x84
    uint32_t NON_USE_22;              // 0x0x88
    uint32_t NON_USE_23;              // 0x8c
    uint32_t NON_USE_24;              // 0x90
    uint32_t NON_USE_25;              // 0x94
    uint32_t NON_USE_26;              // 0x98
    uint32_t NON_USE_27;              // 0x9c
    uint32_t NON_USE_28;              // 0xa0
    uint32_t NON_USE_29;              // 0xa4
    uint32_t NON_USE_30;              // 0xa8
    uint32_t NON_USE_31;              // 0xac
    uint32_t NON_USE_32;              // 0xb0
    uint32_t NON_USE_33;              // 0xb4
    uint32_t NON_USE_34;              // 0xb8
    uint32_t NON_USE_35;              // 0xbc
    uint32_t NON_USE_36;              // 0xc0
    uint32_t NON_USE_37;              // 0xc4
    uint32_t NON_USE_38;              // 0xc8    
    uint32_t NON_USE_39;              // 0xcc
    uint32_t NON_USE_40;              // 0xd0
    uint32_t NON_USE_41;              // 0xd4
    uint32_t NON_USE_42;              // 0xd8
    uint32_t NON_USE_43;              // 0xdc
    uint32_t NON_USE_44;              // 0xe0
    uint32_t NON_USE_45;              // 0xe4
    uint32_t NON_USE_46;              // 0xe8
    uint32_t NON_USE_47;              // 0xec
    uint32_t NON_USE_48;              // 0xf0
    uint32_t NON_USE_49;              // 0xf4
    uint32_t NON_USE_50;              // 0xf8
    uint32_t NON_USE_51;              // 0xfc
    uint32_t NON_USE_52;              // 0x100
    uint32_t NON_USE_53;              // 0x104
    uint32_t NON_USE_54;              // 0x108
    uint32_t NON_USE_55;              // 0x10c
    uint32_t NON_USE_56;              // 0x110
    uint32_t GPIO_SYSSTATUS;          // 0x114
    uint32_t NON_USE_57;              // 0x118
    uint32_t NON_USE_58;              // 0x11c
    uint32_t NON_USE_59;              // 0x120
    uint32_t NON_USE_60;              // 0x124
    uint32_t NON_USE_61;              // 0x128
    uint32_t NON_USE_62;              // 0x12c
    uint32_t GPIO_CTRL;               // 0x130
    uint32_t GPIO_OE;                 // 0x134
    uint32_t GPIO_DATAIN;             // 0x138
    uint32_t GPIO_DATAOUT;            // 0x13c
    uint32_t GPIO_LEVELDETECT0;       // 0x140
    uint32_t GPIO_LEVELDETECT1;       // 0x144
    uint32_t GPIO_RISINGDETECT;       // 0x148
    uint32_t GPIO_FALLINGDETECT;      // 0x14c
    uint32_t GPIO_DEBOUNCENABLE;      // 0x150
    uint32_t GPIO_DEBOUNCINGTIME;     // 0x154
    uint32_t NON_USE_63;              // 0x158
    uint32_t NON_USE_64;              // 0x15c
    uint32_t NON_USE_65;              // 0x160
    uint32_t NON_USE_66;              // 0x164
    uint32_t NON_USE_67;              // 0x168
    uint32_t NON_USE_68;              // 0x16c
    uint32_t NON_USE_69;              // 0x170
    uint32_t NON_USE_70;              // 0x174
    uint32_t NON_USE_71;              // 0x178
    uint32_t NON_USE_72;              // 0x17c
    uint32_t NON_USE_73;              // 0x180
    uint32_t NON_USE_74;              // 0x184
    uint32_t NON_USE_75;              // 0x188
    uint32_t NON_USE_76;              // 0x18c
    uint32_t GPIO_CLEARDATAOUT;       // 0x190
    uint32_t GPIO_SETDATAOUT;         // 0x194
}GPIO_T;


#endif