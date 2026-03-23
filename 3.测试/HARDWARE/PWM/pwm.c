#include "pwm.h"
#include "stm32f10x.h"

// TIM2 PWM输出初始化 (PA0)
// arr：自动重装值
// psc：时钟预分频数
void TIM2_PWM_Init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 1. 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // TIM2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA 时钟

    // 2. 配置 PA0 为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        // 复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置定时器基本参数
    TIM_TimeBaseStructure.TIM_Period = arr;                // 自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;             // 预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. 配置 PWM 模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;      // PWM1 模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;                     // 初始占空比
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);      // CCR1 预装载
    TIM_ARRPreloadConfig(TIM2, ENABLE);                    // ARR 预装载

    // 5. 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}
