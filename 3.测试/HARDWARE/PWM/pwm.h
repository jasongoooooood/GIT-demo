#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

// PWM通道定义 - 使用TIM2_CH2 (PA1)
#define PWM_TIM            TIM2
#define PWM_TIM_RCC        RCC_APB1Periph_TIM2
#define PWM_GPIO_RCC       RCC_APB2Periph_GPIOA
#define PWM_GPIO_PORT      GPIOA
#define PWM_GPIO_PIN       GPIO_Pin_1
#define PWM_CHANNEL        2          // TIM2的通道2

// PWM参数
#define PWM_FREQ           1000       // PWM频率 1kHz
#define PWM_PERIOD         999        // 自动重装载值 (频率 = 72MHz/(72*1000) = 1kHz)
#define PWM_PRESCALER      71         // 预分频值 (72-1)

void PWM_Init(void);
void PWM_SetDuty(uint8_t duty);      // duty: 0-100
void PWM_Start(void);
void PWM_Stop(void);
uint8_t PWM_GetDuty(void);            // 获取当前占空比

#endif