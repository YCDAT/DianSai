#ifndef _WY_ULTRASONIC_H_
#define _WY_ULTRASONIC_H_
#include "sys.h"

#define TRIG PAout(6)

//extern uchar TrigFlag;
extern u32 UlTansferTime;          	// �ɼ����ĸߵ�ƽʱ��
extern u32 UlCurDistance;          	// ��ǰ����

void TrigUl(void);					// �������������;�������
void UlGetDistance(void);			// ��ȡ��ǰ����

#endif
