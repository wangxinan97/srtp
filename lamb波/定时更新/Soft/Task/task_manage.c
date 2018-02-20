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


#include "task_manage.h"
#include "delay.h"
#include "key.h"
#include "AD9910.h"
#include <stdio.h>
#include <ctype.h>
#include <cstring>

#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����0
#define FLASH_SAVE_ADDR  0x0801F000  				//����FLASH �����ַ(����Ϊż��)


u8 Task_Index = 0;//�����л�
u8 Param_Mode = 0;//���������л�
u8 fre_buf[StrMax]; //����ת���ַ�����
u8 display[StrMax]; //������ʾ����
u8 P_Index = 0; //�༭λ��
u8 Task_First=1;//��һ�ν�����
u8 _return=0;
//ɨƵ����
u32 SweepMinFre = 10000000;
u32 SweepMaxFre = 12000000;
u32 SweepStepFre = 10000;
u16 SweepTime = 1;//ms
u8 SweepFlag = 0;
//ADת������
vu16  ADCConvertedValueLocal[2] = {0.000};
vu16 Precent[2] = {0.000};
vu16 Voltage[2] = {0.000};

u8 Task_Delay(u32 delay_time, u8* delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
    if(Time_Get == 0)
    {
      Time_Triger = SysTimer + delay_time;
      Time_Get = 1;
    }
    if(SysTimer >= Time_Triger)
    { 
      Time_Get = 0;
			*delay_ID = 1;		//	���������ѱ�ִ��һ��
			return 1;
    }
		return 0;
}

//u8 TaskOneToIdel(void)
//{
//	static u8 delay_ID0 = 0;
//	static u8 delay_ID1 = 0;
//	static u8 delay_ID2 = 0;
//	static u8 delay_ID3 = 0;
//	u8 delay_arrive0 = 0;
//	u8 delay_arrive1 = 0;
//	u8 delay_arrive2 = 0;
//	u8 delay_arrive3 = 0;
//	
//	delay_arrive0 = Task_Delay(100, &delay_ID0);
//	delay_arrive1 = Task_Delay(100, &delay_ID1);
//	delay_arrive2 = Task_Delay(100, &delay_ID2);
//	delay_arrive3 = Task_Delay(100, &delay_ID3);
//	if((delay_arrive0 == 0) && (delay_ID0 == 0))
//		return 0;
//	else if((delay_arrive0) && (delay_ID0))
//	{
//			//ִ�б���ʱ�����
//	}
//		
//	
//	
//	if(delay_ID0&&delay_ID1&&delay_ID2&&delay_ID3)
//	{
//		delay_ID0 = 0;
//		delay_ID1 = 0;
//		delay_ID2 = 0;
//		delay_ID3 = 0;
//	}
//}

