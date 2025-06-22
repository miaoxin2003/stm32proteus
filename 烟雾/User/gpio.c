#include "gpio.h"   

/**
 * @brief  初始化控制LED的IO
 * @param  无
 * @retval 无
 */
void GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( LED_RED_GPIO_CLK | LED_GREEN_GPIO_CLK | LED_YELLOW_GPIO_CLK | LED_BLUE_GPIO_CLK\
								| BUZZER_GPIO_CLK | FAN_GPIO_CLK | RELAY_GPIO_CLK, ENABLE);	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED_RED_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED_RED_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED_GREEN_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED_YELLOW_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(LED_YELLOW_GPIO_PORT, &GPIO_InitStructure);
        
        /*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = LED_BLUE_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(LED_BLUE_GPIO_PORT, &GPIO_InitStructure);

		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = FAN_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = RELAY_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(RELAY_GPIO_PORT, &GPIO_InitStructure);

        LED_RED_OFF;
        LED_GREEN_OFF;
        LED_YELLOW_OFF;
        LED_BLUE_OFF;
		BUZZER_OFF;
		FAN_OFF;
		RELAY_OFF;
}
/*********************************************END OF FILE**********************/


