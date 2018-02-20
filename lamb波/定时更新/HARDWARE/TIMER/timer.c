/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6����
			��ʾ��12864
�ӿڣ������ӿ������key.h
ʱ�䣺2015/11/3
�汾��1.0
���ߣ���������
����������������ԭ�ӣ�����Լ���������δ��������ɣ��������������κ���;

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/

#include "timer.h"
#include "led.h"
#include "lcd.h"
#include "AD9910.h"
#include "usart.h"
#include "delay.h"
#include <stdio.h>
#include <ctype.h>
#include <cstring>

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void Timerx_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	//��ʱ����ʼ��					   //arr,psc
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim ��Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�	 ��������ж�
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM ����(���)�ж�                  
		TIM_IT_Trigger,   //TIM �����ж�   �������жϵȣ�					��ֵַ	 ((uint16_t)0x0040)         
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����ʱ�������� ������ʱ��
							 
}

//ɨƵ����
uint32_t uminfre = 10560000; //��ʼƵ�ʣ�A01ģʽ10MHz-12MHz;S0ģʽ100MHz-120MHz�����뷶ΧΪ0~4294967295
uint32_t ustepfre = 500; //Ƶ�ʲ�����A01ģʽ1KHz;S0ģʽ10KHz�����뷶ΧΪ0~4294967295
uint32_t umaxfre = 10660000; //��ֹƵ�ʣ�A01ģʽ10MHz-12MHz;S0ģʽ100MHz-120MHz�����뷶ΧΪ0~4294967295
uint32_t ucurfre; //��ǰƵ�ʣ����뷶ΧΪ0~4294967295
//ADת������
extern __IO uint16_t ADCConvertedValue[2]; //�����ⲿ���������12λģ��ת�������
vu16 uADCConvertedValueLocal2[2]; //��ֵ��/��λ���ADת��ֵ

uint32_t ufre_buf[1000];	//���Ƶ����Ϣ
uint32_t uamp_buf[1000];	//��ŷ�ֵ����Ϣ
uint32_t uphs_buf[1000];	//�����λ����Ϣ

void TIM4_IRQHandler(void)   //TIM3�ж�
{
	uint8_t j;
  uint8_t showstr[10]={0};
	uint16_t ucount=0;	//��Ÿ���
	uint8_t uepsilon=2;	//��������
  uint16_t best; //���ŵ�λ��
	uint32_t difference;

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		LCD_Refresh_Gram();//������ʾ
		//Ƶ��ɨ��
		ucurfre = uminfre;
		//USART1_Printf("\r\nSweep begins.\r\n");//��ʼɨƵ
		while(ucurfre <= umaxfre)
		{   
		  Freq_convert(ucurfre);
			delay_ms(20); //�ȴ������ȶ�
			//���ݲɼ��ͷ���
			for(j=0;j<2;j++)
			{
			  uADCConvertedValueLocal2[j] = ADCConvertedValue[j]; //��ȡ��ֵ��/��λ���ADת��ֵ
			}
			//printf("%d,%d;%d\n",ucurfre,uADCConvertedValueLocal2[0],uADCConvertedValueLocal2[1]);
			ufre_buf[ucount] = ucurfre;	
      uamp_buf[ucount] = uADCConvertedValueLocal2[0];	
      uphs_buf[ucount] = uADCConvertedValueLocal2[1];	
      ucount++;	
			ucurfre += ustepfre;
		}
  	//USART1_Printf("\r\nSweep is over.\r\n");//ɨƵ����
    //��ֵ����
		best = Golddiv(uamp_buf,ucount,uepsilon);
		//USART1_Printf("\r\nThe results of peak search:\r\n");//��ֵ�������
		//printf("%d,%d;%d\n",ufre_buf[best],uamp_buf[best],uphs_buf[best]);
		difference = 10671000-ufre_buf[best];//21���Ӧ10650000
		
		//Һ����ʾ
		//LCD_Show_CEStr(2*8,3,"T:");
		sprintf((char *)&showstr, "%3d.%1d",difference/1000,(difference%1000)/100);//�ַ�ת��
		OLED_ShowString(4*8, 3, showstr);
		LCD_Show_CEStr(10*8,3,"C");
	}
}

//�ƽ�ָ
uint16_t Golddiv(u32 uamp_buf[],u16 ucount,u8 uepsilon)
{
	uint8_t i;
	uint16_t x1,x2,a,b,best;
	uint32_t f1,f2;
	
	a=0;
	b=ucount-1;
	x1 = a+0.382*(b-a);
  f1 = uamp_buf[x1]; 
  x2 = a+0.618*(b-a);  
  f2 = uamp_buf[x2];
  
  while((b-a)>uepsilon)
  {
		if (f1 > f2)  
    {
			b = x2;    
      x2 = x1;    
      f2 = f1;
      x1 = a+0.382*(b-a);  
      f1 = uamp_buf[x1];
		}else
    {
			a = x1;
      x1 = x2;
      f1 = f2;
      x2 = a+0.618*(b-a);
      f2 = uamp_buf[x2];
    }
	}
	best = a;
	for(i=a;i<b;i++)
	{
		if(uamp_buf[i+1]>uamp_buf[a])
		{
			best = i+1;
		}
  }		
  return best;
}





