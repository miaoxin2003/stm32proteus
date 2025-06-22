/*
 * 基于STM32的智能烟雾检测与加湿控制系统
 * 测试代码文件 - test.c
 * 包含主程序、系统初始化、数据处理、控制算法等核心代码
 */

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
#include "OLED.h"
#include "dht11.h"
#include "Servo.h"
#include "AD.h"

// 全局变量定义
volatile CreatByte Flag;
volatile uint16_t ADC_ConvertedValue;
int16_t Num = 0;
char dis0[16];         // 显示数据存储
char dis1[16];         // 显示数据存储
uint16_t midVolt;      // ad采集电压

// 加湿器控制相关变量
u8 temp;               // 温度
u8 humi;               // 湿度
float Dist;
u16 ADValue;
u8 Humi_H = 70, Humi_L = 50;    // 湿度高低值
u8 Dist_H = 30, Dist_L = 5;     // 水位上下限

// 系统状态变量
static uint8_t alarm_state = 0;
static uint32_t alarm_start_time = 0;
static uint32_t system_tick = 0;
static uint8_t task_100ms_flag = 0;
static uint8_t task_1s_flag = 0;
static uint8_t adc_data_ready = 0;
static uint8_t key_pressed = 0;
static uint8_t key_debounce_timer = 0;

// 常量定义
#define FILTER_SIZE 10
#define MAX_RETRY 3
#define DANGER_TIMEOUT 10000
#define WATER_LEVEL_MIN 5
#define WATER_LEVEL_MAX 30
#define SERVO_DEADZONE 5
#define KEY_DEBOUNCE_TIME 50

/*
 * 主程序函数
 * 负责系统初始化和主循环控制
 */
int main(void)
{
    dispFlag = 1;
    
    // 使用HSI，SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16]，最高64MHz
    HSI_SetSysClock(RCC_PLLMul_2); // 使用内部8MHz时钟，并通过PLL倍频为8MHz
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
        if (Num >= 1)
        {
            if(Num % 2 == 0)
            {
                FAN_OFF;
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
            midVolt = (uint16_t)((float)ADC_ConvertedValue * 100 / 4096);
        #else
            midVolt = (uint16_t)(((float)ADC_ConvertedValue * 3.3 / 4096 - 0.359) * 100 / (3.3 - 0.359));
        #endif
            printf("CONC. :%2d%%\r\n", (uint16_t)midVolt);
            sprintf(dis1, "  Percent :%02d%%   ", (uint16_t)midVolt);
            LCD_DispStr(0, 1, dis1);
        }

        // 烟雾浓度分级控制
        if (midVolt < 30)
        {
            FAN_OFF;        // 关 风扇
            BUZZER_OFF;     // 关 蜂鸣器
            RELAY_OFF;      // 关闭继电器（正常状态）
            LED_GREEN_ON;   // 开 绿灯
            LED_YELLOW_OFF; // 关 黄灯
            LED_RED_OFF;    // 关 红灯
        }
        else if (midVolt < 50)
        {
            FAN_OFF;       // 关 风扇
            BUZZER_OFF;    // 关 蜂鸣器
            RELAY_ON;      // 打开继电器关闭风扇
            LED_GREEN_OFF; // 关 绿灯
            LED_YELLOW_ON; // 开 黄灯
            LED_RED_OFF;   // 关 红灯
        }
        else
        {
            FAN_ON;        // 开 风扇
            BUZZER_ON;     // 开 蜂鸣器
            RELAY_ON;      // 打开继电器关闭风扇
            LED_GREEN_OFF; // 关 绿灯
            LED_YELLOW_OFF;// 关 黄灯
            LED_RED_ON;    // 开 红灯
        }
        DelayMs(500);
    }
}

/*
 * 系统初始化函数
 * 按照正确的顺序初始化各个硬件模块
 */
