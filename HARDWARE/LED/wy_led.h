#ifndef __WY_LED_H
#define __WY_LED_H
#include "sys.h"// ����GPIO�ڵ�ַ�ĺ궨��

// �궨��LED��GPIO��
#define LED0 PBout(5)//����LED1�Ͷ���ΪPB5�ڶ�Ӧ�ļĴ���λ�е������ˣ�LED1 = 1������PB5���λ��д1����PB5������ߵ�ƽ
#define LED1 PEout(5)

void LED_Init(void);// LED��ʼ��


#endif

