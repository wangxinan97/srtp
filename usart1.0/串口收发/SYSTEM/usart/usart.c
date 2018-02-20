/**********************************************************
* @ File name -> usart.c
* @ Version   -> V1.0
* @ Date      -> 12-26-2013
* @ Brief     -> 系统串口设置相关的函数
**********************************************************/

#include "usart.h"

/**********************************************************
   加入以下代码，支持printf函数，而不需要选择use MicroLIB
**********************************************************/

#if 1

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	USART1->DR = (uint8_t)ch;	//发送数据 
  while((USART1->SR & 0x40) == 0){}	//循环发送，直到发送完毕   	
	return ch;
}
#endif

/**********************   end   **************************/

//将pch指定为char型的指针，char型是8位，这里与上面ASCII码是0~127有关,无符号8位二进制如果全1
void USART1_Printf(char *pch) 
{
	/* 这里是判断'\0'是否到了字符串的行尾，这个字符是看不到的，字符串的结束符就是'\0'
	 * 如果具体还有不懂的请看C语言，这里通过while()循环，逐个字符逐个字符的传输和显示 */
    while(*pch != '\0')		 /* 我们这里用指针表示，*pch就是一个字符，pch就是该字符的地址 */
	  {
        USART_SendData(USART1,*pch); /* 在这里我们将这个要显示的字符地址传输给另外一个函数 */
		/* 当发送完一个字节后,检测一下“TDR（串口的数据传输寄存器）”是否为空了，因为有可能在一些
		 * 其他某些模式，缓冲寄存器可能会有很多数据，需要一点时间才能清空，其实增加了这句代码之后
		 * 会让我们的串口驱动更加健壮，在各种情况下都能正常稳定的工作 */
		    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} 
		/* 以下是我们查看《STM32F103中文手册》获得494页24.6.1节 状态寄存器（USART_SR）章节
		这里说的就是我们程序代码里的USART_FLAG_TXE这个标志位，发送完毕后，再进行标志清空，进行下一次传输
		TXE:发送数据寄存器空
		当TDR寄存器中的数据被硬件转移到移位寄存器的时候，该位被硬件置位。
		0：数据还没有被转移到移位寄存器；
		1：数据已经被转移到移位寄存器。
		注意：单缓冲器传输中使用该位。*/
 		    USART_ClearFlag(USART1, USART_FLAG_TXE);
        pch++;			   /* 此时pch地址加1，指向下一个字符 */
    }
}

/**********************************************************
* 函数功能 ---> 初始化IO 串口1
* 入口参数 ---> bound：波特率	
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void USARTx_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
#ifdef EN_USART1_RX	//如果使能了接收中断
	
	USART1->CR1 |= 1 << 8;	//PE中断使能
	USART1->CR1 |= 1 << 5;	//接收缓冲区非空中断使能
	
	MY_NVIC_Init(3, 3, USART1_IRQn, NVIC_PriorityGroup_2);	//中断分组2，最低优先级
	
#endif
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		
	USART_DeInit(USART1);  //复位串口1
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;	//设置波特率，一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}

//=========================================================
 
#ifdef EN_USART1_RX   //如果使能了接收

//=========================================================

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误 

uint8_t USART_RX_BUF[USART_REC_LEN];	//接收缓冲,最大USART_REC_LEN个字节

//接收状态
//bit15：接收完成标志
//bit14：接收到0x0d
//bit13~0：接收到的有效字节数目，最大512字节

uint16_t USART_RX_STA=0;	//接收状态标记	  

/**********************************************************
* 函数功能 ---> 串口1接收中断服务程序
* 入口参数 ---> bound：波特率	
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void USART1_IRQHandler(void)
{
	uint8_t res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收到数据，发生接收中断
	{
		res = USART_ReceiveData(USART1);	//读取接收到的数据，USART2->DR，硬件自动清除中断标志位 
		
		if((USART_RX_STA & 0x8000) == 0)//接收未完成
		{
			//if(USART_RX_STA & 0x4000)//接收到了0x0d
			//{
				/***********************************************
                                  修改内容如下
                    当用户数据当中有0x0d的时候修正的错误的判断
				***********************************************/
				
				//if(res != 0x0a)
				//{
					//USART_RX_BUF[USART_RX_STA & 0x3fff] = 0x0d;	//补上丢失的0x0d数据
					//USART_RX_STA++;
					//USART_RX_BUF[USART_RX_STA & 0x3fff] = res;	//继续接收数据
					//USART_RX_STA++;
					//USART_RX_STA &= 0xbfff;						//清除0x0d标志
				//}
				
				/***********************************************
                                      修改完成
				***********************************************/
				
				//else	USART_RX_STA |= 0x8000;	//接收完成了
			//}
			//else //还没收到0x0d
			//{	
				//if(res == 0x0d)	USART_RX_STA |= 0x4000;
				//else
				//{
					USART_RX_BUF[USART_RX_STA & 0x3fff] = res;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN - 1))	USART_RX_STA = 0;//接收数据错误,重新开始接收
          if(res == '#')	USART_RX_STA |= 0x8000;	//接收数据以#结束
				//}		 
			//}
		}	//end 接收未完成   		 
	}	//end 接收到数据
}

//=========================================================

#endif	//end使能接收

//=========================================================