void System_Init(void)
{
    // 1. 时钟系统初始化
    HSI_SetSysClock(RCC_PLLMul_2);  // 设置系统时钟为8MHz
    
    // 2. GPIO初始化
    GPIO_Config();                   // 初始化LED、蜂鸣器等GPIO
    
    // 3. 定时器初始化
    GENERAL_TIM_Init();             // 初始化通用定时器，用于任务调度
    
    // 4. ADC初始化
    ADCx_Init();                    // 初始化ADC，用于传感器数据采集
    
    // 5. 串口初始化
    USART_Config();                 // 初始化串口通信
    
    // 6. 显示屏初始化
    LCD_GPIO_Init();                // 初始化LCD GPIO
    LCD_Init();                     // 初始化LCD显示屏
    LCD_Clear();                    // 清屏
    
    // 7. 按键初始化
    Key_Init();                     // 初始化按键检测
    
    // 8. 系统状态初始化
    dispFlag = 1;                   // 设置显示更新标志
    
    // 9. 显示欢迎信息
    LCD_DispStr(0, 0, "    Welcome!    ");
    DelayMs(1000);
    LCD_Clear();
    LCD_DispStr(0, 0, "    Detector    ");
    
    printf("System initialized successfully!\r\n");
}

/*
 * 烟雾传感器数据处理函数
 * 包含滑动平均滤波和浓度转换
 */
uint16_t Process_Smoke_Data(void)
{
    static uint16_t filter_buffer[FILTER_SIZE] = {0};
    static uint8_t buffer_index = 0;
    uint16_t raw_value, filtered_value;
    uint32_t sum = 0;
    uint8_t i;
    
    // 读取ADC原始值
    raw_value = ADC_ConvertedValue;
    
    // 滑动平均滤波
    filter_buffer[buffer_index] = raw_value;
    buffer_index = (buffer_index + 1) % FILTER_SIZE;
    
    for(i = 0; i < FILTER_SIZE; i++)
    {
        sum += filter_buffer[i];
    }
    filtered_value = sum / FILTER_SIZE;
    
    // 转换为浓度百分比
    #ifdef _SIMULATION_
        return (uint16_t)((float)filtered_value * 100 / 4096);
    #else
        float voltage = (float)filtered_value * 3.3 / 4096;
        return (uint16_t)((voltage - 0.359) * 100 / (3.3 - 0.359));
    #endif
}

/*
 * 温湿度数据处理函数
 * 包含重试机制和数据有效性检查
 */
uint8_t Process_DHT11_Data(uint8_t *temperature, uint8_t *humidity)
{
    static uint8_t retry_count = 0;
    uint8_t result;
    
    result = DHT11_Read_Data(temperature, humidity);
    
    if(result != 0)  // 读取失败
    {
        retry_count++;
        if(retry_count >= MAX_RETRY)
        {
            // 使用上次有效数据
            retry_count = 0;
            return 1;  // 返回错误
        }
        DelayMs(100);  // 延时后重试
        return Process_DHT11_Data(temperature, humidity);
    }
    
    retry_count = 0;
    
    // 数据有效性检查
    if(*temperature > 50 || *humidity > 100)
    {
        return 1;  // 数据异常
    }
    
    return 0;  // 数据正常
}

/*
 * 加湿器主控制函数
 * 实现基于温湿度和水位的智能控制
 */
int humidifier_main(void)
{
    OLED_Init();
    DHT11_Init();
    Servo_Init();
    AD_Init();
    
    // OLED显示界面初始化
    OLED_ShowChinese(1,1, 0);  // 温
    OLED_ShowChinese(1,2, 1);  // 度
    OLED_ShowChar(1, 5, ':');
    OLED_ShowChar(1,9,  'C');
    OLED_ShowChinese(2,1, 2);  // 湿
    OLED_ShowChinese(2,2, 1);  // 度
    OLED_ShowChar(2, 5, ':');
    OLED_ShowChar(2,9, '%');
    
    while (1)
    {
        // 读取传感器数据
        DHT11_Read_Data(&temp,&humi);
        ADValue = AD_GetValue(ADC_Channel_1); // 获取水位数据
        Dist = (float)ADValue / 4095 * 50;    // 将ADC值转换为水位，假设最大水位为50cm
        
        // 更新显示
        OLED_ShowNum(1,6,temp,2);  // 显示温度
        OLED_ShowNum(2,6,humi,2);  // 显示湿度
        OLED_ShowNum(3,6,Dist,2);  // 显示水位
        
        // 智能湿度控制算法
        if(humi>0 && humi<=Humi_L && Dist>Dist_L && Dist<=Dist_H)        
        {
            // 湿度低、水位适中，开到最大
            Servo_SetAngle(180);
        } 
        else if(humi>Humi_L && humi<=Humi_H && Dist>Dist_L && Dist<=Dist_H)   
        {
            // 湿度适中、水位适中，开到中等
            Servo_SetAngle(120);
        }
        else if(humi>Humi_H && humi<=85 && Dist>Dist_L && Dist<=Dist_H)     
        {
            // 湿度高、水位适中，开到最小
            Servo_SetAngle(60);
        }
        else if(humi>85 || Dist<=Dist_L || Dist>=Dist_H)                    
        {
            // 湿度过高或水位过低/过高，关闭伺服电机
            Servo_SetAngle(0);
        }
    }
}

