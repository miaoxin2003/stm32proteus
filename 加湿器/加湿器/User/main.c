#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "dht11.h"
#include "Servo.h"
#include "AD.h"

u8 temp;//温度
u8 humi;//湿度
float Dist;
u16 ADValue;

u8 Humi_H=70,Humi_L=50;           //湿度高低值
u8 Dist_H=30,Dist_L=5;            //水位上下限
int main(void)
{
    OLED_Init();
	DHT11_Init();
	Servo_Init();
	AD_Init();	
	OLED_ShowChinese(1,1, 0);
	OLED_ShowChinese(1,2, 1);
	OLED_ShowChar(1, 5, ':');
	OLED_ShowChar(1,9,  'C');	
	OLED_ShowChinese(2,1, 2);
	OLED_ShowChinese(2,2, 1);

	OLED_ShowChar(2, 5, ':');	
	OLED_ShowChar(2,9, '%');
	
	
  while (1)
  {
			DHT11_Read_Data(&temp,&humi);
	    ADValue = AD_GetValue(ADC_Channel_1 );// 获取水位数据
	    Dist = (float)ADValue / 4095 * 50;// 将 ADC 值转换为水位，假设最大水位为 50cm
			OLED_ShowNum(1,6,temp,2);// 显示温度
			OLED_ShowNum(2,6,humi,2);// 显示湿度
	    OLED_ShowNum(3,6,Dist,2);// 显示水位
	  if(humi>0&&humi<=Humi_L&&Dist>Dist_L&&Dist<=Dist_H)        //满足水位温湿度条件，开关最大
			{
				Servo_SetAngle(180);	  // 湿度低、水位适中，开到最大 
			} 
		if(humi>Humi_L&&humi<=Humi_H&&Dist>Dist_L&&Dist<=Dist_H)   //满足水位温湿度条件，开关中
			{
				Servo_SetAngle(120);		// 湿度适中、水位适中，开到中等	
			}
			if(humi>Humi_H&&humi<=85&&Dist>Dist_L&&Dist<=Dist_H)     //满足水位温湿度条件，开关小
			{
				 Servo_SetAngle(60);		// 湿度高、水位适中，开到最小		
			}
			if(humi>85|Dist<=Dist_L|Dist>=Dist_H)                    //湿度太大或者水位超过最大小于最小值关机
			{
				Servo_SetAngle(0);	// 湿度过高或水位过低/过高，关闭伺服电机
			}
  }
}
 
	
