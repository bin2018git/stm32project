/**********************************************
*���ƣ�main.c
*������stm32Project���Գ���
*���ߣ�li
*ʱ�䣺2017-04-14
***********************************************/
#include "stm32f10x.h"
//#include "stdio.h"
//#include "stdlib.h"
#include "string.h"
//#include "math.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "showchinese.h"
#include "ds1302.h"
#include "rc522.h"
#include "timer.h"
//#include "font.h"
//#include "usart.h"
//#include <string.h>

u8 tempdate=0;
u8 InOutFlag=0;
u8 NotUseCard=0;
u8 NotUseCardTime=0;
u8 RemainParkSpace=99;
u16 ParkTime=0;
u32 timeCount=0;
u16 count=0;


u8 status; //״̬����ֵ
unsigned char SendBuffer[30];//д��������ݴ洢
unsigned char MoneyBuffer[30];//д��������ݴ洢
unsigned char MLastSelectedSnr[4];//�洢�������к�
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};//��ʼ����

unsigned char KeyA[6] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11};//KeyA����
unsigned char KeyB[6] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};//KeyB����

unsigned char data[16]={//��ÿ�������Ŀ�3�������ã�6�ֽ�KeyA��4�ֽڴ�ȡ���ƣ�0xff,0x07,0x80,0x69����6�ֽ�KeyB
							0x11,0x11,0x11,0x11,//
							0x11,0x11,0xFF,0x07,//
							0x80,0x69,0x22,0x22,//
							0x22,0x22,0x22,0x22 //
						}; 

//M1����ĳһ��дΪ���¸�ʽ����ÿ�ΪǮ�����ɽ��տۿ�ͳ�ֵ����
//4�ֽڽ����ֽ���ǰ����4�ֽڽ��ȡ����4�ֽڽ�1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ��
unsigned char data1[16]={  //��ʼ��1000��E8��03��00��00��
							0xE8,0x03,0x00,0x00,//4�ֽڽ��
							0x17,0xFC,0xFF,0xFF,//4�ֽڽ��ȡ��
							0xE8,0x03,0x00,0x00,//4�ֽڽ��
							0x02,0xFD,0x02,0xFD //1�ֽڿ��ַ��1�ֽڿ��ַȡ����1�ֽڿ��ַ��1�ֽڿ��ַȡ��
						}; 
// 	��Ϊ16��������ÿ������Ϊ4�飬ÿ��16���ֽ�,�Կ�Ϊ��ȡ��λ
unsigned char data2[4]={1,0,0,0};//�ۿ�Զ�Ӧ���ֽڽ����пۿ�


//ͣ��״̬����1���ֽڴ��״̬��־����2-7�ֽڴ���볡ʱ�䣬��8-13��ų���ʱ��
unsigned char UserInfoData[16]={
							0x00,0x17,0x05,0x22,
							0x08,0x30,0x59,0x17,
							0x05,0x22,0x08,0x31,
							0x59,0x00,0x00,0x00
						}; 

__packed typedef struct  
{										    
 	u8 year;	//��
	u8 mon;		//��
	u8 date;	//��
	u8 hour;	//ʱ
	u8 min;		//��
	u8 sec;		//��
	u8 week;
}TimeStr; 

TimeStr TimeAnay; //ʱ��ṹ��

u8 DateTime[7] = {0,0,0,0,0,0,0};   //�꣬�£��գ�ʱ���֣��룬��


//��ʼ����������
void InitAll(void)
{
	//ϵͳʱ�ӳ�ʼ��
	SystemInit();
		
	//��ʱ��ʼ��
	delay_init(72);
	
	//led�Ƴ�ʼ��
	LED_Init();		
	
	BEEP_Init();

	//LCD��ʾ����ʼ��
	LCD_Init();
	
	//ds1302��ʼ��
	InitClock();           
	
	//RC522��ʼ��
	RC522_init();
	
	//��ʱ������
	TIM2_Configuration();

}



