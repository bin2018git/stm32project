#include "ds1302.h"
#include "delay.h"

#define DS1302_CLK_H()	GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define DS1302_CLK_L()	GPIO_ResetBits(GPIOC,GPIO_Pin_12)

#define DS1302_RST_H()	GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define DS1302_RST_L()	GPIO_ResetBits(GPIOA,GPIO_Pin_15)

#define DS1302_OUT_H()	GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define DS1302_OUT_L()	GPIO_ResetBits(GPIOC,GPIO_Pin_11)
											
#define DS1302_IN_X		  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)

#define Time_24_Hour	0x00	//24ʱ�ƿ���	
#define Time_Start		0x00	//��ʼ��ʱ

#define DS1302_SECOND	0x80	//DS1302���Ĵ������������
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_WEEK		0x8A
#define DS1302_YEAR		0x8C
#define DS1302_WRITE	0x8E
#define DS1302_POWER	0x90

static GPIO_InitTypeDef GPIO_InitStructure;

void DS1302_Configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

	/* PE4,5,6��� */
	//GPIO_ResetBits(GPIOC,GPIO_Pin_11|GPIO_Pin_12);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void DS1302_OUT(void)
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

static void DS1302_IN(void)
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void DS1302SendByte(unsigned char byte)
{
	unsigned char	i;

	for(i=0x01;i;i<<=1)
	{
		if(byte&i)	DS1302_OUT_H();
		else	DS1302_OUT_L();
		DS1302_CLK_H();
		delay_us(2);		//����ʱ
		DS1302_CLK_L();
	}
}

unsigned char DS1302ReceiveByte(void)
{
	unsigned char	i,byte=0;

	for(i=0x01;i;i<<=1)
	{
		if(DS1302_IN_X)	byte |= i;
		DS1302_CLK_H();
		delay_us(2);		//����ʱ
		DS1302_CLK_L();
	}
	return(byte);
}

void Write1302(unsigned char addr,unsigned char data)
{
  DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	delay_us(3);
	DS1302SendByte(addr);
	DS1302SendByte(data);
	delay_us(3);
	DS1302_RST_L();
}


unsigned char Read1302(unsigned char addr)
{
    unsigned char data=0;

  DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	delay_us(3);
	DS1302SendByte(addr|0x01);
	DS1302_IN();
	data = DS1302ReceiveByte();
	delay_us(3);
	DS1302_RST_L();
	return(data);
}


//��ȡʱ�亯��
void DS1302_GetTime(unsigned char *time)
{	
	time[0] = Read1302(DS1302_YEAR);
	time[1] = Read1302(DS1302_WEEK);
	time[2] = Read1302(DS1302_MONTH);
	time[3] = Read1302(DS1302_DAY);
	time[4] = Read1302(DS1302_HOUR);
	time[5] = Read1302(DS1302_MINUTE);
	time[6] = Read1302(DS1302_SECOND);	
}


/*
��ȡDS1302�е�RAM
addr:��ַ,��0��30,��31���ֽڵĿռ�
����Ϊ����ȡ������
*/
unsigned char ReadDS1302Ram(unsigned char addr)
{
	unsigned char	tmp,res;

	tmp = (addr<<1)|0xc0;
	res = Read1302(tmp);
	return(res);
}

/*
дDS1302�е�RAM
addr:��ַ,��0��30,��31���ֽڵĿռ�
data:Ҫд������
*/
void WriteDS1302Ram(unsigned char addr,unsigned char data)
{
	unsigned char	tmp;

	Write1302(DS1302_WRITE,0x00);		//�ر�д����
	tmp = (addr<<1)|0xc0;
	Write1302(tmp,data);
	Write1302(DS1302_WRITE,0x80);		//��д����
}

void ReadDSRam(unsigned char *p,unsigned char add,unsigned char cnt)
{
	unsigned char i;
	
	if(cnt>30) return;
	for(i=0;i<cnt;i++)
	{
		*p = ReadDS1302Ram(add+1+i);
		p++;
	}
}

