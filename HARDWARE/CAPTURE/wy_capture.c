#include "wy_headfile.h"


u32 TIM5_CAPTURE_STA = 0;	// ���벶��״̬����32λ��Ϊ���ܹ��������ʱ�䳤���ź�
u16	TIM5_CAPTURE_VAL;		// ���벶��ֵ


/*****************************************************
*@Func:	���벶���ʼ�������ڳ��沶��
******
*@para:
ocp_nvc	��ռ���ȼ�
fow_nvc �����ȼ�
mode ���񾫶�����ѡ��0Ϊms����1ms-1000ms), 1Ϊus����1us-1000us) ��mode=0����������Ϊ0.1ms,mode=1,��������Ϊ1us
n ���񾫶ȣ�ms��1-1000��us��1-1000��
eg�� mode = 1,n= 1000,��1us����һ��,1000us����һ���жϣ�mode = 0,n=10,��0.1ms����һ�Σ�10ms����һ���ж�
******
*@desc:		**ͨ�ò���Ҫ�͵��ݴ������벶��һ��ʹ�ã�**
ͨ�ö�ʱ��ΪT2/T3/T4/T5
��ʱ�����ʱ����㹫ʽ��Tout= ((rstnum+1)*(psc+1))/Tclk
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
��ʱʱ����߿��Դﵽ60s
eg: ��ʱ0.5s : rstnum = 4999, psc = 7199 (һ���ڴ˺����ڲ�ѡ��TIM_CKD_DIV1����)
TIM_CKD_DIV1�����Ƶ���ˣ�����ʱ�ȷ��us������Ƶ������ʱ��߿��Ե��ڵ�60s,�������Ҫ���߾���Ҫ�л���
******************************************************/
void General_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// ����ֵ
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM5_ICInitStructure;				// ��ʱ��5ͨ��3���벶������

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	// ʹ��TIM5ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	// ʹ��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  			// PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// PA2 ����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);						// ����PA2 ����


    if(mode == 0)
    {
        rstnum = 10 * n - 1;
        psc = 7199;			// 0.1ms ����һ��,����Ϊ0.1ms
        // ʵ�ʵ�������0.2ms-6s
    }
    else if(mode == 1)
    {
        rstnum = n - 1;
        psc = 71; 			// 1us ����һ��,����Ϊ1us
        // ʵ�ʵ�������1us-6ms
    }

    //��ʼ����ʱ��5 TIM5
    TIM_TimeBaseStructure.TIM_Period = rstnum; 				// �趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 				// Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		// TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 					// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM5���벶�����
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3; 					// ͨ��3
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// �����ز���
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	// ֱ��ӳ��
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			// ���������Ƶ,����Ƶ
    TIM5_ICInitStructure.TIM_ICFilter = 0x03;							// IC1F=0011 ���������˲��������������˴Σ�
    TIM_ICInit(TIM5, &TIM5_ICInitStructure);

    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  					// TIM5�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	// ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			// �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					// IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  									// ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC3, ENABLE);				// ��������ж� ,����3��ͨ�������ж�

    TIM_Cmd(TIM5, ENABLE ); 											// ʹ�ܶ�ʱ��5

}



/*****************************************************
*@Func:	�õ�����ʱ��
******
*@para:
******
*@desc: *ע��һ��Ҫ�ȶ����ڲ���Ķ˿ڽ��г�ʼ��������Gpio_Init()��ʵ�֣�ʹ��PA4���Գ������е�ʱ��
******************************************************/
u32 GetCaptureTime(void)
{
    static u32 temp = 0;
    if(TIM5_CAPTURE_STA & 0X80000000)			// �ɹ�������һ��������
    {
		LED1 = !LED1;
        temp = TIM5_CAPTURE_STA & 0X3FFFFFFF;	// ��ȡ�����ڼ䶨ʱ������Ĵ���
        temp *= 1000;							// ��ʱ�����ʱ���ܺͣ�1000us���һ�Σ�
        temp += TIM5_CAPTURE_VAL;				// ���ϼĴ����еļ���ֵ���õ��ܵĸߵ�ƽʱ��
        TIM5_CAPTURE_STA = 0;					// ������һ�β���
    }
    return temp;
}




//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)
{
    // �Զ�ʱ�����´������м���
    if((TIM5_CAPTURE_STA & 0X80000000) == 0)						// ��δ�������
    {
        if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
        {
            if(TIM5_CAPTURE_STA & 0X40000000)					// �Ѿ����񵽸ߵ�ƽ��
            {
                if((TIM5_CAPTURE_STA & 0X3FFFFFFF) == 0X3FFFFFFF)	// �ߵ�ƽ̫���ˣ����²���
                {
                    TIM5_CAPTURE_STA |= 0X80000000;				// ��ǳɹ�������һ��
                    TIM5_CAPTURE_VAL = 0XFFFF;
                }
                else
                {
                    TIM5_CAPTURE_STA++;
                }
            }
        }
		
        // ���ǶԲ���������м�⣨һ���������������Σ������ؽ���һ�Σ��½��ؽ�����һ�Σ�
        if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)				// ����3���������¼�
        {
            if(TIM5_CAPTURE_STA & 0X40000000)						// ��֮ǰ�Ѿ��������½���
            {
                TIM5_CAPTURE_STA |= 0X80000000;						// �����γɹ����񵽵�������
                TIM5_CAPTURE_VAL = TIM_GetCapture3(TIM5);  			// ���Ĵ����д�ŵĶ�ʱ����������
                TIM_OC3PolarityConfig(TIM5, TIM_ICPolarity_Rising); // ͨ��3 ����Ϊ�����ز���
            } else  												// ��δ��ʼ,��һ�β���������
            {
                TIM5_CAPTURE_STA = 0;								// ���
                TIM5_CAPTURE_VAL = 0;
                TIM_SetCounter(TIM5, 0);
                TIM5_CAPTURE_STA |= 0X40000000;						// ��ǲ�����������
                TIM_OC3PolarityConfig(TIM5, TIM_ICPolarity_Falling);// ͨ��3����Ϊ�½��ز���
            }
        }
    }

    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3 | TIM_IT_Update); 		// ���ͨ��3�жϱ�־λ
}

