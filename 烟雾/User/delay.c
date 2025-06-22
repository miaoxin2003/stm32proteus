#include "delay.h"

static uint32_t TimingDelay;

/**
* @brief  wait n us
* @param  n
* @retval None
*/
void wait(__IO uint32_t n)
{
    while (n--)
        ;
}

//static u8  fac_us=0;//us延时倍乘数
//static u16 fac_ms=0;//ms延时倍乘数

//void Delay_Init(void)
//{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//选择外部时钟  HCLK/8
//	fac_us=SystemCoreClock/1000000;
//	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数
//}

//void DelayUs10x(uint32_t nus)
//{
//	u32 temp;
//	SysTick->LOAD=nus*fac_us*10; //时间加载
//	SysTick->VAL=0x00;        //清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
//	SysTick->VAL =0X00;       //清空计数器
//}

//void DelayMs(uint16_t nms)
//{
//	u32 temp;
//	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;           //清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数
//	do
//	{
//		temp=SysTick->CTRL;
//	}
//	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
//	SysTick->VAL =0X00;       //清空计数器
//}

///**
//* @brief  DelayUs(__IO uint32_t nTime)
//* @param  nTime
//* @retval None
//*/
//void DelayUs(__IO uint32_t nTime)
//{
//    TimingDelay = nTime; //1us

//    if (SysTick_Config(SystemCoreClock / 1000000)) //定时10us
//    {
//        while (1);
//    }
//    while (TimingDelay != 0);
//    // 关闭SysTick定时器
//    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//}

/**
 * @brief  DelayUs10x(__IO uint32_t nTime)
 * @param  nTime
 * @retval None
 */
void DelayUs10x(__IO uint32_t nTime)
{
    TimingDelay = nTime; //10us

    if (SysTick_Config(SystemCoreClock / 100000)) //定时10us
    {
        while (1)
            ;
    }
    while (TimingDelay != 0)
        ;
    // 关闭SysTick定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief  DelayMs(__IO uint16_t nTime)
 * @param  nTime
 * @retval None
 */
void DelayMs(__IO uint16_t nTime)
{
    TimingDelay = nTime; //1ms

    if (SysTick_Config(SystemCoreClock / 1000)) //定时1ms
    {
        while (1)
            ;
    }
    while (TimingDelay != 0)
        ;
    // 关闭SysTick定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief  TimingDelay_Decrement()
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

/**
 * @brief  SysTick_Handler,系统定时器中断处理
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}
