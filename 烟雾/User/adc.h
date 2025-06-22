#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"


#define    ADCx                          ADC1
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOB
#define    ADC_PORT                      GPIOB

// PB0-Õ®µ¿8 ∂¿¡¢IO
#define    ADC_PIN                       GPIO_Pin_0
#define    ADC_CHANNEL                   ADC_Channel_8
#define    ADC_DMA_CHANNEL               DMA1_Channel1

void ADCx_Init(void);

#endif /* __ADC_H */