void CardNumShow(u8 *temp)
{
	u8 hex_temp[8],i;

	hex_temp[0]=(temp[0]&0xf0)>>4;
	hex_temp[1]=temp[0]&0x0f;
	hex_temp[2]=(temp[1]&0xf0)>>4;
	hex_temp[3]=temp[1]&0x0f;
	hex_temp[4]=(temp[2]&0xf0)>>4;
	hex_temp[5]=temp[2]&0x0f;
	hex_temp[6]=(temp[3]&0xf0)>>4;
	hex_temp[7]=temp[3]&0x0f;

	for(i=0;i<8;i++)	//
	{
		if(hex_temp[(i%8)]<=0x09)
		{
			LCD_ShowNum(50+i*8,80,hex_temp[(i%8)],1,16);
		}
		else 
		{
			switch(hex_temp[(i%8)])
			{
				case 0x0a:LCD_ShowString(50+i*8,80,200,16,16,"A");break;				
				case 0x0b:LCD_ShowString(50+i*8,80,200,16,16,"B");break;
				case 0x0c:LCD_ShowString(50+i*8,80,200,16,16,"C");break;
				case 0x0d:LCD_ShowString(50+i*8,80,200,16,16,"D");break;
				case 0x0e:LCD_ShowString(50+i*8,80,200,16,16,"E");break;
				case 0x0f:LCD_ShowString(50+i*8,80,200,16,16,"F");break;
			}
		}
	}
}



void LCD_ShowTime(u8 *p)
{
			//��,��,��,ʱ,��, ��
	//���BCD��ʮ����ת��
	TimeAnay.year=(DateTime[0]>>4)*10+(DateTime[0]&0x0f);
	TimeAnay.mon=(DateTime[1]>>4)*10+(DateTime[1]&0x0f);
	TimeAnay.date=(DateTime[2]>>4)*10+(DateTime[2]&0x0f);
	TimeAnay.hour=(DateTime[3]>>4)*10+(DateTime[3]&0x0f);
	TimeAnay.min=(DateTime[4]>>4)*10+(DateTime[4]&0x0f);
	TimeAnay.sec=(DateTime[5]>>4)*10+(DateTime[5]&0x0f); //������ô�߾���,�뼶ʱ�䲻�ÿ���
	TimeAnay.week=(DateTime[6]>>4)*10+(DateTime[6]&0x0f); 
	
	POINT_COLOR=BLUE;	
	if(tempdate != TimeAnay.date)
	{
		tempdate=TimeAnay.date;			//����������ÿһ�����һ��
		Test_Show_CH_Font24(132,266,0,BLUE);				//��
		Test_Show_CH_Font24(156,266,1,BLUE);				//��
		Test_Show_CH_Font24(180,266,TimeAnay.week+1,BLUE);	//��
		
		LCD_ShowNum(100,290,20,2,24);	
		LCD_ShowNum(124,290,TimeAnay.year,2,24);		  //2017��		
		LCD_ShowChar(148,290,'-',24,0);
		LCD_ShowxNum(164,290,TimeAnay.mon,2,24,0x80);     //05��
		LCD_ShowChar(188,290,'-',24,0);
		LCD_ShowxNum(204,290,TimeAnay.date,2,24,0x80);    //17��
	}
	
	LCD_ShowNum(68,40,TimeAnay.hour,2,24);			//ʱ
	LCD_ShowChar(92,40,':',24,0);
	LCD_ShowxNum(108,40,TimeAnay.min,2,24,0x80);	//��
	LCD_ShowChar(132,40,':',24,0);
	LCD_ShowxNum(148,40,TimeAnay.sec,2,24,0x80);	//��
}





