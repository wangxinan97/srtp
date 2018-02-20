#include "AD9910.h"
#include "usart.h"
#include "delay.h"
#include "STM32F103_LIB.h"
#include "led.h"
#include "lcd.h"
#include "sys.h"
#include "timer.h"

#define LED2ON 	  (GPIOB->BRR = GPIO_Pin_2)
#define LED2OFF 	  (GPIOB->BSRR = GPIO_Pin_2)

int main(void)
{
	//系统初始化
	delay_init(72);	//初始化延时函数
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//设置中断分组
	USARTx_Init(9600);
	STM32_ADC_Configuration();
  Init_ad9910();
	LED_Init();	//初始化LED接口
	initial_lcd();//液晶初始化
	LCD_Clear();
	//LCD_Show_CEStr(40,3,"初始化");
	//开定时中断
	//Timerx_Init(59999,35999);//0x0000~0xFFFF,2KHz的计数频率,计数到60000为30000ms,即每30s中断一次
  Timerx_Init(9999,35999);//0x0000~0xFFFF,2KHz的计数频率,计数到10000为5000ms,即每5s中断一次
	
	//待机，等待中断	
	while(1) 
	{
		//闪烁LED，提示系统正在运行
		LED2ON; 		//点亮LED灯
		delay_ms(500);	  // 延时	
    LED2OFF;				  // 熄灭LED灯
		delay_ms(500);	  // 延时	//输出高电平，熄灭灯
	}	
}
