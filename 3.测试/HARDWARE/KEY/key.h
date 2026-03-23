#ifndef __KEY_H
#define	__KEY_H
#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

// /***************�����Լ��������****************/

// #define		KEY_GPIO_CLK								RCC_APB2Periph_GPIOA
// #define 	KEY_GPIO_PORT								GPIOA
// #define 	KEY_GPIO_PIN								GPIO_Pin_0	

// /*********************END**********************/


/***************�����Լ��������****************/

// KEY0 - PC8
#define		KEY0_GPIO_CLK							RCC_APB2Periph_GPIOC
#define 	KEY0_GPIO_PORT							GPIOC
#define 	KEY0_GPIO_PIN							GPIO_Pin_8	

// KEY1 - PC9
#define		KEY1_GPIO_CLK							RCC_APB2Periph_GPIOC
#define 	KEY1_GPIO_PORT							GPIOC
#define 	KEY1_GPIO_PIN							GPIO_Pin_9	

/*********************END**********************/

#define KEY0  GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN)	//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN)	//��ȡ����1

#define KEY0_PRES 1	//KEY0����
#define KEY1_PRES 2	//KEY1����


// #define KEY  GPIO_ReadInputDataBit(KEY_GPIO_PORT,KEY_GPIO_PIN)		//��ȡ����1

// #define KEY_PRES 1	//KEY����


void Key_Init(void);
uint16_t Key_GetData(void);
uint8_t KEY_Scan(void);
// u8 KEY_Scan(u8 mode);
#endif /* __ADC_H */