int main(void)
{
	InitAll();
	PcdReset();		     //rc522��λ
	PcdAntennaOff(); 	 //rc522�ر�����
	delay_ms(1);       //�ر����ߺʹ�������Ҫ�������1ms
	PcdAntennaOn();  	 //rc522��������
	M500PcdConfigISOType( 'A' );   //���ó�ISO14443_A��Э�鿨

	
	delay_ms(1000);
		  
//	LCD_ShowString(30,40,200,24,24,"Mini STM32 ^_^");	
//	LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");	
 	/*****ͣ�����շ�ϵͳ*****/
	Test_Show_CH_Font24(36,8,9,BLUE);
	Test_Show_CH_Font24(60,8,10,BLUE);
	Test_Show_CH_Font24(84,8,11,BLUE);		
	Test_Show_CH_Font24(108,8,12,BLUE);
	Test_Show_CH_Font24(132,8,13,BLUE);
	Test_Show_CH_Font24(156,8,14,BLUE);		
	Test_Show_CH_Font24(180,8,15,BLUE);
	
	
	Test_Show_CH_Font16(10,80,9,BLUE);//��
	Test_Show_CH_Font16(26,80,10,BLUE);//��	
	
	POINT_COLOR=BLUE;	
	LCD_ShowString(42,80,200,16,16,":");//���ţ�
	
	   /******���******/
	Test_Show_CH_Font16(10,104,22,BLUE);//
	Test_Show_CH_Font16(26,104,23,BLUE);//
	LCD_ShowString(42,104,200,16,16,":");	
	
	Test_Show_CH_Font16(10,128,25,BLUE);//��
	Test_Show_CH_Font16(26,128,17,BLUE);//��
	Test_Show_CH_Font16(42,128,11,BLUE);//ʱ
	Test_Show_CH_Font16(58,128,12,BLUE);//��
	LCD_ShowString(74,128,200,16,16,":");//ʱ�䣺:	

	Test_Show_CH_Font16(10,152,24,BLUE);//��
	Test_Show_CH_Font16(26,152,17,BLUE);//��
	Test_Show_CH_Font16(42,152,11,BLUE);//ʱ
	Test_Show_CH_Font16(58,152,12,BLUE);//��
	LCD_ShowString(74,152,200,16,16,":");//ʱ�䣺:	

	Test_Show_CH_Font16(10,176,15,BLUE);//ͣ
	Test_Show_CH_Font16(26,176,16,BLUE);//��
	Test_Show_CH_Font16(42,176,11,BLUE);//ʱ
	Test_Show_CH_Font16(58,176,12,BLUE);//��
	LCD_ShowString(74,176,200,16,16,":");//ʱ�䣺:	

	Test_Show_CH_Font16(10,200,28,BLUE);//��
	Test_Show_CH_Font16(26,200,29,BLUE);//��
	Test_Show_CH_Font16(42,200,30,BLUE);//��
	Test_Show_CH_Font16(58,200,23,BLUE);//��
	LCD_ShowString(74,200,200,16,16,":");	

	Test_Show_CH_Font16(10,224,26,BLUE);//ʣ
	Test_Show_CH_Font16(26,224,27,BLUE);//��
	Test_Show_CH_Font16(42,224,16,BLUE);//��
	Test_Show_CH_Font16(58,224,14,BLUE);//λ
	LCD_ShowString(74,224,200,16,16,":");	
	LCD_ShowxNum(94,220,RemainParkSpace,2,24,0x80);
	
	ReadDS1302Clock(DateTime);
	LCD_ShowTime(DateTime);
	
	while(1)
	{					
		//delay_ms(200);
		if(NotUseCard==0)
		{
			status= PcdRequest(PICC_REQALL,SendBuffer);//��һ��Ѱ�����������еĿ�
			if(status!=0)
				status= PcdRequest(PICC_REQALL,SendBuffer);//�ڶ���Ѱ�����������еĿ�	
			if(status==0)
			{
				TIM_Cmd(TIM2, DISABLE);				
				PcdAnticoll(SendBuffer);//������ϵ�кţ���λ��ǰ 
				memcpy(MLastSelectedSnr,SendBuffer,4);//���濨�����кŵ�MLastSelectedSnr	
				PcdSelect(MLastSelectedSnr);//ѡ�񿨣�MLastSelectedSnr�������к�								
				status = PcdAuthState(0x60, 0x01, KeyA, MLastSelectedSnr);// У����ַ0x01�Ŀ�����
				if(status==0)
				{					
					PcdRead(0x01,SendBuffer);//�������ַ0x01������
					
					if(SendBuffer[0]==0)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_2);							
						InOutFlag=0;//�볡
					}
					else
					{
						GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
						InOutFlag=1;//����
					}
					
					NotUseCard=2;
					NotUseCardTime=0;					
			
					CardNumShow(MLastSelectedSnr);//��ʾѰ���Ŀ������к�					
					
					if(InOutFlag==0)//�볡
					{
						PcdRead(0x02,MoneyBuffer);//�������ַ0x02������
						UserInfoData[0]=1;
						UserInfoData[1]=TimeAnay.year;
						UserInfoData[2]=TimeAnay.mon;
						UserInfoData[3]=TimeAnay.date;
						UserInfoData[4]=TimeAnay.hour;
						UserInfoData[5]=TimeAnay.min;
						UserInfoData[6]=TimeAnay.sec;
						UserInfoData[7]=90;
						PcdWrite(0x01,UserInfoData);//д�����ݵ����ַ0x01
						
						LCD_ShowNum(82,128,20,2,16);	
						LCD_ShowNum(98,128,TimeAnay.year,2,16);		  //2017��		
						LCD_ShowChar(114,128,'-',16,0);
						LCD_ShowxNum(122,128,TimeAnay.mon,2,16,0x80);     //05��
						LCD_ShowChar(138,128,'-',16,0);
						LCD_ShowxNum(146,128,TimeAnay.date,2,16,0x80);    //17��
						
						LCD_ShowNum(170,128,TimeAnay.hour,2,16);			//ʱ
						LCD_ShowChar(186,128,':',16,0);
						LCD_ShowxNum(194,128,TimeAnay.min,2,16,0x80);	//��
						LCD_ShowChar(210,128,':',16,0);
						LCD_ShowxNum(218,128,TimeAnay.sec,2,16,0x80);	//��	
						
						LCD_ShowxNum(62,100,MoneyBuffer[0],2,24,0x80);	//���
						Test_Show_CH_Font16(86,104,31,BLUE);//Ԫ
						
						
						RemainParkSpace-=1;
						LCD_ShowxNum(94,220,RemainParkSpace,2,24,0x80);	//ʣ�೵λ

						POINT_COLOR=RED;
						LCD_ShowString(34,266,200,16,16,"Welcome!");		
						POINT_COLOR=BLUE;	

					}
					else if(InOutFlag==1)//����
					{	
						PcdRead(0x02,MoneyBuffer);//�������ַ0x02������	
						//MoneyBuffer[0]=90;						
						if(TimeAnay.mon!=SendBuffer[2])
						{
							ParkTime=(TimeAnay.mon-SendBuffer[2])*30*24*60+(TimeAnay.date-SendBuffer[3])*24*60+(TimeAnay.hour-SendBuffer[4])*60+(TimeAnay.min-SendBuffer[5]);
						}
						else
						{
								if(TimeAnay.date!=SendBuffer[3])
								{
										ParkTime=(TimeAnay.date-SendBuffer[3])*24*60+(TimeAnay.hour-SendBuffer[4])*60+(TimeAnay.min-SendBuffer[5]);
								}
								else
								{
										ParkTime=(TimeAnay.hour-SendBuffer[4])*60+(TimeAnay.min-SendBuffer[5]);
								}
						}
						if(MoneyBuffer[0]>ParkTime)
						{
								MoneyBuffer[0]-=ParkTime;
						}
						else
						{
								
						}

						PcdWrite(0x02,MoneyBuffer);//д�����ݵ����ַ0x02						
						UserInfoData[0]=0;
						UserInfoData[1]=0;
						UserInfoData[2]=0;
						UserInfoData[3]=0;
						UserInfoData[4]=0;
						UserInfoData[5]=0;
						UserInfoData[6]=0;
						
						PcdWrite(0x01,UserInfoData);//д�����ݵ����ַ0x01

						PcdHalt();//��ֹ���Ĳ���		
						
						//��ʾ�볡ʱ��
						LCD_ShowNum(82,128,20,2,16);	
						LCD_ShowNum(98,128,SendBuffer[1],2,16);		  //2017��		
						LCD_ShowChar(114,128,'-',16,0);
						LCD_ShowxNum(122,128,SendBuffer[2],2,16,0x80);     //05��
						LCD_ShowChar(138,128,'-',16,0);
						LCD_ShowxNum(146,128,SendBuffer[3],2,16,0x80);    //17��
						
						LCD_ShowNum(170,128,SendBuffer[4],2,16);			//ʱ
						LCD_ShowChar(186,128,':',16,0);
						LCD_ShowxNum(194,128,SendBuffer[5],2,16,0x80);	//��
						LCD_ShowChar(210,128,':',16,0);
						LCD_ShowxNum(218,128,SendBuffer[6],2,16,0x80);	//��	
						
						//��ʾ����ʱ��
						LCD_ShowNum(82,152,20,2,16);	
						LCD_ShowNum(98,152,TimeAnay.year,2,16);		  //2017��		
						LCD_ShowChar(114,152,'-',16,0);
						LCD_ShowxNum(122,152,TimeAnay.mon,2,16,0x80);     //05��
						LCD_ShowChar(138,152,'-',16,0);
						LCD_ShowxNum(146,152,TimeAnay.date,2,16,0x80);    //17��
						
						LCD_ShowNum(170,152,TimeAnay.hour,2,16);			//ʱ
						LCD_ShowChar(186,152,':',16,0);
						LCD_ShowxNum(194,152,TimeAnay.min,2,16,0x80);	//��
						LCD_ShowChar(210,152,':',16,0);
						LCD_ShowxNum(218,152,TimeAnay.sec,2,16,0x80);	//��	
						
						
						if(ParkTime<10)
						{
								LCD_ShowxNum(94,172,ParkTime,1,24,0x80);
								LCD_ShowString(106,172,200,16,24,"min");	
						}
						else if(ParkTime<100)
						{
								LCD_ShowxNum(94,172,ParkTime,2,24,0x80);
								LCD_ShowString(118,172,200,16,24,"min");	
						}
						else if(ParkTime<1000)
						{
								LCD_ShowxNum(94,172,ParkTime,3,24,0x80);
								LCD_ShowString(130,172,200,16,24,"min");	
						}
						
						RemainParkSpace+=1;
						if(RemainParkSpace>99)RemainParkSpace=99;
						LCD_ShowxNum(94,220,RemainParkSpace,2,24,0x80);	//ʣ�೵λ
						
						LCD_ShowxNum(62,100,MoneyBuffer[0],2,24,0x80);	//���
						Test_Show_CH_Font16(86,104,31,BLUE);//Ԫ
						
						LCD_ShowxNum(94,196,ParkTime,2,24,0x80);	//����
						Test_Show_CH_Font16(118,200,31,BLUE);//Ԫ
					}
					
					//��������Ƶ��2000hz, ��1s
					for(count=1000;count>0;count--)
					{
						GPIO_ResetBits(GPIOC,GPIO_Pin_5);
						delay_us(200);
						GPIO_SetBits(GPIOC,GPIO_Pin_5);
						delay_us(200);
					}	
					
					GPIO_SetBits(GPIOD,GPIO_Pin_2);							
					GPIO_SetBits(GPIOA,GPIO_Pin_8);	

				}
			}
			TIM_Cmd(TIM2, ENABLE);
		}	
		else if(NotUseCard==1)
		{
			NotUseCard=0;

			LCD_ShowString(50,80,200,16,16,"        ");//������ţ�
			LCD_ShowString(50,104,200,16,24,"        ");//������					
			LCD_ShowString(80,128,200,16,16,"                    ");//����볡ʱ��
			LCD_ShowString(80,152,200,16,16,"                    ");//�������ʱ��
			LCD_ShowString(80,176,176,16,24,"      ");//���ͣ��ʱ��
			LCD_ShowString(80,200,200,16,24,"      ");//����������
			LCD_ShowString(34,266,200,16,16,"        ");//���welcome
		}				
//		status=PcdWrite(0x01,UserInfoData);//д�����ݵ����ַ0x02
//		if(status!=0)
//		{continue;}	
	}
}



/*��ʱ��TIM2Ϊ50ms�ж�   */
void TIM2_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET) 
		{	
				TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
			
				timeCount++;
			
				if(timeCount>1000)
				{
					timeCount=0;
				}
											
				if(timeCount%5==0)
				{

					ReadDS1302Clock(DateTime);							
				}
				else if(timeCount%5==1)
				{

					LCD_ShowTime(DateTime);
				}
				else if(timeCount%5==2)
				{
					//GPIO_ResetBits(GPIOC,GPIO_Pin_5);
					if(NotUseCard==2)
					{
						NotUseCardTime++;
						if(NotUseCardTime>16)
						{
							NotUseCard=1;
						}
					}
				}
			
		}
}





