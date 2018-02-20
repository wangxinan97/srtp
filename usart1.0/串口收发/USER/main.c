#include "AD9910.h"
#include "usart.h"
#include "delay.h"
#include "STM32F103_LIB.h"
#include "led.h"
#include "config.h"

#define LEDON 	  (GPIOB->BRR = GPIO_Pin_2)
#define LEDOFF 	  (GPIOB->BSRR = GPIO_Pin_2)

#define SOFTWAREDEBUG 1

int main(void)
{
	
	//�������ݽ���
	extern uint16_t USART_RX_STA; //�����ⲿ����������״̬���	  
	extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //�����ⲿ���������ջ��壬���USART_REC_LEN���ֽ�
	uchar udata12[12] = {0}; //��Ž��յ������ݣ�0123��ʼƵ��+4567Ƶ���ƶ�+89ɨ�����
  
	//ɨƵ����
	uint32_t uminfre = 10000000; //��ʼƵ�ʣ�A01ģʽ10MHz-12MHz;S0ģʽ100MHz-120MHz�����뷶ΧΪ0~4294967295
	uint32_t ustepfre = 2000; //Ƶ�ʲ�����A01ģʽ1KHz;S0ģʽ10KHz�����뷶ΧΪ0~4294967295
	//uint16_t ucount = 1000; //ɨ����������뷶ΧΪ0~65535
	uint32_t umaxfre = 12000000; //��ֹƵ�ʣ�A01ģʽ10MHz-12MHz;S0ģʽ100MHz-120MHz�����뷶ΧΪ0~4294967295
	uint32_t ucurfre; //��ǰƵ�ʣ����뷶ΧΪ0~4294967295
  
	//ADת������
	extern __IO uint16_t ADCConvertedValue[2]; //�����ⲿ���������12λģ��ת�������
	vu16 uADCConvertedValueLocal2[2]; //��ֵ��/��λ���ADת��ֵ
	
	//��ʼ��
	delay_init(72);	//��ʼ����ʱ����
	USARTx_Init(9600);
	STM32_ADC_Configuration();
	Init_ad9910();
	LED_Init();	//��ʼ��LED�ӿ�
	
#ifdef SOFTWAREDEBUG
		return simulation();
#endif

	while(1) //�ȴ��ж�
	{
		if(USART_RX_STA & 0x8000) //���������
		{
			uint8_t i = 0;
			uint8_t ulength = 0; //���ݶγ���
			uint8_t j;
			
			while(USART_RX_BUF[i+2] != '#') 
			{
				udata12[i] = USART_RX_BUF[i+2]; //��ȡ����1���յ�������
				ulength++; //ͨ������1����
        i++;
			}
			if((USART_RX_BUF[0] == 0x10)&&(USART_RX_BUF[1] == 0x01)&&(ulength == 0x0C)) //֡ͷ��0x10�������֣�0x01�����ݳ��ȣ�12���ֽ�
      {
			  //��ȡɨ�����
			  uminfre = (udata12[0]<<24)|(udata12[1]<<16)|(udata12[2]<<8)|(udata12[3]); //PC�˸��ֽ��ȷ���
			  umaxfre = (udata12[4]<<24)|(udata12[5]<<16)|(udata12[6]<<8)|(udata12[7]);
				ustepfre = (udata12[8]<<24)|(udata12[9]<<16)|(udata12[10]<<8)|(udata12[11]);
			  ucurfre = uminfre;
			  //��ʼɨƵ
			  USART1_Printf("\r\nSweep begins.\r\n");
				//for(i=0;i<ucount;i++)
				while(ucurfre <= umaxfre)
			  {   
				  Freq_convert(ucurfre);
				  delay_ms(20); //�ȴ������ȶ�
				  //���ݲɼ��ͷ���
				  for(j=0;j<2;j++)
			    {
					  uADCConvertedValueLocal2[j] = ADCConvertedValue[j]; //��ȡ��ֵ��/��λ���ADת��ֵ
				  }
				  printf("%d,%d;%d\n",ucurfre,uADCConvertedValueLocal2[0],uADCConvertedValueLocal2[1]);
			    ucurfre += ustepfre;
		    }
			  //ɨƵ����
			  USART1_Printf("\r\nSweep is over.\r\n");
			  USART_RX_STA=0; //���¿�ʼ����
			}else 
			{
				USART_RX_STA=0; //���¿�ʼ����
      }
		}else
		{
			//��˸LED����ʾϵͳ��������
			LEDON; 		//����LED��
		  delay_ms(500);	  // ��ʱ	
    	LEDOFF;				  // Ϩ��LED��
		  delay_ms(500);	  // ��ʱ	//����ߵ�ƽ��Ϩ���
		}
	}	
}
