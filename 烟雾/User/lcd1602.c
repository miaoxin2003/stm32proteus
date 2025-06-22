#include "lcd1602.h"

#define CLR_RS (GPIO_ResetBits(RS_GPIO_PORT, RS_GPIO_PIN))
#define SET_RS (GPIO_SetBits(RS_GPIO_PORT, RS_GPIO_PIN))

#if _RW_PIN_
    #define CLR_RW (GPIO_ResetBits(RW_GPIO_PORT, RW_GPIO_PIN))
    #define SET_RW (GPIO_SetBits(RW_GPIO_PORT, RW_GPIO_PIN))
#endif

#define CLR_EN (GPIO_ResetBits(EN_GPIO_PORT, EN_GPIO_PIN))
#define SET_EN (GPIO_SetBits(EN_GPIO_PORT, EN_GPIO_PIN))

/**
* @brief  LCD1602_GPIO_Init
* @param  None
* @retval None
*/
void LCD_GPIO_Init(void)
{ //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //禁用jtag，不然写入程序和程序执行都会受影响
    RCC_APB2PeriphClockCmd(DQ_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RS_GPIO_CLK | EN_GPIO_CLK, ENABLE); //打开GPIO时钟
    #if _RW_PIN_
        RCC_APB2PeriphClockCmd(RW_GPIO_CLK, ENABLE); //打开GPIO时钟
    #endif
    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = RS_GPIO_PIN;
    GPIO_Init(RS_GPIO_PORT, &GPIO_InitStructure);

    #if _RW_PIN_
        GPIO_InitStructure.GPIO_Pin = RW_GPIO_PIN;
        GPIO_Init(RW_GPIO_PORT, &GPIO_InitStructure);
    #endif

    GPIO_InitStructure.GPIO_Pin = EN_GPIO_PIN;
    GPIO_Init(EN_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DQ_GPIO_PIN;
    GPIO_Init(DQ_GPIO_PORT, &GPIO_InitStructure);
}

/***********************************************
函数名称：LCD_DispStr
功    能：让液晶从某个位置起连续显示一个字符串
参    数：column--位置的列坐标
        row--位置的行坐标
        p_str--指向字符串存放位置的指针
返回值  ：无
***********************************************/
void LCD_DispStr(uchar column, uchar row, char *p_str)
{
    char *p_temp;
    uchar i;
    uchar n = 0;

    p_temp = p_str;
    while (*(p_str++) != '\0')
    {
        n++; //计算字符串有效字符的个数
    }
    for (i = 0; i < n; i++)
    {
        LCD_DispOneChar(column++, row, p_temp[i]);
        if (column == 0x10)
        {
            break;
        }
    }
}

/*******************************************
函数名称：DispNchar
功    能：让液晶从某个位置起连续显示N个字符
参    数：column--位置的列坐标
        row--位置的行坐标
        n--字符个数
        p_str--指向字符存放位置的指针
返回值  ：无
********************************************/
void LCD_DispNChar(uchar column, uchar row, uchar n, char *p_str)
{
    uchar i;

    for (i = 0; i < n; i++)
    {
        LCD_DispOneChar(column++, row, p_str[i]);
        if (column == 0x10)
        {
            column = 0;
            row ^= 1;
        }
    }
}

/*******************************************
函数名称：LCD_DispOneChar
功    能：在某个位置显示一个字符
参    数：column--位置的列坐标
        row--位置的行坐标
        data--显示的字符数据
返回值  ：无
********************************************/
void LCD_DispOneChar(uchar column, uchar row, char dat)
{
    LCD_LocateXY(column, row);
    LCD_WriteData(dat);
}

/*******************************************
函数名称：LCD_LocateXY
功    能：向液晶输入显示字符位置的坐标信息
参    数：column--位置的列坐标
        row--位置的行坐标
返回值  ：无
********************************************/
void LCD_LocateXY(uchar column, uchar row)
{
    uchar temp;

    temp = column & 0x0f;
    row &= 0x01;
    if (row)
    {
        temp |= 0x40; //如果在第2行
    }
    temp |= 0x80;

    LCD_WriteCommand(temp, 0);
}

/*******************************************
函数名称：LCD_Init
功    能：对1602液晶模块进行复位操作
参    数：无
返回值  ：无
********************************************/
void LCD_Init()
{
    //规定的复位操作
    LCD_WriteCommand(0x38, 0);
    DelayMs(5);
    LCD_WriteCommand(0x38, 0);
    DelayMs(5);
    LCD_WriteCommand(0x38, 0);
    DelayMs(5);

    LCD_WriteCommand(0x38, 1); //显示模式设置
    LCD_WriteCommand(0x08, 1); //显示关闭
    LCD_WriteCommand(0x01, 1); //显示清屏
    LCD_WriteCommand(0x06, 1); //写字符时整体不移动
    LCD_WriteCommand(0x0c, 1); //显示开，不开游标，不闪烁
}

/*------------------------------------------------
                清屏函数
------------------------------------------------*/
void LCD_Clear()
{
    LCD_WriteCommand(0x01, 0);
    DelayMs(5);
}

/*******************************************
函数名称：LCD_SetCursor
功    能：让光标在某个位置闪烁
参    数：column--位置的列坐标
        row--位置的行坐标
        onFlag-- 1打开光标,0关闭光标
返回值  ：无
********************************************/
void LCD_SetCursor(uchar column, uchar row, uchar onFlag)
{
    if (onFlag == 1)
    {
        LCD_WriteCommand(0x0F, 0);
        LCD_LocateXY(column, row);
    }
}

/*******************************************
函数名称：LCD_WriteCommand
功    能：向液晶模块写入命令
参    数：cmd--命令，
        chk--是否判忙的标志，1：判忙，0：不判
返回值  ：无
********************************************/
void LCD_WriteCommand(uchar cmd, uchar chk)
{

    if (chk)
    {
        LCD_WaitForEnable(); // 检测忙信号?
    }
    
    SET_EN;
    CLR_RS;

    #if _RW_PIN_
        CLR_RW;
    #endif

    DelayUs(200);

    GPIO_Write(DQ_GPIO_PORT, (cmd | (GPIO_ReadOutputData(DQ_GPIO_PORT) & (~DQ_GPIO_PIN))));

    SET_EN; //产生使能脉冲信号
    DelayUs(200);
    CLR_EN;
}

/*******************************************
函数名称：LCD_WriteData
功    能：向液晶显示的当前地址写入显示数据
参    数：dat--显示字符数据
返回值  ：无
********************************************/
void LCD_WriteData(uchar dat)
{
    CLR_EN;
    SET_RS;

    #if _RW_PIN_
        CLR_RW;
    #endif
    
    GPIO_Write(DQ_GPIO_PORT, (dat | (GPIO_ReadOutputData(DQ_GPIO_PORT) & (~DQ_GPIO_PIN))));

    DelayUs(200);
    SET_EN;
    //产生使能脉冲信号
    DelayUs(200);
    CLR_EN;
}

/*******************************************
函数名称：LCD_WaitForEnable
功    能：等待1602液晶完成内部操作
参    数：无
返回值  ：无
********************************************/
static void LCD_WaitForEnable(void)
{
    uint16_t later = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//第七Pin的工作方式为浮空输入模式，用于检测LCD1602的忙状态
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = DQ_GPIO_PIN;
    GPIO_Init( DQ_GPIO_PORT, &GPIO_InitStructure );

    CLR_RS;

    #if _RW_PIN_
        SET_RW;
    #endif

    DelayUs(200);
    SET_EN;
    DelayUs(200);
    //    while((DataIn&BUSY)!=0);
    while (((GPIO_ReadInputDataBit( DQ_GPIO_PORT, DQ_GPIO_PIN) & BUSY) != 0) && (later < 1000)) //检测忙标志
    {
        DelayUs(10);
        later++;
    }
    CLR_EN;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//使DQ_GPIO_PORT的状态还原成推挽模式
    GPIO_Init( DQ_GPIO_PORT, &GPIO_InitStructure );
}


