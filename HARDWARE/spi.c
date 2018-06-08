/**********************************************
*名称：spi.c
*描述：SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C/CC1101	
*作者：li
*时间：2017-03-14
***********************************************/
#include "spi.h"
 
//SPI口初始化
void SPI1_Init(void)
{
    SPI_InitTypeDef		SPI_InitStructure;
    GPIO_InitTypeDef	GPIO_InitStructure;	
	
	/*使能GPIOA复用功能时钟，使能SPI1时钟*/
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
	
    /*对硬件stm32的SPI配置*/
    /*配置SPI的SCK,MISO,MOSI引脚，GPIOA^5,GPIOA^6,GPIOA^7 */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		
	/*PA3：CE模块信息输入  PA2：IRQ模块信息输入，可配置为IRQ*/
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置CS 引脚: GPIOA^4*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


//----------------------------------------------SPI模式------------------------------------------------//
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
									//SPI_Direction_2Lines_FullDuplex 			//双线全双工
									//SPI_Direction_2Lines_RxOnly	  			//双线接收
									//SPI_Direction_1Line_Rx				  	//单线接收
									//SPI_Direction_1Line_Tx		  			//单线发送	
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					//设置SPI工作模式:设置为主SPI
								//SPI_Mode_Slave								//从机模式
								//SPI_Mode_Master								//主机模式	
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				//设置SPI的数据大小:SPI发送接收8位帧结构
									//SPI_DataSize_16b
									//SPI_DataSize_8b	
	//※注意：CPOL和CPHA共同控制时钟信号有效边沿
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 				//时钟稳态电平
								//SPI_CPOL_High									//时钟悬空高		
								//SPI_CPOL_Low	
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						//数据捕获时钟源
								//SPI_CPHA_1Edge								
								//SPI_CPHA_2Edge								//数据捕获于第二个时钟沿	
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					//NSS信号由软件产生
								//SPI_NSS_Hard									//硬件管理
								//SPI_NSS_Soft									//软件管理(NSS)
	//※注意：最大时钟频率为10MHz								
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //2分频，72MHz/2
										//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
										//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
										//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
										//SPI_BaudRatePrescaler_64  16分频  (SPI 1.125M@sys 72M)
										//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
											//SPI_BaudRatePrescaler_256	
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				//传输起始位
									//SPI_FirstBit_MSB							//高位在前
									//SPI_FirstBit_LSB							//低位在前	
    SPI_InitStructure.SPI_CRCPolynomial = 7;                          //CRC校验复位
	
    SPI_Init(SPI1, &SPI_InitStructure);	//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
    SPI_Cmd(SPI1, ENABLE);	 //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输			
} 


//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}











