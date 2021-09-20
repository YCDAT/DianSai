#include "wy_headfile.h"


/*****************************************************
*@Func:	��ʱ�жϳ�ʼ��
******
*@para:
timer	ʱ�Ӻŵ�ö�ٱ���
ocp_nvc	��ռ���ȼ�
fow_nvc �����ȼ�
mode ��ʱ����ѡ��0Ϊs����1s-30s����1Ϊms����1ms-1000ms), 2Ϊus����1us-1000us)
n ��ʱʱ�䣨��ͬģʽ��ѡ��Χ��ͬ,s��1-30��ms��1-1000��us��1-1000��
******
*@desc:
ͨ�ö�ʱ��ΪT2/T3/T4/T5
��ʱ�����ʱ����㹫ʽ��Tout= ((rstnum+1)*(psc+1))/Tclk
ϵͳƵ��1��Ƶʱ��TIM_CKD_DIV1����Tclk = 72M
��ʱʱ����߿��Դﵽ60s
eg: ��ʱ0.5s : rstnum = 4999, psc = 7199 (һ���ڴ˺����ڲ�ѡ��TIM_CKD_DIV1����)
TIM_CKD_DIV1�����Ƶ���ˣ�����ʱ�ȷ��us������Ƶ������ʱ��߿��Ե��ڵ�60s,�������Ҫ���߾���Ҫ�л���
******************************************************/
void TIM_Init(TIMER timer, u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// ����ֵ
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ��ʱ����ѡ��
    if(mode == 0)
    {
        rstnum = 2000 * n - 1;
        psc = 35999;
        // 1s-30s�Ƕ����û����ԣ�ʵ�ʵ�������1ms-30s
    }
    else if(mode == 1)
    {
        rstnum = 10 * n - 1;
        psc = 7199;
        // ʵ�ʵ�������0.2ms-6s
    }
    else if(mode == 2)
    {
        rstnum = n - 1;
        psc = 71;
        // ʵ�ʵ�������1us-6ms
    }

    //��ʱʱ���趨
    TIM_TimeBaseStructure.TIM_Period = rstnum;					//	�����Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;					//	����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//	ѡ��ʱ����Դ������ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //	TIM���ϼ���ģʽ

    //ѡ��ʱ�����������ȼ�
    if(timer == TIMER3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		//	ʱ��ʹ��

        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 			//	����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

        TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE ); 					//	ʹ��ָ����TIM3�ж�,���������ж�

        //�ж����ȼ�NVIC����
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  					//	TIM3�ж�
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	//	��ռ���ȼ�0��
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			//	�����ȼ�3��
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//	IRQͨ����ʹ��
        NVIC_Init(&NVIC_InitStructure);  									//	��ʼ��NVIC�Ĵ���

        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
    }
}

/*****************************************************
*@Func:	��ʱ��3�жϷ������
******
*@para:
******
*@desc:
******************************************************/
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3����(������жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  	//���TIMx�����жϱ�־
        LED1 = !LED1;
    }
}











