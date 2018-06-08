#include "beep.h"
   

//初始化PAC5为输出口.并使能这个口的时钟		    
//BEEP IO初始化
void BEEP_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PC5端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出，IO口速度为50MHz
	GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PC5输出低，蜂鸣器初始状态为关闭

}


