#include "bluetooth.h"
#include "stm32f10x.h"

extern u8 Res;   // 与语音模块共用控制变量
extern uint16_t pwm_val;
extern uint16_t pwm_max;

void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // PA2 → USART2_TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA3 → USART2_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2 参数
    USART_InitStructure.USART_BaudRate = 9600;  // HC-05 默认 9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);

    // 中断配置
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 蓝牙接收中断
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        u8 ch = USART_ReceiveData(USART2);

        // 蓝牙控制协议（你可以扩展）
        if(ch == '1') Res = 1;   // 蓝牙发送 '1' → 开灯
        if(ch == '2') Res = 2;   // 蓝牙发送 '0' → 关灯
		else if(ch == 'H') pwm_val = (pwm_val + 50 > pwm_max) ? pwm_max : pwm_val + 50;
        else if(ch == 'L') pwm_val = (pwm_val >= 50) ? pwm_val - 50 : 0;
    }
}
