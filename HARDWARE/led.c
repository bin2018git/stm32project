#include "led.h"
   

//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED1-->PA5端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA5输出高，LED初始状态为灭

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED0-->PD2端口配置, 推挽输出
//  GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出，IO口速度为50MHz
//  GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PD2输出高，LED初始状态为灭
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED0-->PD2端口配置, 推挽输出
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出，IO口速度为50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD2输出高，LED初始状态为灭	
}
 
