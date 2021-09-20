#ifndef __WY_PWM_H
#define __WY_PWM_H
#include "sys.h" 

extern u8 Steer1Duty;

void TIM3_PWM_Init(u8 mode, u16 f, u16 duty);
	

#endif