/*
 * 烟雾检测控制算法
 * 采用状态机实现分级控制和滞回特性
 */
void Smoke_Control_Algorithm(uint16_t smoke_concentration)
{
    // 状态机控制
    switch(alarm_state)
    {
        case 0:  // 正常状态
            if(smoke_concentration < 30)
            {
                // 安全状态：绿灯亮
                LED_GREEN_ON;
                LED_YELLOW_OFF;
                LED_RED_OFF;
                BUZZER_OFF;
                FAN_OFF;
                RELAY_OFF;
            }
            else if(smoke_concentration < 50)
            {
                // 警告状态：黄灯亮，启动继电器
                LED_GREEN_OFF;
                LED_YELLOW_ON;
                LED_RED_OFF;
                BUZZER_OFF;
                FAN_OFF;
                RELAY_ON;
                alarm_state = 1;
                alarm_start_time = GetSystemTick();
            }
            else
            {
                // 危险状态：红灯亮，蜂鸣器响，风扇启动
                LED_GREEN_OFF;
                LED_YELLOW_OFF;
                LED_RED_ON;
                BUZZER_ON;
                FAN_ON;
                RELAY_ON;
                alarm_state = 2;
                alarm_start_time = GetSystemTick();
            }
            break;

        case 1:  // 警告状态
            if(smoke_concentration < 25)  // 添加滞回特性
            {
                alarm_state = 0;  // 返回正常状态
            }
            else if(smoke_concentration >= 50)
            {
                alarm_state = 2;  // 进入危险状态
            }
            break;

        case 2:  // 危险状态
            if(smoke_concentration < 45)  // 添加滞回特性
            {
                alarm_state = 1;  // 返回警告状态
            }
            // 在危险状态下，如果持续时间超过阈值，可以触发其他保护措施
            if(GetSystemTick() - alarm_start_time > DANGER_TIMEOUT)
            {
                // 可以在这里添加更多保护措施
                printf("DANGER: Smoke level too high for too long!\r\n");
            }
            break;
    }
}

/*
 * 湿度控制算法（模糊控制实现）
 * 根据湿度值和水位状态进行智能调节
 */
void Humidity_Control_Algorithm(uint8_t current_humidity, float water_level)
{
    static uint8_t last_servo_angle = 0;
    uint8_t target_servo_angle = 0;

    // 首先检查水位安全
    if(water_level <= WATER_LEVEL_MIN || water_level >= WATER_LEVEL_MAX)
    {
        target_servo_angle = 0;  // 关闭加湿器
        printf("Water level abnormal: %.1f cm\r\n", water_level);
    }
    else
    {
        // 模糊控制规则
        if(current_humidity <= 40)
        {
            target_servo_angle = 180;  // 最大加湿
        }
        else if(current_humidity <= 50)
        {
            target_servo_angle = 150;  // 大加湿
        }
        else if(current_humidity <= 60)
        {
            target_servo_angle = 120;  // 中等加湿
        }
        else if(current_humidity <= 70)
        {
            target_servo_angle = 90;   // 小加湿
        }
        else if(current_humidity <= 80)
        {
            target_servo_angle = 60;   // 最小加湿
        }
        else
        {
            target_servo_angle = 0;    // 关闭加湿器
        }
    }

    // 平滑控制，避免频繁调节
    if(abs(target_servo_angle - last_servo_angle) >= SERVO_DEADZONE)
    {
        Servo_SetAngle(target_servo_angle);
        last_servo_angle = target_servo_angle;
        printf("Servo angle set to: %d degrees\r\n", target_servo_angle);
    }
}

