#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void ADC1_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 2. ADC 时钟 = PCLK2 / 6 = 12MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    // 3. PA6 → ADC1_IN6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 4. ADC 配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    // 5. 校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

// 单次采样
uint16_t ADC1_GetValue(uint8_t ch)
{
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_55Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}

// 多次平均
uint16_t ADC1_GetAverage(uint8_t ch, uint8_t times)
{
    uint32_t sum = 0;
    for(uint8_t i = 0; i < times; i++)
    {
        sum += ADC1_GetValue(ch);
    }
    return sum / times;
}
