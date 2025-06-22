#define __MAIN_C
#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "gpio.h"
#include "clkconfig.h"
#include "delay.h"
#include "lcd1602.h"
#include "GeneralTim.h"
#include "usart.h"
#include "adc.h"
#include "key.h"

volatile CreatByte Flag;

volatile uint16_t ADC_ConvertedValue;
int16_t Num=0;
char dis0[16];         //数组暂存
char dis1[16];         //数组暂存
uint16_t midVolt; //ad采集电压

int main(void)
{
    dispFlag = 1;

    // 使用HSI，SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],最高是64MH
    HSI_SetSysClock(RCC_PLLMul_2); //使用内部8MHz晶振，并设置PLL输出为8MHz
		Key_Init();
    GENERAL_TIM_Init();
    DelayMs(250);
    ADCx_Init();
    DelayMs(250);
    USART_Config();
    DelayMs(250);
    GPIO_Config();
    DelayMs(250);
    
    
    LCD_GPIO_Init();
    LCD_Init();
    LCD_Clear();
    DelayMs(250);
    LCD_DispStr(0, 0, "    Welcome!    ");
    DelayMs(1000);
    LCD_Clear();
    LCD_DispStr(0, 0, "    Detector    ");
   
    
    printf("ready ok!\r\n");
    LED_RED_ON;
    
    while (1)
    {
			  Num += KeyNum_Get();
				if (Num>=1)
				{
					if(Num%2==0)
					{
					FAN_OFF
					}
					else
					{
					FAN_ON;
					}
				}
        if (dispFlag == 1)
        {
            dispFlag = 0;

        #ifdef _SIMULATION_
            midVolt = (uint16_t)((float)ADC_ConvertedValue * 100 / 4096);                 //读取AD检测到的值转化为百分比
        #else
            midVolt = (uint16_t)(((float)ADC_ConvertedValue * 3.3 / 4096 - 0.359) * 100 / (3.3 - 0.359)); //读取AD检测到的值转化为百分比
        #endif
            printf("CONC. :%2d%%\r\n", (uint16_t)midVolt); //串口发送
            sprintf(dis1, "  Percent :%02d%%   ", (uint16_t)midVolt); //显示当前检测值
            LCD_DispStr(0, 1, dis1);
        }

        if (midVolt < 30)
        {
            FAN_OFF;        //关 风扇
            BUZZER_OFF;     //关 蜂鸣器
            RELAY_OFF;      //关闭继电器打开气阀
            LED_GREEN_ON;   //开 绿灯
            LED_YELLOW_OFF; //关 黄灯
            LED_RED_OFF;    //关 红灯
        }
        else if (midVolt < 50)
        {
            FAN_OFF;       //关 风扇
            BUZZER_OFF;    //关 蜂鸣器
            RELAY_ON;      //打开继电器关闭气阀
            LED_GREEN_OFF; //关 绿灯
            LED_YELLOW_ON; //开 黄灯
            LED_RED_OFF;   //关 红灯
        }
        else
        {
            FAN_ON;        //开 风扇
            BUZZER_ON;     //开 蜂鸣器
            RELAY_ON;      //打开继电器关闭气阀
            LED_GREEN_OFF; //关 绿灯
            LED_YELLOW_OFF;//关 黄灯
            LED_RED_ON;    //开 红灯
        }
        DelayMs(500);
    }
		
}

/*********************************************END OF FILE**********************/
