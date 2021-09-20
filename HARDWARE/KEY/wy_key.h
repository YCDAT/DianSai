#ifndef __WY_KEY_H
#define __WY_KEY_H
#include "sys.h"

#define KEY0 PEin(4)	// 读取按键0
#define KEY1 PEin(3)	// 读取按键1
#define KEYUP PAin(0)	// 读取按键WK_UP

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

void Key_Init(void);				// 按键初始化
uint8_t Key_Read(KEY Key_Num);		// 读按键状态
uint8_t Key_Scan(KEYMODE Key_Mode);	// 扫描所有按键


#endif
