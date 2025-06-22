#include "GeneralTim.h"
#include "main.h"

// 中断优先级配置
static void GENERAL_TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ;
    // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM2_Mode_Config(void)
{
    // 开启定时器时钟,即内部时钟CK_INT=8M
	GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 开启定时器时钟,即内部时钟CK_INT=8M
    GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK, ENABLE);
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
#ifdef _SIMULATION_
    TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM2_Period + 1;
#else
    TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM2_Period;
#endif
    // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM2_Prescaler;
    // 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM2, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM2, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(GENERAL_TIM2, TIM_IT_Update, ENABLE);

    // 使能计数器
    TIM_Cmd(GENERAL_TIM2, ENABLE);
}

void GENERAL_TIM_Init(void)
{

    GENERAL_TIM2_NVIC_Config();
    GENERAL_TIM2_Mode_Config();

}


