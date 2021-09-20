#include "wy_headfile.h"

#define TPAD_ARR_MAX_VAL 	0XFFFF	// ����ARRֵ
vu16 tpad_default_val = 0;			// ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��


/*****************************************************
*@Func:	��ʼ��������������ÿ��ص�ʱ����������ȡֵ.
******
*@para:
����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
******
*@desc: ������ǵ��ݳ���ʱ�䣬û�з����ֵ�ʱ����һ������ʱ�䡢�����ֵ�ʱ������һ����������Ϊ�ж��Ƿ��� 
******************************************************/
u8 TPAD_Cap_Init(void)
{
	u16 buf[10];
	u16 temp;
	u8 j,i;
	TIM5_CH2_Cap_Init(2,0,1,TPAD_ARR_MAX_VAL);	// ���벶���ʼ��������Ϊ1us���������ֵTPAD_ARR_MAX_VAL����ʱ��5ͨ��2��PA1)����
	for(i=0;i<10;i++)// ������ȡ10��
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)// ����
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])// ��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];// ȡ�м��6�����ݽ���ƽ��
	tpad_default_val=temp/6;
//	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	// if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;// ��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������!
	return 0;		     	    					   
}



/*****************************************************
*@Func:	���벶���ʼ�������ڵ��ݴ�����������
******
*@para:
ocp_nvc	��ռ���ȼ�
fow_nvc �����ȼ�
mode ���񾫶�����ѡ��0Ϊms����1ms-1000ms), 1Ϊus����1us-1000us) ��mode=0����������Ϊ0.1ms,mode=1,��������Ϊ1us
n ���񾫶ȣ�ms��1-1000��us��1-1000��
eg�� mode = 1,n= 1000,��1us����һ��,1000us����һ���жϣ�mode = 0,n=10,��0.1ms����һ�Σ�10ms����һ���ж�
******
*@desc: 
******************************************************/
void TIM5_CH2_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// ����ֵ
    u16 psc = 65535;		// ʱ�ӵ�Ԥ��Ƶ��

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM5_ICInitStructure;				// ��ʱ��5ͨ��2���벶������
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	// ʹ��TIM5ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	// ʹ��GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  			// PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);						// ����PA1 ����


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
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; 					// ѡ������� ͨ��2
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// �����ز���
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			// ���������Ƶ,����Ƶ
    TIM5_ICInitStructure.TIM_ICFilter = 0x03;							// IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
    TIM_ICInit(TIM5, &TIM5_ICInitStructure);

    TIM_Cmd(TIM5, ENABLE ); 											// ʹ�ܶ�ʱ��5

}



/*****************************************************
*@Func:	TPAD��λһ��(��������͵�ƽ���������ݷŵ磩
******
*@para:
******
*@desc:
******************************************************/
void TPAD_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 // ʹ��PA�˿�ʱ��
	
	//����GPIOA.1Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // PA1 �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 // �������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 // PA.1���0,�ŵ�

	delay_ms(5);

	TIM_SetCounter(TIM5,0);									 // ��ʱ��������0
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2|TIM_IT_Update); 	 // ����жϱ�־
	//����GPIOA.1Ϊ��������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 	 // ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/*****************************************************
*@Func:	�õ���ʱ������ֵ
******
*@para:
******
*@desc:	�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
******************************************************/
u16 TPAD_Get_Val(void)
{				   
	TPAD_Reset();// �������ݷŵ�
	while(TIM_GetFlagStatus(TIM5, TIM_IT_CC2) == RESET)// ����־λ�ȴ�����������
	{
		if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL-500)
			return TIM_GetCounter(TIM5);// ��ʱ��,ֱ�ӷ���CNT��ֵ
	};	
	return TIM_GetCapture2(TIM5);	  
} 	


/*****************************************************
*@Func:	��ȡn��,ȡ���ֵ
******
*@para: n��������ȡ�Ĵ���
******
*@desc:	����ֵ��n�ζ������������������ֵ
******************************************************/
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();// �õ�һ��ֵ
		if(temp>res)res=temp;
	};
	return res;
}


/*****************************************************
*@Func:	ɨ�败������
******
*@para: mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
******
*@desc:	����ֵ:0,û�а���;1,�а���;
******************************************************/								  
#define TPAD_GATE_VAL 	10	// ��������ֵ��Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;		// 0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;			// Ĭ�ϲ�������Ϊ3��	 
	u16 maxval;
	if(mode)
	{
		sample=6;			// ֧��������ʱ�����ò�������Ϊ6��
		keyen=0;			// ֧������	  
	}
	maxval=TPAD_Get_MaxVal(sample);			 		// ��ȡ���β����е����ֵ 
	if(maxval>(tpad_default_val+TPAD_GATE_VAL))		// ����tpad_default_val+TPAD_GATE_VAL,��Ч
	{	
		if(mode)delay_ms(100);				 		// ֧����������ģʽ��Ҳ�ܼ�⵽���δ���
		if(keyen==0)res=1;					 		// keyen==0,��Ч
//		LCD_ShowInt(30,240,200,16,16,maxval);		// ��ʾ�ִ���ʱ���ݵĳ���ʱ�� 
//		printf("r:%d\r\n",maxval);		     	   	// ��ӡ�ִ���ʱ���ݵĳ���ʱ�� 					   
		keyen=3;							 		// ����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}

