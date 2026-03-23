#include "key.h"


/* void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd (KEY_GPIO_CLK, ENABLE );				// ������ ��Ƭ�����Ŷ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN;				// �������� ��Ƭ������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					// ����Ϊ�������
	
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);					// ��ʼ�� 
	
}

uint16_t Key_GetData()
{
	static u8 key_up=1;//�������ɿ���־
	if(key_up&&(KEY==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY==0)return KEY_PRES;
	}else if(KEY==1)key_up=1; 	    
 	return 0;// �ް�������
}


 */

 
/* void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ����GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// ��ʼ��KEY0 (PC8)
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// ��������
	GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure);
	
	// ��ʼ��KEY1 (PC9)
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// ��������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
}
 */





uint8_t KEY_Scan(void)
{
    static uint8_t key_up = 1;

    if(key_up && (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0))
    {
        delay_ms(10);
        key_up = 0;
        return 1;   // KEY0
    }

    if(key_up && (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0))
    {
        delay_ms(10);
        key_up = 0;
        return 2;   // KEY1
    }

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 1 &&
       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 1)
    {
        key_up = 1; // 按键全部松开
    }

    return 0;
}


void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. ʹ�� GPIOC ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 2. ���� PC8��PC9 Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      // ��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}



// ��ȡ����ֵ������0-�ް�����1-KEY0���£�2-KEY1����
uint16_t Key_GetData(void)
{
	static u8 key0_up = 1;  // KEY0�ɿ���־
	static u8 key1_up = 1;  // KEY1�ɿ���־
	
	// KEY0��� (PC8, �͵�ƽ����)
	if(key0_up && (KEY0 == 0))
	{
		delay_ms(10);		// ȥ����
		if(KEY0 == 0)
		{
			key0_up = 0;
			return KEY0_PRES;	// ����1
		}
	}
	else if(KEY0 == 1)		// KEY0�ɿ�
	{
		key0_up = 1;
	}
	
	// KEY1��� (PC9, �͵�ƽ����)
	if(key1_up && (KEY1 == 0))
	{
		delay_ms(10);		// ȥ����
		if(KEY1 == 0)
		{
			key1_up = 0;
			return KEY1_PRES;	// ����2
		}
	}
	else if(KEY1 == 1)		// KEY1�ɿ�
	{
		key1_up = 1;
	}
	
	return 0;  // �ް�������
}