u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
	
	if(!(*Last_delay_ID))
		return 0;
	if(Time_Get == 0)
	{
		Time_Triger = SysTimer + delay_time;
		Time_Get = 1;
	}
	if(SysTimer >= Time_Triger)
	{ 
		Time_Get = 0;
		*Last_delay_ID = 0;
		*Self_delay_ID = 1;		//	���������ѱ�ִ��һ��
		return 1;
	}
	return 0;
}
void welcome_KW(void)
{
	u8 delay_ID[11] = {0,0,0,0,0,0,0,0,0,0};
	u8 delay_ID_Welcome[3] = {0,0,0};
	u16 Wel_time0 = 50,Wel_time1 = 100;
	delay_ID[10] = 1;
	delay_ID_Welcome[2] = 1;
	LCD_Show_CEStr(0,6,"ADF4351");
	LCD_Show_CEStr(56,6,"��ʼ��");
	while(1)
	{
	if(TaskCycleDelay(Wel_time0, &delay_ID[10], &delay_ID[0]))
		LCD_Show_CEStr(32,0,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[0], &delay_ID[1]))
		LCD_Show_CEStr(48,0,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[1], &delay_ID[2]))
		LCD_Show_CEStr(64,0,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[2], &delay_ID[3]))
		LCD_Show_CEStr(80,0,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[3], &delay_ID[4]))
		LCD_Show_CEStr(16,2,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[4], &delay_ID[5]))
		LCD_Show_CEStr(32,2,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[5], &delay_ID[6]))
		LCD_Show_CEStr(48,2,"Ϊ");
	if(TaskCycleDelay(Wel_time0, &delay_ID[6], &delay_ID[7]))
		LCD_Show_CEStr(64,2,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[7], &delay_ID[8]))
		LCD_Show_CEStr(80,2,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[8], &delay_ID[9]))
		LCD_Show_CEStr(96,2,"��");
	if(TaskCycleDelay(Wel_time0, &delay_ID[9], &delay_ID[0]))
		break;
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[2], &delay_ID_Welcome[0]))
		LCD_Show_CEStr(104,6,".  ");
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[0], &delay_ID_Welcome[1]))
		LCD_Show_CEStr(112,6,".");
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[1], &delay_ID_Welcome[2]))
		LCD_Show_CEStr(120,6,".");
		LCD_Refresh_Gram();
	}
}
//�����ݷŵ���һ�����棬��ʾ��FloatNumС����λ����CursorEn���ʹ��
void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn)
{
	int i, len;
	
	len = strlen(source);
	i = len - FloatNum;//��������
	if(FloatNum>0)dis[i] = '.';
	for (i = 0; i < len; i++)
	{
		if(i < (len-FloatNum)) dis[i] = source[i];
		else 
		{ dis[i+1] = source[i]; }
	}
	
	if(CursorEn)
	{
		if(dispoint < (len-FloatNum)) dis[dispoint] += 128;
		else dis[dispoint+1] += 128;	
	}
}

void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	//�����ж�
	switch(Key_Value)
	{
		case K_4_S: fre_buf[P_Index]++;break;
		case K_4_L: fre_buf[P_Index]++;break;
		case K_5_L: P_Index++;break;
		case K_5_S: P_Index++;break;
		case K_1_L: P_Index--;break;
		case K_1_S: P_Index--;break;
		case K_3_S: fre_buf[P_Index]--;break;
		case K_3_L: fre_buf[P_Index]--;break;
		case K_2_S: Param_Mode++;break;
	}
	if(fre_buf[P_Index] == '/') fre_buf[P_Index] = '9';//<'0'
	if(fre_buf[P_Index] == ':') fre_buf[P_Index] = '0';//>'9'
	//�����л�
	if(Key_Value == K_2_L) 
	{	
		Task_Index++;
		LCD_Clear();
	}
	if(Key_Value == K_2_L || Key_Value == K_2_S) Task_First=1;//���µ�һ�ν���
	if(Task_Index >= Interface) Task_Index=0;
	switch(Task_Index)
	{
		case 1: 
			Task0_PointFre(Key_Value);
			break;
		case 0: 
			Task3_SweepFre(Key_Value);
			break;
	}
	//��������
	if(Task_Index != 1) SweepFlag=0;//
	else SweepFlag=1;
	//�̶���ʾ
	//LCD_Show_CEStr(0,0,"9910");
}
//����

