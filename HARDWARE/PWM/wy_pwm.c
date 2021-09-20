#include "wy_headfile.h"

u8 Steer1Duty = 5;

/*****************************************************
*@Func:	PWM部分初始化 （使用端口PB5输出）
******
*@para:
PWM输出初始化
mode 频率区间选择，0为Hz级（1Hz-1000Hz），1为KHZ级（1KHZ-1000KHZ)
f 频率（Hz级与KHz级均为1-1000）
duty 占空比(0-100]
******
*@desc:
TIM3 PWM部分初始化
通用定时器为T2/T3/T4/T5
PWM频率计算公式（基于定时器溢出）：f = Tclk / ((rstnum+1)*(psc+1))
系统频率1分频时（TIM_CKD_DIV1），Tclk = 72M
使用端口PB5输出（可修改）

占空比原理： 比较值 = 重载值 * 占空比(duty/100) , 当定时器计数值小于比较值时，输出低电平
******************************************************/ 
void TIM3_PWM_Init(u8 mode, u16 f, u16 duty)
{  
	u16 rstnum = 65535;  	// 重载值
    u16 psc = 65535;		// 时钟的预分频数
	u16 compare_value = 0;  // 占空比的比较值

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 // 频率区间选择
    if(mode == 0)
    {
        rstnum = 10000/f -1;
        psc = 7199;
        // 实际调节区间1/6Hz - 5000Hz  
    }
    else if(mode == 1)
    {
        rstnum = 1000/f -1;
        psc = 71;
        // 实际调节区间1/6KHz - 1000KHz
    }
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 	//Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 				// TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// 初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = rstnum; 				// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				// 设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				// 选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// 输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  						// 根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  				// 使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  										// 使能TIM3
	
	// 设置占空比（TIM3的CH2通道的输出占空比）
	if(duty > 0 && duty < 100)
	{
		compare_value = duty * (rstnum + 1) / 100;
		TIM_SetCompare2(TIM3, compare_value);
	}
}

