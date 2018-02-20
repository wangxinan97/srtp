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
	
	//串口数据接收
	extern uint16_t USART_RX_STA; //声明外部变量，接收状态标记	  
	extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //声明外部变量，接收缓冲，最大USART_REC_LEN个字节
	uchar udata12[12] = {0}; //存放接收到的数据：0123起始频率+4567频率移动+89扫描点数
  
	//扫频参数
	uint32_t uminfre = 10000000; //起始频率，A01模式10MHz-12MHz;S0模式100MHz-120MHz，输入范围为0~4294967295
	uint32_t ustepfre = 2000; //频率步长，A01模式1KHz;S0模式10KHz，输入范围为0~4294967295
	//uint16_t ucount = 1000; //扫描点数，输入范围为0~65535
	uint32_t umaxfre = 12000000; //截止频率，A01模式10MHz-12MHz;S0模式100MHz-120MHz，输入范围为0~4294967295
	uint32_t ucurfre; //当前频率，输入范围为0~4294967295
  
	//AD转换参数
	extern __IO uint16_t ADCConvertedValue[2]; //声明外部变量，存放12位模数转换器结果
	vu16 uADCConvertedValueLocal2[2]; //幅值比/相位差的AD转换值
	
	//初始化
	delay_init(72);	//初始化延时函数
	USARTx_Init(9600);
	STM32_ADC_Configuration();
	Init_ad9910();
	LED_Init();	//初始化LED接口
	
#ifdef SOFTWAREDEBUG
		return simulation();
#endif

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
			LEDON; 		//点亮LED灯
		  delay_ms(500);	  // 延时	
    	LEDOFF;				  // 熄灭LED灯
		  delay_ms(500);	  // 延时	//输出高电平，熄灭灯
		}
	}	
}
