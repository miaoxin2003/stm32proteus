#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x_gpio.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
// R-��ɫ
#define LED_RED_GPIO_PORT    	GPIOB			            /* GPIO�˿� */
#define LED_RED_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED_RED_GPIO_PIN		GPIO_Pin_5			        /* ���ӵ�LED_RED��GPIO */

// G-��ɫ
#define LED_GREEN_GPIO_PORT    	GPIOB			            /* GPIO�˿� */
#define LED_GREEN_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED_GREEN_GPIO_PIN		GPIO_Pin_6			        /* ���ӵ�LED_GREEN��GPIO */

// Y-��ɫ
#define LED_YELLOW_GPIO_PORT    GPIOB			            /* GPIO�˿� */
#define LED_YELLOW_GPIO_CLK 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED_YELLOW_GPIO_PIN		GPIO_Pin_7			        /* ���ӵ�LED_YELLOW��GPIO */

// B-��ɫ
#define LED_BLUE_GPIO_PORT    	GPIOD			            /* GPIO�˿� */
#define LED_BLUE_GPIO_CLK 	    RCC_APB2Periph_GPIOD		/* GPIO�˿�ʱ�� */
#define LED_BLUE_GPIO_PIN		GPIO_Pin_2			        /* ���ӵ�LED_BLUE��GPIO */

// BUZZER
#define BUZZER_GPIO_PORT        GPIOC			            /* GPIO�˿� */
#define BUZZER_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define BUZZER_GPIO_PIN		    GPIO_Pin_8			        /* ���ӵ�BUZZER��GPIO */

// FAN
#define FAN_GPIO_PORT           GPIOC			            /* GPIO�˿� */
#define FAN_GPIO_CLK 	        RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define FAN_GPIO_PIN		    GPIO_Pin_9			        /* ���ӵ�FAN��GPIO */

// RELAY
#define RELAY_GPIO_PORT         GPIOC			            /* GPIO�˿� */
#define RELAY_GPIO_CLK 	        RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define RELAY_GPIO_PIN		    GPIO_Pin_10			        /* ���ӵ�RELAY��GPIO */

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
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

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
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