void WriteDSRam(unsigned char *p,unsigned char add,unsigned char cnt)
{
	unsigned char i;
	
	if(cnt>30) return;
	for(i=0;i<cnt;i++)
	{
		WriteDS1302Ram(add+1+i,*p++);
	}
}
  
/*
��ʱ�亯��,˳��Ϊ:��������ʱ����
*/
void ReadDS1302Clock(unsigned char *p)
{
	DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	delay_us(3);
	DS1302SendByte(0xbf);			//ͻ��ģʽ
	DS1302_IN();
	p[5] = DS1302ReceiveByte();		//��
	p[4] = DS1302ReceiveByte();		//��
	p[3] = DS1302ReceiveByte();		//ʱ
	p[2] = DS1302ReceiveByte();		//��
	p[1] = DS1302ReceiveByte();		//��
	p[6] = DS1302ReceiveByte();		//��
	p[0] = DS1302ReceiveByte();		//��
	DS1302ReceiveByte();			//������־�ֽ�
	delay_us(3);
	DS1302_RST_L();
}

/*
дʱ�亯��,˳��Ϊ:��������ʱ����
*/
void WriteDS1302Clock(unsigned char *p)
{
	Write1302(DS1302_WRITE,0x00);		//�ر�д����
	DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	delay_us(3);
	DS1302SendByte(0xbe);				//ͻ��ģʽ
	DS1302SendByte(p[5]);				//��
	DS1302SendByte(p[4]);				//��
	DS1302SendByte(p[3]);				//ʱ
	DS1302SendByte(p[2]);				//��
	DS1302SendByte(p[1]);				//��
	DS1302SendByte(p[6]);				//�ܣ����ó���һ��û��ʹ��
	DS1302SendByte(p[0]);				//��
	DS1302SendByte(0x80);				//������־�ֽ�
	delay_us(3);
	DS1302_RST_L();
}




void InitClock(void)
{
	unsigned char	tmp;
	unsigned char tt[7];
	//unsigned char i,dd1[30],dd2[30];
	

	DS1302_Configuration();            //ds1302�������
	
	tmp = ReadDS1302Ram(0);
	if(tmp^0xa5)
	{
		WriteDS1302Ram(0,0xa5);
		Write1302(DS1302_WRITE,0x00);		 //�ر�д����
		Write1302(0x90,0x03);				     //��ֹ������
		Write1302(DS1302_HOUR,0x00);		 //���ó�24Сʱ��
		Write1302(DS1302_SECOND,0x00);	 //ʹ��ʱ������
		Write1302(DS1302_WRITE,0x80);		 //��д����
	
	
	tt[0] = 0x17;             //0x16����ʾ16��ds1302�Ĵ�����λ����λ֮����ʮ���Ƽ���
	tt[1] = 0x05;
	tt[2] = 0x16;
	tt[3] = 0x22;
	tt[4] = 0x17;
	tt[5] = 0x00;             //17��01��09��14ʱ46��00��
	tt[6] = 0x02;							//��
	
	WriteDS1302Clock(tt);
	}
	
// 	for(i=0;i<30;i++)
// 	{
// 		dd1[i] = i;
// 		dd2[i] = 0;
// 	}
// 	
// 	WriteDSRam(dd1,0,30);
// 	ReadDSRam(dd2,0,30);
	
}
/*
void TestDS1302(void)
{
	u8 i,tt[7],dd1[30],dd2[30];
	
	DS1302_Configuration();
	InitClock();
	tt[0] = 0x10;
	tt[1] = 0x0c;
	tt[2] = 0x17;
	tt[3] = 0x11;
	tt[4] = 0x0c;
	tt[5] = 0x00;
	WriteDS1302Clock(tt);
	for(i=0;i<30;i++)
	{
		dd1[i] = i;
		dd2[i] = 0;
	}
	WriteDSRam(dd1,0,30);
	ReadDSRam(dd2,0,30);
	while(1) 
	{
		ReadDS1302Clock(tt);
	}
}
*/
//end

