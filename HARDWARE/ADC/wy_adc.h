#ifndef __WY_ADC_H
#define __WY_ADC_H	
#include "sys.h"


extern u16 adc1;
extern u16 adc2;
extern u16 adc3;

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
