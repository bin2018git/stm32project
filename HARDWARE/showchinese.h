#ifndef __CHINESE_H
#define __CHINESE_H	 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板——扩展实验
//自定义汉字显示  代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/7/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


void TEST_FONT(void);
void Test_Show_CH_Font16(u16 x,u16 y,u8 index,u16 color);
void Test_Show_CH_Font24(u16 x,u16 y,u8 index,u16 color);		  				    
#endif
