#include "wy_headfile.h"

u8 Steer1Duty = 5;

/*****************************************************
*@Func:	PWM���ֳ�ʼ�� ��ʹ�ö˿�PB5�����
******
*@para:
PWM�����ʼ��
mode Ƶ������ѡ��0ΪHz����1Hz-1000Hz����1ΪKHZ����1KHZ-1000KHZ)
f Ƶ�ʣ�Hz����KHz����Ϊ1-1000��
duty ռ�ձ�(0-100]
******
*@desc:
TIM3 PWM���ֳ�ʼ��
ͨ�ö�ʱ��ΪT2/T3/T4/T5
PWMƵ�ʼ��㹫ʽ�����ڶ�ʱ���������f = Tclk / ((rstnum+1)*(psc+1))
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
ʹ�ö˿�PB5��������޸ģ�

ռ�ձ�ԭ�� �Ƚ�ֵ = ����ֵ * ռ�ձ�(duty/100) , ����ʱ������ֵС�ڱȽ�ֵʱ������͵�ƽ
******************************************************/ 
void TIM3_PWM_Init(u8 mode, u16 f, u16 duty)
{  
	u16 rstnum = 65535;  	// ����ֵ
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��
	u16 compare_value = 0;  // ռ�ձȵıȽ�ֵ

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	 // Ƶ������ѡ��
    if(mode == 0)
    {
        rstnum = 10000/f -1;
        psc = 7199;
        // ʵ�ʵ�������1/6Hz - 5000Hz  
    }
    else if(mode == 1)
    {
        rstnum = 1000/f -1;
        psc = 71;
        // ʵ�ʵ�������1/6KHz - 1000KHz
    }
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 	//Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 				// TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// ��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = rstnum; 				// ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		// TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				// ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		// �������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  						// ����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  				// ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  										// ʹ��TIM3
	
	// ����ռ�ձȣ�TIM3��CH2ͨ�������ռ�ձȣ�
	if(duty > 0 && duty < 100)
	{
		compare_value = duty * (rstnum + 1) / 100;
		TIM_SetCompare2(TIM3, compare_value);
	}
}

