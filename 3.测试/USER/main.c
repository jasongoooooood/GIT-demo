#include "stm32f10x.h"
#include "oled.h"
#include "led.h"
#include "usart3.h"
#include "delay.h"
#include "key.h"
#include "pwm.h"
// #include "bluetooth.h"
#include "adc2.h"

#include "timer.h"
/*****************???????******************
											STM32
 * ??			:	ASRPRO??????                     
 * ??			: V1.0
 * ??			: 2025.2.8
 * MCU			:	STM32F103C8T6
 * ??			:	??usart3.h						
 * BILIBILI	:	???????
 * CSDN			:	???????
 * ??			:	?? 

**********************BEGIN***********************/


uint16_t pwm_val = 0;
uint16_t pwm_max = 500;
extern u8 Res;
extern uint16_t ADC2_Value;

extern volatile uint32_t sys_ms;



int main(void)
{
			// 1. 基础配置
    delay_init(72);
    KEY_Init();
    TIM2_PWM_Init(pwm_max, 0); 
    
    // 2. OLED初始化（先把屏点亮，不一定要在这里延时1秒）
    OLED_Init();
		ADC2_Init();// 初始化光敏 ADC
	

    OLED_Clear();
    
    // 3. 快速显示静态文字
    OLED_ShowChinese(0,0,0,16,1);
    OLED_ShowChinese(16,0,1,16,1);
    OLED_ShowChinese(32,0,2,16,1);
    OLED_ShowChinese(48,0,3,16,1);
    OLED_ShowChar(64,0,':',16,1);
		// 显示光照值（放在 OLED 第三行）
		OLED_ShowString(0, 32, "Light:", 16, 1);
    
    // 4. 最后开启串口，确保前面的耗时操作都做完了
    Res = 0; // 先清零一次
    USART3_Config(); 
		// USART2_Config();   // 初始化蓝牙串口

    
    u16 last_pwm = 999;

		TIM3_Init(9999, 71);   // 72MHz / (71+1) = 1MHz → 1MHz / 10000 = 100Hz = 10ms

//    delay_init(72);
//    USART3_Config();         // 语音模块
//    KEY_Init();              // PC8 PC9 按键
//		OLED_Init();
//		delay_ms(1000);
//	
//		OLED_Clear();
//		OLED_ShowChinese(0,0,0,16,1);
//		OLED_ShowChinese(16,0,1,16,1);
//		OLED_ShowChinese(32,0,2,16,1);
//		OLED_ShowChinese(48,0,3,16,1);
//		OLED_ShowChar(64,0,':',16,1);
//	
//    TIM2_PWM_Init(pwm_max, 0);   // PA0 PWM LED
//	
//		u16 last_pwm = 999; // 记录上一次的值，用于对比
		// --- 关键修改点 1: 将 last_pwm 设为一个不可能的值 ---
    // 这样开机后第一次进入 while(1) 就会强制刷新一次屏幕显示“灭”
    uint32_t light_refresh_count = 0;
		while(1)
		{
				// 1. 优先处理语音指令
				switch(Res)
				{
						case 1:     // 语音：开灯
								pwm_val = 500;
								Res = 0;
//								OLED_ShowChinese(80,0,4,16,1);
								break;

						case 2:     // 语音：关灯
								pwm_val = 0;
								Res = 0;
//								OLED_ShowChinese(80,0,5,16,1);
								break;

						default:
								break;
			}


		// 2. 处理按键 (PC8, PC9)
				uint8_t key = KEY_Scan();

				if(key == 1)
					pwm_val = (pwm_val + 50 > pwm_max) ? pwm_max : pwm_val + 50;

				if(key == 2)
					pwm_val = (pwm_val >= 50) ? pwm_val - 50 : 0;

				// 3. 更新 PWM
				TIM_SetCompare1(TIM2, pwm_val);


				// 4. 【核心优化】所有显示逻辑汇总到这里
				// 只有在亮度变化时，才“忍痛”花几十毫秒刷一次屏

				if(pwm_val != last_pwm)
				{
						OLED_ShowNum(64, 16, pwm_val, 3, 16, 1);

						if(pwm_val == 0) OLED_ShowChinese(80, 0, 5, 16, 1); // 灭
						else             OLED_ShowChinese(80, 0, 4, 16, 1); // 亮
						
						// 注意：如果你用的是带显存的库，这里可能需要手动调一次 OLED_Refresh();
						// 如果 ShowChinese 内部自带了 Refresh，就不用再写了
						last_pwm = pwm_val; 
				}
				
				static uint16_t last_display_adc = 0; // 记录上一次显示在屏幕上的值
				static uint32_t last_update = 0;

				// 1. 设置一个较长的检测间隔，比如 200ms
				if(sys_ms - last_update >= 200)   
				{
					last_update = sys_ms;
					
					// 2. 【核心优化】只有当当前的 ADC2_Value 和上一次显示的值差值超过 30 时才刷屏
					// 这样可以过滤掉 ADC 的微小抖动，极大减少刷屏次数
					if(abs(ADC2_Value - last_display_adc) > 30) 
					{
						OLED_ShowNum(48, 32, ADC2_Value, 4, 16, 1);
						last_display_adc = ADC2_Value; // 更新记录
					}

				

				}
		}
}