void Task0_PointFre(u32 Key_Value)//���Ҳ� (10M) 0-100 000 000
{
	static u32 SinFre = 10650000;
	u8 showstr[StrMax]={0};
	
	if(Task_First)
	{
		Task_First = 0;
		Key_Value = K_2_S;
		Param_Mode %= 3;//�������ģʽ���Ա�װ���ʼֵ
		if(Param_Mode == 0) sprintf(fre_buf, "%9d", SinFre);
		if(Param_Mode == 1) sprintf(fre_buf, "%9d", Voltage[0]);
		if(Param_Mode == 2) sprintf(fre_buf, "%9d", Voltage[1]);
		//LCD_Show_CEStr(64-8*3,0,"���Ҳ�");
		_return=1;
	}

	if(Key_Value != K_NO)
	{
		//�ж�
		if(Param_Mode == 0)//Ƶ������
		{
			P_Index = P_Index%9;//����λ��
		  SinFre = atol(fre_buf);//�ַ�ת�����֣��ж�������
		  if(SinFre>1000000000) SinFre=1000000000;//��������
		  if(SinFre<0) SinFre=0;
		  sprintf(fre_buf, "%9d", SinFre);//�ַ�ת��
		}
		//��ʾ
		sprintf(showstr, "%9d", SinFre);//�ַ�ת��
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		Copybuf2dis(showstr, display, P_Index, 0, 1);
		OLED_ShowString(64-4*11+24, 2, display);
		LCD_Show_CEStr(64-4*11+9*8+24,2,"Hz");
		LCD_Show_CEStr(0,2,"Fre:");
		//���ݴ���д��
		
		Freq_convert(SinFre);
		delay_ms(20); //�������ȶ�֮���ٲɼ�
		
		ADCConvertedValueLocal[0] = ADCConvertedValue[0]; //��ȡ��ֵ�ȵ�ADת��ֵ
		ADCConvertedValueLocal[1] = ADCConvertedValue[1]; //��ȡ��λ���ADת��ֵ
		Precent[0] = (ADCConvertedValueLocal[0]*100/0x1000);	//����ٷֱ�
    Precent[1] = (ADCConvertedValueLocal[1]*100/0x1000);	//����ٷֱ�
		Voltage[0] = Precent[0]*33;						  // 3.3V�ĵ�ƽ�������Ч��ƽ
    Voltage[1] = Precent[1]*33;						  // 3.3V�ĵ�ƽ�������Ч��ƽ
		//���ڴ�ӡ
		printf("%4d,%4d", Voltage[0],Voltage[1]);
				
		if(Param_Mode == 1)//��ֵ��ADת������
		{
			P_Index %= 4;//����λ��
		  sprintf(fre_buf, "%4d", Voltage[0]);//�ַ�ת��
		}
		//��ʾ
		sprintf(showstr, "%4d", Voltage[0]);//�ַ�ת��
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		Copybuf2dis(showstr, display, P_Index, 0, 1);
		display[4]=0;//ֻ��ʾ3λ
		OLED_ShowString(64-4*11+24, 4, display);
		LCD_Show_CEStr(64-4*11+4*8+24,4,"mV");
		LCD_Show_CEStr(0,4,"VMAG:");
		//���ݴ���д��
		
		if(Param_Mode == 2)//��λ��ADת������
		{
			P_Index %= 4;//����λ��
		  sprintf(fre_buf, "%4d", Voltage[1]);//�ַ�ת��
		}
		//��ʾ
		sprintf(showstr, "%4d", Voltage[1]);//�ַ�ת��
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		Copybuf2dis(showstr, display, P_Index, 0, 1);
		display[4]=0;//ֻ��ʾ3λ
		OLED_ShowString(64-4*11+24, 6, display);
		LCD_Show_CEStr(64-4*11+4*8+24,6,"mV");
		LCD_Show_CEStr(0,6,"VPHS:");
		//���ݴ���д��
		
		_return=1;
	}
}

void Task1_Square(u32 Key_Value)//���� 300k
{}
void Task2_Triangular(u32 Key_Value)//���ǲ� 1M
{}

