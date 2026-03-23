#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void ADC1_Init(void);
uint16_t ADC1_GetValue(uint8_t ch);
uint16_t ADC1_GetAverage(uint8_t ch, uint8_t times);

#endif
