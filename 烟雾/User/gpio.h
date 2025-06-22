#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x_gpio.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
// R-红色
#define LED_RED_GPIO_PORT    	GPIOB			            /* GPIO端口 */
#define LED_RED_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED_RED_GPIO_PIN		GPIO_Pin_5			        /* 连接到LED_RED的GPIO */

// G-绿色
#define LED_GREEN_GPIO_PORT    	GPIOB			            /* GPIO端口 */
#define LED_GREEN_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED_GREEN_GPIO_PIN		GPIO_Pin_6			        /* 连接到LED_GREEN的GPIO */

// Y-黄色
#define LED_YELLOW_GPIO_PORT    GPIOB			            /* GPIO端口 */
#define LED_YELLOW_GPIO_CLK 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED_YELLOW_GPIO_PIN		GPIO_Pin_7			        /* 连接到LED_YELLOW的GPIO */

// B-蓝色
#define LED_BLUE_GPIO_PORT    	GPIOD			            /* GPIO端口 */
#define LED_BLUE_GPIO_CLK 	    RCC_APB2Periph_GPIOD		/* GPIO端口时钟 */
#define LED_BLUE_GPIO_PIN		GPIO_Pin_2			        /* 连接到LED_BLUE的GPIO */

// BUZZER
#define BUZZER_GPIO_PORT        GPIOC			            /* GPIO端口 */
#define BUZZER_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define BUZZER_GPIO_PIN		    GPIO_Pin_8			        /* 连接到BUZZER的GPIO */

// FAN
#define FAN_GPIO_PORT           GPIOC			            /* GPIO端口 */
#define FAN_GPIO_CLK 	        RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define FAN_GPIO_PIN		    GPIO_Pin_9			        /* 连接到FAN的GPIO */

// RELAY
#define RELAY_GPIO_PORT         GPIOC			            /* GPIO端口 */
#define RELAY_GPIO_CLK 	        RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define RELAY_GPIO_PIN		    GPIO_Pin_10			        /* 连接到RELAY的GPIO */

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define LED_RED(a)	if (a)	\
                    GPIO_SetBits(LED_RED_GPIO_PORT,LED_RED_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(LED_RED_GPIO_PORT,LED_RED_GPIO_PIN)

#define LED_GREEN(a)	if (a)	\
                    GPIO_SetBits(LED_GREEN_GPIO_PORT,LED_GREEN_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(LED_GREEN_GPIO_PORT,LED_GREEN_GPIO_PIN)

#define LED_YELLOW(a)	if (a)	\
                    GPIO_SetBits(LED_YELLOW_GPIO_PORT,LED_YELLOW_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(LED_YELLOW_GPIO_PORT,LED_YELLOW_GPIO_PIN)
                    
#define LED_BLUE(a)	if (a)	\
                    GPIO_SetBits(LED_BLUE_GPIO_PORT,LED_BLUE_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(LED_BLUE_GPIO_PORT,LED_BLUE_GPIO_PIN)                    

#define BUZZER(a)	if (a)	\
                    GPIO_SetBits(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(BUZZER_GPIO_PORT,BUZZER_GPIO_PIN)                    

#define FAN(a)	if (a)	\
                    GPIO_SetBits(FAN_GPIO_PORT,FAN_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(FAN_GPIO_PORT,FAN_GPIO_PIN)                    

#define RELAY(a)	if (a)	\
                    GPIO_SetBits(RELAY_GPIO_PORT,RELAY_GPIO_PIN);\
                    else		\
                    GPIO_ResetBits(RELAY_GPIO_PORT,RELAY_GPIO_PIN)                    

void GPIO_Config(void);

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED_RED_TOGGLE   digitalToggle(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN)
#define LED_RED_OFF      digitalHi(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN)
#define LED_RED_ON       digitalLo(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN)

#define LED_GREEN_TOGGLE   digitalToggle(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN)
#define LED_GREEN_OFF      digitalHi(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN)
#define LED_GREEN_ON       digitalLo(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN)

#define LED_YELLOW_TOGGLE   digitalToggle(LED_YELLOW_GPIO_PORT, LED_YELLOW_GPIO_PIN)
#define LED_YELLOW_OFF      digitalHi(LED_YELLOW_GPIO_PORT, LED_YELLOW_GPIO_PIN)
#define LED_YELLOW_ON       digitalLo(LED_YELLOW_GPIO_PORT, LED_YELLOW_GPIO_PIN)

#define LED_BLUE_TOGGLE   digitalToggle(LED_BLUE_GPIO_PORT, LED_BLUE_GPIO_PIN)
#define LED_BLUE_OFF      digitalHi(LED_BLUE_GPIO_PORT, LED_BLUE_GPIO_PIN)
#define LED_BLUE_ON       digitalLo(LED_BLUE_GPIO_PORT, LED_BLUE_GPIO_PIN)

#define BUZZER_TOGGLE digitalToggle(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_OFF    digitalHi(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_ON     digitalLo(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)

#define FAN_TOGGLE digitalToggle(FAN_GPIO_PORT, FAN_GPIO_PIN)
#define FAN_OFF    digitalHi(FAN_GPIO_PORT, FAN_GPIO_PIN)
#define FAN_ON     digitalLo(FAN_GPIO_PORT, FAN_GPIO_PIN)

#define RELAY_TOGGLE digitalToggle(RELAY_GPIO_PORT, RELAY_GPIO_PIN)
#define RELAY_OFF    digitalHi(RELAY_GPIO_PORT, RELAY_GPIO_PIN)
#define RELAY_ON     digitalLo(RELAY_GPIO_PORT, RELAY_GPIO_PIN)

#endif /* __BSP_GPIO_H */
