#ifndef __GENERALTIME_H
#define __GENERALTIME_H


#include "stm32f10x.h"


/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可
#define GENERAL_TIM2_S    1

#if  GENERAL_TIM2_S

#define            GENERAL_TIM2                   TIM2
#define            GENERAL_TIM2_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM2_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM2_Period            (10000-1)
#define            GENERAL_TIM2_Prescaler         (8-1)
#define            GENERAL_TIM2_IRQ               TIM2_IRQn
#define            GENERAL_TIM2_IRQHandler        TIM2_IRQHandler

#endif

/**************************函数声明********************************/

void GENERAL_TIM_Init(void);


#endif	/* __BSP_GENERALTIME_H */


