#ifndef __LCD1602_H
#define __LCD1602_H

#include "stm32f10x_gpio.h"
#include "delay.h"

#define uchar unsigned char

/***********************************引脚定义********************************************/
#define BUSY 0x0080		//忙标志
#define _RW_PIN_ 1

#define RS_GPIO_PORT GPIOB               /* GPIO端口 */
#define RS_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define RS_GPIO_PIN GPIO_Pin_8          /* 连接到RS的GPIO */

#if _RW_PIN_
    #define RW_GPIO_PORT GPIOC               /* GPIO端口 */
    #define RW_GPIO_CLK RCC_APB2Periph_GPIOC /* GPIO端口时钟 */
    #define RW_GPIO_PIN GPIO_Pin_13           /* 连接到RW的GPIO */
#endif

#define EN_GPIO_PORT GPIOB               /* GPIO端口 */
#define EN_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define EN_GPIO_PIN GPIO_Pin_9           /* 连接到EN的GPIO */

#define DQ_GPIO_PORT GPIOA               /* GPIO端口 */
#define DQ_GPIO_CLK RCC_APB2Periph_GPIOA /* GPIO端口时钟 */
#define DQ_GPIO_PIN (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | \
                    GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
                    GPIO_Pin_6 | GPIO_Pin_7)        /* 连接到DQ的GPIO */

/***********************************函数定义********************************************/
void LCD_GPIO_Init(void);
void LCD_DispStr(uchar column, uchar row, char *p_str);
void LCD_DispNChar(uchar column, uchar row, uchar n, char *p_str);
void LCD_DispOneChar(uchar column, uchar row, char dat);
void LCD_LocateXY(uchar column, uchar row);
void LCD_Init(void);
void LCD_WriteCommand(uchar cmd, uchar chk);
void LCD_WriteData(uchar dat);
void LCD_Clear(void);
void LCD_SetCursor(uchar column, uchar row, uchar onFlag);
static void LCD_WaitForEnable(void);

#endif /* __BSP_LCD1602_H */

