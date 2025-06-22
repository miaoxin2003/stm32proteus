#include "clkconfig.h"
#include "stm32f10x_rcc.h"


void HSE_SetSysClock(uint32_t pllmul)
{
	__IO uint32_t StartUpCounter = 0, HSEStartUpStatus = 0;

  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);

  // 等待 HSE 启动稳定
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	// 只有 HSE 稳定之后则继续往下执行
  if (HSEStartUpStatus == SUCCESS)
  {
//----------------------------------------------------------------------//
    // 使能FLASH 预存取缓冲区
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);

    // AHB预分频因子设置为1分频，HCLK = SYSCLK 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    // APB2预分频因子设置为1分频，PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    // APB1预分频因子设置为1分频，PCLK1 = HCLK/2 
    RCC_PCLK1Config(RCC_HCLK_Div2);
		

    // 设置PLL时钟来源为HSE，设置PLL倍频因子
		// PLLCLK = 8MHz * pllmul
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul);
//------------------------------------------------------------------//

    // 开启PLL 
    RCC_PLLCmd(ENABLE);

    // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    // 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    // 读取时钟切换状态位，确保PLLCLK被选为系统时钟
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { 
    while (1)
    {
    }
  }
}

void HSI_SetSysClock(uint32_t pllmul)
{
	__IO uint32_t HSIStartUpStatus = 0;
  RCC_DeInit(); 
	RCC_HSICmd(ENABLE);
	HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;
  if (HSIStartUpStatus == RCC_CR_HSIRDY)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, pllmul);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { 
    while (1)
    {
    }
  }
}
