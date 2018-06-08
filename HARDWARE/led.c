#include "led.h"
   

//��ʼ��PA8��PD2Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED1-->PA5�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA5����ߣ�LED��ʼ״̬Ϊ��

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED0-->PD2�˿�����, �������
//  GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ50MHz
//  GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PD2����ߣ�LED��ʼ״̬Ϊ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED0-->PD2�˿�����, �������
 GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD2����ߣ�LED��ʼ״̬Ϊ��	
}
 
