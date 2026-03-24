#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "dht11.h"
#include "oled.h"

/*****************辰哥单片机设计******************
											STM32
 * 文件			:	DHT11温度湿度传感器实验                    
 * 版本			: V1.0
 * 日期			: 2024.8.4
 * MCU			:	STM32F103C8T6
 * 接口			:	见dht11.h文件		
 * IP账号		:	辰哥单片机设计（同BILIBILI|抖音|快手|小红书|CSDN|公众号|视频号等）
 * 作者			:	辰哥
 * 工作室		: 异方辰电子工作室
 * 讲解视频	:	https://www.bilibili.com/video/BV182421Z7by/?share_source=copy_web&vd_source=097fdeaf6b6ecfed8a9ff7119c32faf2
 * 官方网站	:	www.yfcdz.cn

**********************BEGIN***********************/		

u8 temp;
u8 humi;

int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	delay_init(72);
//	LED_Init();
//	LED_On();

	USART1_Config();//串口初始化
	OLED_Init();
	
	printf("Start \n");
	delay_ms(1000);
	while(DHT11_Init())
	{
		printf("DHT11 Error \r\n");
		delay_ms(1000);
	}
	//显示“温度：”
	OLED_ShowChinese(0,0,0,16,1); //温
	OLED_ShowChinese(16,0,1,16,1);//度
	OLED_ShowChar(72,0,':',16,1);
	OLED_ShowChar(72, 0, 'C',16,1);	
	//显示“湿度：”
	OLED_ShowChinese(0,16,2,16,1); //湿
	OLED_ShowChinese(16,16,1,16,1);//度
	OLED_ShowChar(72,16,':',16,1);
	OLED_ShowChar(72, 16, '%',16,1);	
	
  while (1)
  {
		DHT11_Read_Data(&temp,&humi);//
		printf("temp %d ,humi %d\r\n",temp,humi);
//		LED_Toggle();
		delay_ms(1000);
		//显示温度数据
		OLED_ShowNum(48,0,temp,2,16,1);
		//显示湿度数据
		OLED_ShowNum(48,16,humi,2,16,1);
  }
}



