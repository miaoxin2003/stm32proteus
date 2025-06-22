#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"

#define SYSCLK 8 //指明CPU工作频率为4MHz
#define A 9 //一次循环所花的周期数
#define B 3 //调用、初始化、返回总共所用的周期数
#define DelayUs(nus) wait(((nus)*(SYSCLK)-(B))/(A))

void TimingDelay_Decrement(void);
void wait(uint32_t n);
// void DelayUs(__IO uint32_t nTime);
void DelayUs10x(__IO uint32_t nTime);
void DelayMs(__IO uint16_t nTime);
// void Delay_Init(void);

#endif /* __BSP_DELAY_H */

