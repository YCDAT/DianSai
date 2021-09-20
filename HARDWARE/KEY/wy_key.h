#ifndef __WY_KEY_H
#define __WY_KEY_H
#include "sys.h"

#define KEY0 PEin(4)	// ��ȡ����0
#define KEY1 PEin(3)	// ��ȡ����1
#define KEYUP PAin(0)	// ��ȡ����WK_UP

typedef enum
{
	Key0 = 1,
	Key1,
	KeyUp,
	
	Key0_L,
	Key1_L,
	KeyUp_L
}KEY;

typedef enum
{
	long_check = 1,
	long_uncheck
}KEYMODE;

void Key_Init(void);				// ������ʼ��
uint8_t Key_Read(KEY Key_Num);		// ������״̬
uint8_t Key_Scan(KEYMODE Key_Mode);	// ɨ�����а���


#endif
