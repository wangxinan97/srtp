/**********************************************************
                       康威电子
										 
功能：stm32f103rct6控制
			显示：12864
接口：按键接口请参照key.h
时间：2015/11/3
版本：1.0
作者：康威电子
其他：程序借鉴正点原子，添加自己的驱动，未经作者许可，不得用于其它任何用途

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
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

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timerx_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	//定时器初始化					   //arr,psc
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim 分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断	 允许更新中断
		TIM4, //TIM2
		TIM_IT_Update  |  //TIM 更新(溢出)中断                  
		TIM_IT_Trigger,   //TIM 触发中断   （捕获中断等）					地址值	 ((uint16_t)0x0040)         
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx（定时器）外设 开启定时器
							 
}

//扫频参数
uint32_t uminfre = 10560000; //起始频率，A01模式10MHz-12MHz;S0模式100MHz-120MHz，输入范围为0~4294967295
uint32_t ustepfre = 500; //频率步长，A01模式1KHz;S0模式10KHz，输入范围为0~4294967295
uint32_t umaxfre = 10660000; //截止频率，A01模式10MHz-12MHz;S0模式100MHz-120MHz，输入范围为0~4294967295
uint32_t ucurfre; //当前频率，输入范围为0~4294967295
//AD转换参数
extern __IO uint16_t ADCConvertedValue[2]; //声明外部变量，存放12位模数转换器结果
vu16 uADCConvertedValueLocal2[2]; //幅值比/相位差的AD转换值

uint32_t ufre_buf[1000];	//存放频率信息
uint32_t uamp_buf[1000];	//存放幅值比信息
uint32_t uphs_buf[1000];	//存放相位差信息

void TIM4_IRQHandler(void)   //TIM3中断
{
	uint8_t j;
  uint8_t showstr[10]={0};
	uint16_t ucount=0;	//存放个数
	uint8_t uepsilon=2;	//收敛精度
  uint16_t best; //最优点位置
	uint32_t difference;

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		
		LCD_Refresh_Gram();//更新显示
		//频率扫描
		ucurfre = uminfre;
		//USART1_Printf("\r\nSweep begins.\r\n");//开始扫频
		while(ucurfre <= umaxfre)
		{   
		  Freq_convert(ucurfre);
			delay_ms(20); //等待波形稳定
			//数据采集和发送
			for(j=0;j<2;j++)
			{
			  uADCConvertedValueLocal2[j] = ADCConvertedValue[j]; //获取幅值比/相位差的AD转换值
			}
			//printf("%d,%d;%d\n",ucurfre,uADCConvertedValueLocal2[0],uADCConvertedValueLocal2[1]);
			ufre_buf[ucount] = ucurfre;	
      uamp_buf[ucount] = uADCConvertedValueLocal2[0];	
      uphs_buf[ucount] = uADCConvertedValueLocal2[1];	
      ucount++;	
			ucurfre += ustepfre;
		}
  	//USART1_Printf("\r\nSweep is over.\r\n");//扫频结束
    //峰值搜索
		best = Golddiv(uamp_buf,ucount,uepsilon);
		//USART1_Printf("\r\nThe results of peak search:\r\n");//峰值搜索结果
		//printf("%d,%d;%d\n",ufre_buf[best],uamp_buf[best],uphs_buf[best]);
		difference = 10671000-ufre_buf[best];//21℃对应10650000
		
		//液晶显示
		//LCD_Show_CEStr(2*8,3,"T:");
		sprintf((char *)&showstr, "%3d.%1d",difference/1000,(difference%1000)/100);//字符转换
		OLED_ShowString(4*8, 3, showstr);
		LCD_Show_CEStr(10*8,3,"C");
	}
}

//黄金分割法
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





