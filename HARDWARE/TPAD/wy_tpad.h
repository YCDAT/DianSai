#ifndef __WY_TPAD_H
#define __WY_TPAD_H
#include "sys.h"		

extern vu16 tpad_default_val;
	
u8 TPAD_Cap_Init(void);			// 电容捕获初始化
void TIM5_CH2_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n);		// 捕获通道初始化
void TPAD_Reset(void);			// 捕获复位（重新获取电容充电原始时间）
u16 TPAD_Get_Val(void);			// 得到电容平均充电时间
u16 TPAD_Get_MaxVal(u8 n);		// 得到电容充电时间最长的一次
u8 TPAD_Scan(u8 mode);  		// 进行一次完整的捕获判断

#endif
























