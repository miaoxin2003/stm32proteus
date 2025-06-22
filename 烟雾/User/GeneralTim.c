#include "GeneralTim.h"
#include "main.h"

// �ж����ȼ�����
static void GENERAL_TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ;
    // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM2_Mode_Config(void)
{
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=8M
	GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK,ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=8M
    GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
#ifdef _SIMULATION_
    TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM2_Period + 1;
#else
    TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM2_Period;
#endif
    // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM2_Prescaler;
    // ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM2, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM2, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(GENERAL_TIM2, TIM_IT_Update, ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM2, ENABLE);
}

void GENERAL_TIM_Init(void)
{

    GENERAL_TIM2_NVIC_Config();
    GENERAL_TIM2_Mode_Config();

}


