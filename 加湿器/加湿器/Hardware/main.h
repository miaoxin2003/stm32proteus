#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "stdio.h"

typedef union _CREATBYTE_
{
    uint8_t Byte;
    struct
    {
        uint8_t b0 : 1;
        uint8_t b1 : 1;
        uint8_t b2 : 1;
        uint8_t b3 : 1;
        uint8_t b4 : 1;
        uint8_t b5 : 1;
        uint8_t b6 : 1;
        uint8_t b7 : 1;
    } bits;
}CreatByte;

#ifndef __MAIN_C
    extern volatile CreatByte Flag;
    extern volatile uint16_t ADC_ConvertedValue;
#endif

#define dispFlag Flag.bits.b0

#endif /* __MAIN_H */
