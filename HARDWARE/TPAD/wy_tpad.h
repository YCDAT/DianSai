#ifndef __WY_TPAD_H
#define __WY_TPAD_H
#include "sys.h"		

extern vu16 tpad_default_val;
	
u8 TPAD_Cap_Init(void);			// ���ݲ����ʼ��
void TIM5_CH2_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n);		// ����ͨ����ʼ��
void TPAD_Reset(void);			// ����λ�����»�ȡ���ݳ��ԭʼʱ�䣩
u16 TPAD_Get_Val(void);			// �õ�����ƽ�����ʱ��
u16 TPAD_Get_MaxVal(u8 n);		// �õ����ݳ��ʱ�����һ��
u8 TPAD_Scan(u8 mode);  		// ����һ�������Ĳ����ж�

#endif
