void Task3_SweepFre(u32 Key_Value)//ɨƵ
{
  u32 SinFre = SweepMinFre;
	u8 showstr[StrMax]={0};
	
	if(Task_First)
	{
		Task_First = 0;//�����һ�ν�����
		Key_Value = K_2_S;
		Param_Mode %= 4;//�������ģʽ���Ա�װ���ʼֵ
		if(Param_Mode == 0) sprintf(fre_buf, "%9d", SweepMinFre);
		if(Param_Mode == 1) sprintf(fre_buf, "%9d", SweepMaxFre);
		if(Param_Mode == 2) sprintf(fre_buf, "%9d", SweepStepFre);
		if(Param_Mode == 3) sprintf(fre_buf, "%3d", SweepTime);
		//LCD_Show_CEStr(64-8*3,0," ɨƵ ");//ģʽ����
		_return=1;//������ʾ��־ 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//��СƵ������
		{
			P_Index %= 9;//����λ��
			SweepMinFre = atol(fre_buf);//�ַ�ת��
			if(SweepMinFre>SweepMaxFre) SweepMinFre=SweepMaxFre;//��������
			if(SweepMinFre<0) SweepMinFre=0;
			sprintf(fre_buf, "%9d", SweepMinFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%9d", SweepMinFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(64-4*11+16, 0, display);
		LCD_Show_CEStr(64-4*11+9*8+16,0,"Hz");
		LCD_Show_CEStr(0,0,"Min:");
		
		if(Param_Mode == 1)//���Ƶ������
		{
			P_Index %= 9;//����λ��
			SweepMaxFre = atol(fre_buf);//�ַ�ת��
			if(SweepMaxFre>100000000) SweepMaxFre=100000000;//��������
			if(SweepMaxFre<0) SweepMaxFre=0;
			sprintf(fre_buf, "%9d", SweepMaxFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%9d", SweepMaxFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(64-4*11+16, 2, display);
		LCD_Show_CEStr(64-4*11+9*8+16,2,"Hz");
		LCD_Show_CEStr(0,2,"Max:");
		
		if(Param_Mode == 2)//Ƶ�ʲ�������
		{
			P_Index %= 7;//����λ��
			SweepStepFre = atol(fre_buf);//�ַ�ת��
			if(SweepStepFre>10000) SweepStepFre=10000-1;//��������
			if(SweepStepFre<0) SweepStepFre=0;
			sprintf(fre_buf, "%7d", SweepStepFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%7d", SweepStepFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 2) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[7]=0;//ֻ��ʾ7λ
		OLED_ShowString(64-4*11+16, 4, display);
		LCD_Show_CEStr(64-4*11+16+8*7,4,"Hz");
		LCD_Show_CEStr(0,4,"Stp:");
						
		while(SinFre <= SweepMaxFre)
		{
			Freq_convert(SinFre);
		  delay_ms(20); //�������ȶ�֮���ٲɼ�
		  ADCConvertedValueLocal[0] = ADCConvertedValue[0]; //��ȡ��ֵ�ȵ�ADת��ֵ
		  ADCConvertedValueLocal[1] = ADCConvertedValue[1]; //��ȡ��λ���ADת��ֵ
		  Precent[0] = (ADCConvertedValueLocal[0]*100/0x1000);	//����ٷֱ�
      Voltage[0] = Precent[0]*33;						  // 3.3V�ĵ�ƽ�������Ч��ƽ
		  Precent[1] = (ADCConvertedValueLocal[1]*100/0x1000);	//����ٷֱ�
      Voltage[1] = Precent[1]*33;						  // 3.3V�ĵ�ƽ�������Ч��ƽ
			printf("%4d,%4d\n", Voltage[0],Voltage[1]);//���ڴ�ӡ
			delay_ms(80);
			SinFre = SinFre + SweepStepFre;
		}
      
		if(Param_Mode == 3)//ADת������
		{
			P_Index %= 4;//����λ��
			sprintf(fre_buf, "%4d", Voltage[0]);//�ַ�ת��
		}
		//��ʾ
		sprintf(showstr, "%4d", Voltage[0]);//�ַ�ת��
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		Copybuf2dis(showstr, display, P_Index, 0, 1);
		display[4]=0;//ֻ��ʾ3λ
		OLED_ShowString(0, 6, display);
		LCD_Show_CEStr(4*8,6,"mV");
		//���ݴ���д��

		if(Param_Mode == 4)//ADת������
		{
			P_Index %= 4;//����λ��
			sprintf(fre_buf, "%4d", Voltage[1]);//�ַ�ת��
		}
		//��ʾ
		sprintf(showstr, "%4d", Voltage[1]);//�ַ�ת��
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		Copybuf2dis(showstr, display, P_Index, 0, 1);
		display[4]=0;//ֻ��ʾ3λ
		OLED_ShowString(64, 6, display);
		LCD_Show_CEStr(64+4*8,6,"mV");
		//���ݴ���д��
		
		_return=1;
	}
}

//
void fre_buf_change(u8 *strbuf)
{
	int i;
	for (i = 0 ; i < strlen(strbuf); i++)
	 if(strbuf[i]==0x20) strbuf[i] = '0';
	for (i = 0 ; i < strlen(fre_buf); i++)
	 if(fre_buf[i]==0x20) fre_buf[i] = '0';
}

