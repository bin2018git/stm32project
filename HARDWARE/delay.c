#include "delay.h"

static u8 fac_us=0;
static u16 fac_ms=0;

void delay_init(u8 system)
{
	 SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	 fac_us=system/8;
	 fac_ms=(u16)fac_us*1000;
}


void delay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->CTRL=0x01;
	SysTick->VAL=0X00;
	do
	{
		temp=SysTick->CTRL;
	}while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;
	SysTick->VAL=0X00;	
}


void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=(u32)nus*fac_us;
	SysTick->CTRL=0x01;
	SysTick->VAL=0X00;
	do
	{
		temp=SysTick->CTRL;
	}while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;
	SysTick->VAL=0X00;	
}	  


