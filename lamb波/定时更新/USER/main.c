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
	//ϵͳ��ʼ��
	delay_init(72);	//��ʼ����ʱ����
	MY_NVIC_PriorityGroup_Config(NVIC_PriorityGroup_2);	//�����жϷ���
	USARTx_Init(9600);
	STM32_ADC_Configuration();
  Init_ad9910();
	LED_Init();	//��ʼ��LED�ӿ�
	initial_lcd();//Һ����ʼ��
	LCD_Clear();
	//LCD_Show_CEStr(40,3,"��ʼ��");
	//����ʱ�ж�
	//Timerx_Init(59999,35999);//0x0000~0xFFFF,2KHz�ļ���Ƶ��,������60000Ϊ30000ms,��ÿ30s�ж�һ��
  Timerx_Init(9999,35999);//0x0000~0xFFFF,2KHz�ļ���Ƶ��,������10000Ϊ5000ms,��ÿ5s�ж�һ��
	
	//�������ȴ��ж�	
	while(1) 
	{
		//��˸LED����ʾϵͳ��������
		LED2ON; 		//����LED��
		delay_ms(500);	  // ��ʱ	
    LED2OFF;				  // Ϩ��LED��
		delay_ms(500);	  // ��ʱ	//����ߵ�ƽ��Ϩ���
	}	
}
