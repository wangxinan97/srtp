#include "config.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "stdio.h"

int32_t da[3][10003]={0};

//#define SOFTWAREDEBUG
int simulation(){
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
      LEDON;    //����LED��
      delay_ms(500);    // ��ʱ 
      LEDOFF;         // Ϩ��LED��
      delay_ms(500);    // ��ʱ //����ߵ�ƽ��Ϩ���
    }
  } 
	return 1;
}

void readExcel(){
	 int i,j;
   FILE *fp = NULL ;
   fp = fopen("G:\\Desktop\\usart1.0\\config\\test.xls","r") ;
	 fseek(fp, 10L, SEEK_SET);  /*�ӵڶ��п�ʼ��ȡ*/
   for(i = 0 ;i < 3 ; i++)
        for(j = 0 ;j < 10003 ; j++)
           fscanf(fp,"%d",&da[i][j]);
	 
}
