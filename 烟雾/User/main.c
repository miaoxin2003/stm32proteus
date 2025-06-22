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
char dis0[16];         //�����ݴ�
char dis1[16];         //�����ݴ�
uint16_t midVolt; //ad�ɼ���ѹ

int main(void)
{
    dispFlag = 1;

    // ʹ��HSI��SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],�����64MH
    HSI_SetSysClock(RCC_PLLMul_2); //ʹ���ڲ�8MHz���񣬲�����PLL���Ϊ8MHz
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
            midVolt = (uint16_t)((float)ADC_ConvertedValue * 100 / 4096);                 //��ȡAD��⵽��ֵת��Ϊ�ٷֱ�
        #else
            midVolt = (uint16_t)(((float)ADC_ConvertedValue * 3.3 / 4096 - 0.359) * 100 / (3.3 - 0.359)); //��ȡAD��⵽��ֵת��Ϊ�ٷֱ�
        #endif
            printf("CONC. :%2d%%\r\n", (uint16_t)midVolt); //���ڷ���
            sprintf(dis1, "  Percent :%02d%%   ", (uint16_t)midVolt); //��ʾ��ǰ���ֵ
            LCD_DispStr(0, 1, dis1);
        }

        if (midVolt < 30)
        {
            FAN_OFF;        //�� ����
            BUZZER_OFF;     //�� ������
            RELAY_OFF;      //�رռ̵���������
            LED_GREEN_ON;   //�� �̵�
            LED_YELLOW_OFF; //�� �Ƶ�
            LED_RED_OFF;    //�� ���
        }
        else if (midVolt < 50)
        {
            FAN_OFF;       //�� ����
            BUZZER_OFF;    //�� ������
            RELAY_ON;      //�򿪼̵����ر�����
            LED_GREEN_OFF; //�� �̵�
            LED_YELLOW_ON; //�� �Ƶ�
            LED_RED_OFF;   //�� ���
        }
        else
        {
            FAN_ON;        //�� ����
            BUZZER_ON;     //�� ������
            RELAY_ON;      //�򿪼̵����ر�����
            LED_GREEN_OFF; //�� �̵�
            LED_YELLOW_OFF;//�� �Ƶ�
            LED_RED_ON;    //�� ���
        }
        DelayMs(500);
    }
		
}

/*********************************************END OF FILE**********************/
