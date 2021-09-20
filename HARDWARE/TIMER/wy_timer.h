#ifndef __WY_TIMER_H
#define __WY_TIMER_H
#include "sys.h" 

typedef enum
{
	TIMER1 = 1,
	TIMER2,
	TIMER3,
	TIMER4,
	TIMER5,
	TIMER6,
	TIMER7	
}TIMER;

void TIM_Init(TIMER timer,u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n); //
 
#endif
