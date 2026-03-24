#include "dht11.h"
#include "delay.h"
      
/*****************³½¸çµ¥Æ¬»úÉè¼Æ******************
											STM32
 * ÎÄ¼þ			:	DHT11ÎÂÊª¶È´«¸ÐÆ÷cÎÄ¼þ                   
 * °æ±¾			: V1.0
 * ÈÕÆÚ			: 2024.8.4
 * MCU			:	STM32F103C8T6
 * ½Ó¿Ú			:	¼ûdht11.hÎÄ¼þ		
 * IPÕËºÅ		:	³½¸çµ¥Æ¬»úÉè¼Æ£¨Í¬BILIBILI|¶¶Òô|¿ìÊÖ|Ð¡ºìÊé|CSDN|¹«ÖÚºÅ|ÊÓÆµºÅµÈ£©
 * ×÷Õß			:	³½¸ç
 * ¹¤×÷ÊÒ		: Òì·½³½µç×Ó¹¤×÷ÊÒ
 * ½²½âÊÓÆµ	:	https://www.bilibili.com/video/BV182421Z7by/?share_source=copy_web&vd_source=097fdeaf6b6ecfed8a9ff7119c32faf2
 * ¹Ù·½ÍøÕ¾	:	www.yfcdz.cn

**********************BEGIN***********************/		
			
//¸´Î»DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_Mode(OUT); 	//SET OUTPUT
	DHT11_Low; 	      //À­µÍDQ
	delay_ms(20);    	//Ö÷»úÀ­µÍ18~30ms
	DHT11_High; 			//DQ=1 
	delay_us(13);     	//Ö÷»úÀ­¸ß10~35us
}

//µÈ´ýDHT11µÄ»ØÓ¦
//·µ»Ø1:Î´¼ì²âµ½DHT11µÄ´æÔÚ
//·µ»Ø0:´æÔÚ
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_Mode(IN);//SET INPUT	 
    while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//DHT11»áÀ­µÍ40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//DHT11À­µÍºó»áÔÙ´ÎÀ­¸ß40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//´ÓDHT11¶ÁÈ¡Ò»¸öÎ»
//·µ»ØÖµ£º1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//µÈ´ý±äÎªµÍµçÆ½
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//µÈ´ý±ä¸ßµçÆ½
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//µÈ´ý40us
	if(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN))return 1;
	else return 0;		   
}

//´ÓDHT11¶ÁÈ¡Ò»¸ö×Ö½Ú
//·µ»ØÖµ£º¶Áµ½µÄÊý¾Ý
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}

//´ÓDHT11¶ÁÈ¡Ò»´ÎÊý¾Ý
//temp:ÎÂ¶ÈÖµ(·¶Î§:0~50¡ã)
//humi:Êª¶ÈÖµ(·¶Î§:20%~90%)
//·µ»ØÖµ£º0,Õý³£;1,¶ÁÈ¡Ê§°Ü
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//¶ÁÈ¡40Î»Êý¾Ý
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else return 1;
	return 0;	    
}

//³õÊ¼»¯DHT11µÄIO¿Ú DQ Í¬Ê±¼ì²âDHT11µÄ´æÔÚ
//·µ»Ø1:²»´æÔÚ
//·µ»Ø0:´æÔÚ    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;	
 	RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);	 //Ê¹ÄÜPA¶Ë¿ÚÊ±ÖÓ
 	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;				 //PG11¶Ë¿ÚÅäÖÃ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);				 //³õÊ¼»¯IO¿Ú
 	GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN);						 //PG11 Êä³ö¸ß
			    
	DHT11_Rst();  //¸´Î»DHT11
	return DHT11_Check();//µÈ´ýDHT11µÄ»ØÓ¦
} 

void DHT11_Mode(u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(mode)
	{
		GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin =  DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

