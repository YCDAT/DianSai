#include "wy_headfile.h"


/*****************************************************
*@Func:	��ʼ��������
******
*@para:
******
*@desc: 
******************************************************/	
void Beep_Init(void)
{
	GPIO_InitTypeDef Beep_InitStructure;				// ����IO�ڳ�ʼ���ṹ��
	// IO��ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	// IO�ڳ�ʼ��
	Beep_InitStructure.GPIO_Pin = GPIO_Pin_8;
	Beep_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	Beep_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &Beep_InitStructure);				// ��ʼ��Beep��PB8��
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);					// Beep����0
}