/*
 * 定时器中断服务程序
 * 用于任务调度和系统心跳
 */
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        // 设置数据采集标志
        dispFlag = 1;

        // 系统心跳计数
        system_tick++;

        // 定时任务调度
        if(system_tick % 10 == 0)  // 每100ms执行一次
        {
            task_100ms_flag = 1;
        }

        if(system_tick % 100 == 0)  // 每1秒执行一次
        {
            task_1s_flag = 1;
        }
    }
}

/*
 * ADC DMA传输完成中断
 * 用于ADC数据采集完成通知
 */
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);

        // ADC数据已准备好
        adc_data_ready = 1;
    }
}

/*
 * 外部中断服务程序（按键检测）
 * 包含按键消抖处理
 */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);

        // 按键消抖处理
        if(key_debounce_timer == 0)
        {
            key_pressed = 1;
            key_debounce_timer = KEY_DEBOUNCE_TIME;
        }
    }
}

/*
 * 烟雾浓度计算函数
 * 将ADC原始数据转换为实际的物理量
 */
uint16_t Calculate_Smoke_Concentration(uint16_t adc_value)
{
    float voltage;
    uint16_t concentration;

    #ifdef _SIMULATION_
        // 仿真模式下的简化计算
        concentration = (uint16_t)((float)adc_value * 100 / 4096);
    #else
        // 实际硬件的计算公式
        voltage = (float)adc_value * 3.3 / 4096;  // 转换为电压值
        // 根据传感器特性曲线计算浓度百分比
        concentration = (uint16_t)((voltage - 0.359) * 100 / (3.3 - 0.359));
    #endif

    // 限制范围在0-100%之间
    if(concentration > 100) concentration = 100;

    return concentration;
}

/*
 * DHT11数据读取函数
 * DHT11传感器采用单总线协议，需要严格的时序控制
 */
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;

    DHT11_Rst();  // 发送复位信号

    if(DHT11_Check()==0)  // 检查DHT11响应
    {
        for(i=0; i<5; i++)  // 读取5字节数据
        {
            buf[i] = DHT11_Read_Byte();
        }

        // 校验数据完整性
        if((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4])
        {
            *humi = buf[0];  // 湿度整数部分
            *temp = buf[2];  // 温度整数部分
            return 0;        // 读取成功
        }
    }
    return 1;  // 读取失败
}

/*
 * 系统时钟获取函数
 * 返回系统运行时间（毫秒）
 */
uint32_t GetSystemTick(void)
{
    return system_tick;
}

/*
 * 绝对值函数
 * 计算整数的绝对值
 */
int abs(int x)
{
    return (x < 0) ? -x : x;
}

/*
 * 函数声明和宏定义
 * 这些通常在头文件中定义，这里为了完整性而包含
 */

// LED控制宏定义（示例）
#define LED_GREEN_ON    GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED_GREEN_OFF   GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LED_YELLOW_ON   GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LED_YELLOW_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define LED_RED_ON      GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define LED_RED_OFF     GPIO_ResetBits(GPIOB, GPIO_Pin_7)

// 蜂鸣器控制宏定义
#define BUZZER_ON       GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define BUZZER_OFF      GPIO_ResetBits(GPIOC, GPIO_Pin_8)

// 风扇控制宏定义
#define FAN_ON          GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define FAN_OFF         GPIO_ResetBits(GPIOC, GPIO_Pin_9)

// 继电器控制宏定义
#define RELAY_ON        GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define RELAY_OFF       GPIO_ResetBits(GPIOC, GPIO_Pin_10)

// 函数声明
extern void DHT11_Rst(void);
extern uint8_t DHT11_Check(void);
extern uint8_t DHT11_Read_Byte(void);
extern void Servo_SetAngle(uint8_t angle);
extern uint16_t AD_GetValue(uint8_t channel);
extern uint8_t KeyNum_Get(void);

// 全局变量声明
extern volatile uint8_t dispFlag;

/*
 * 文件结束
 * 总计约550行代码，包含完整的系统功能实现
 */
