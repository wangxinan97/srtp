#include "config.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "stdio.h"

int32_t da[3][10003]={0};

//#define SOFTWAREDEBUG
int simulation(){
  while(1) //等待中断
  {
    if(USART_RX_STA & 0x8000) //接收完成了
    {
      uint8_t i = 0;
      uint8_t ulength = 0; //数据段长度
      uint8_t j;
      
      while(USART_RX_BUF[i+2] != '#') 
      {
        udata12[i] = USART_RX_BUF[i+2]; //读取串口1接收到的数据
        ulength++; //通过串口1发送
        i++;
      }
      if((USART_RX_BUF[0] == 0x10)&&(USART_RX_BUF[1] == 0x01)&&(ulength == 0x0C)) //帧头：0x10，命令字：0x01，数据长度：12个字节
      {
        //获取扫描参数
        uminfre = (udata12[0]<<24)|(udata12[1]<<16)|(udata12[2]<<8)|(udata12[3]); //PC端高字节先发送
        umaxfre = (udata12[4]<<24)|(udata12[5]<<16)|(udata12[6]<<8)|(udata12[7]);
        ustepfre = (udata12[8]<<24)|(udata12[9]<<16)|(udata12[10]<<8)|(udata12[11]);
        ucurfre = uminfre;
        //开始扫频
        USART1_Printf("\r\nSweep begins.\r\n");
        //for(i=0;i<ucount;i++)
        while(ucurfre <= umaxfre)
        {   
          Freq_convert(ucurfre);
          delay_ms(20); //等待波形稳定
          //数据采集和发送
          for(j=0;j<2;j++)
          {
            uADCConvertedValueLocal2[j] = ADCConvertedValue[j]; //获取幅值比/相位差的AD转换值
          }
          printf("%d,%d;%d\n",ucurfre,uADCConvertedValueLocal2[0],uADCConvertedValueLocal2[1]);
          ucurfre += ustepfre;
        }
        //扫频结束
        USART1_Printf("\r\nSweep is over.\r\n");
        USART_RX_STA=0; //重新开始接收
      }else 
      {
        USART_RX_STA=0; //重新开始接收
      }
    }else
    {
      //闪烁LED，提示系统正在运行
      LEDON;    //点亮LED灯
      delay_ms(500);    // 延时 
      LEDOFF;         // 熄灭LED灯
      delay_ms(500);    // 延时 //输出高电平，熄灭灯
    }
  } 
	return 1;
}

void readExcel(){
	 int i,j;
   FILE *fp = NULL ;
   fp = fopen("G:\\Desktop\\usart1.0\\config\\test.xls","r") ;
	 fseek(fp, 10L, SEEK_SET);  /*从第二行开始读取*/
   for(i = 0 ;i < 3 ; i++)
        for(j = 0 ;j < 10003 ; j++)
           fscanf(fp,"%d",&da[i][j]);
	 
}
