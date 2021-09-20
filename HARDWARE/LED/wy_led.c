#include "wy_headfile.h"


/*****************************************************
*@Func:	��ʼ��LED
******
*@para: LED0:PB5  LED1:PE5
******
*@desc: 
******************************************************/	
void LED_Init(void)
{
	GPIO_InitTypeDef LED0_InitStructure;// ����PE5��GPIO�����ýṹ��
	GPIO_InitTypeDef LED1_InitStructure;// ����PB5��GPIO�����ýṹ��
	
	// ����ʹ�õ�������ʱ�ӽ���ʹ�ܣ�һ��Ҫ��ʹ��ʱ���ٶԶ˿ڽ��г�ʼ����
	// IO��ʹ�õ���APB2��Ƶ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);// ʹ��APB2�����ϵ�GPIOB ��GPIOE ��ʱ�ӡ�
	LED0_InitStructure.GPIO_Pin = GPIO_Pin_5;				// LED0-->PB.5 �˿�����
	LED0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
	LED0_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// ����Ϊ����ģʽ
	GPIO_Init(GPIOB, &LED0_InitStructure);					// ��PB5�ڽ��г�ʼ��
	GPIO_SetBits(GPIOB,GPIO_Pin_5);						 	// PB.5 �����
	
	LED1_InitStructure.GPIO_Pin = GPIO_Pin_5;				// LED1-->PE.5 �˿�����
	LED1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����IO���ٶ�Ϊ50MHz
	LED1_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		// ����Ϊ����ģʽ
	GPIO_Init(GPIOE, &LED1_InitStructure);					// ��PE5�ڽ��г�ʼ��
	GPIO_SetBits(GPIOE,GPIO_Pin_5); 						// PE.5 ����� 

}

