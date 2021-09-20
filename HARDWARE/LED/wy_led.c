#include "wy_headfile.h"


/*****************************************************
*@Func:	初始化LED
******
*@para: LED0:PB5  LED1:PE5
******
*@desc: 
******************************************************/	
void LED_Init(void)
{
	GPIO_InitTypeDef LED0_InitStructure;// 定义PE5的GPIO口设置结构体
	GPIO_InitTypeDef LED1_InitStructure;// 定义PB5的GPIO口设置结构体
	
	// 对所使用到的外设时钟进行使能（一定要先使能时钟再对端口进行初始化）
	// IO口使用的是APB2分频器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);// 使能APB2总线上的GPIOB 和GPIOE 的时钟。
	LED0_InitStructure.GPIO_Pin = GPIO_Pin_5;				// LED0-->PB.5 端口配置
	LED0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
	LED0_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// 设置为推挽模式
	GPIO_Init(GPIOB, &LED0_InitStructure);					// 对PB5口进行初始化
	GPIO_SetBits(GPIOB,GPIO_Pin_5);						 	// PB.5 输出高
	
	LED1_InitStructure.GPIO_Pin = GPIO_Pin_5;				// LED1-->PE.5 端口配置
	LED1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 设置IO口速度为50MHz
	LED1_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// 设置为推挽模式
	GPIO_Init(GPIOE, &LED1_InitStructure);					// 对PE5口进行初始化
	GPIO_SetBits(GPIOE,GPIO_Pin_5); 						// PE.5 输出高 

}

