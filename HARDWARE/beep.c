#include "beep.h"
   

//��ʼ��PAC5Ϊ�����.��ʹ������ڵ�ʱ��		    
//BEEP IO��ʼ��
void BEEP_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PC5�˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //���������IO���ٶ�Ϊ50MHz
	GPIO_ResetBits(GPIOC,GPIO_Pin_5); 						 //PC5����ͣ���������ʼ״̬Ϊ�ر�

}

