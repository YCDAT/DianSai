#ifndef __WY_CAPTURE_H
#define __WY_CAPTURE_H
#include "sys.h"

extern u16	TIM5_CAPTURE_VAL;
extern u32  TIM5_CAPTURE_STA;
void General_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n);
u32 GetCaptureTime(void);
#endif
