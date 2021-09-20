#include "wy_headfile.h"


/*****************************************************
*@Func:	初始化蜂鸣器
******
*@para:
******
*@desc: 
******************************************************/	
void Beep_Init(void)
{
	GPIO_InitTypeDef Beep_InitStructure;				// 定义IO口初始化结构体
	// IO口时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	// IO口初始化
	Beep_InitStructure.GPIO_Pin = GPIO_Pin_8;
	Beep_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	Beep_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &Beep_InitStructure);				// 初始化Beep的PB8口
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);					// Beep口置0
}
