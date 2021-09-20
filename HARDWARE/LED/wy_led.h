#ifndef __WY_LED_H
#define __WY_LED_H
#include "sys.h"// 引入GPIO口地址的宏定义

// 宏定义LED的GPIO口
#define LED0 PBout(5)//这样LED1就定义为PB5口对应的寄存器位中的内容了，LED1 = 1，就往PB5这个位就写1，即PB5口输出高电平
#define LED1 PEout(5)

void LED_Init(void);// LED初始化


#endif

