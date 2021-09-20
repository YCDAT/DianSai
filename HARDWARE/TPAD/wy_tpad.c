#include "wy_headfile.h"

#define TPAD_ARR_MAX_VAL 	0XFFFF	// 最大的ARR值
vu16 tpad_default_val = 0;			// 空载的时候(没有手按下),计数器需要的时间


/*****************************************************
*@Func:	初始化触摸按键、获得空载的时候触摸按键的取值.
******
*@para:
返回值:0,初始化成功;1,初始化失败
******
*@desc: 捕获的是电容充电的时间，没有放置手的时候是一个捕获时间、放置手的时候是另一个，依次作为判断是否触摸 
******************************************************/
u8 TPAD_Cap_Init(void)
{
	u16 buf[10];
	u16 temp;
	u8 j,i;
	TIM5_CH2_Cap_Init(2,0,1,TPAD_ARR_MAX_VAL);	// 输入捕获初始化，精度为1us，计数最大值TPAD_ARR_MAX_VAL，定时器5通道2（PA1)输入
	for(i=0;i<10;i++)// 连续读取10次
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)// 排序
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])// 升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];// 取中间的6个数据进行平均
	tpad_default_val=temp/6;
//	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	// if(tpad_default_val>TPAD_ARR_MAX_VAL/2)return 1;// 初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
	return 0;		     	    					   
}



/*****************************************************
*@Func:	输入捕获初始化，用于电容触摸按键捕获
******
*@para:
ocp_nvc	抢占优先级
fow_nvc 次优先级
mode 捕获精度区间选择，0为ms级（1ms-1000ms), 1为us级（1us-1000us) ，mode=0，计数精度为0.1ms,mode=1,计数精度为1us
n 捕获精度（ms级1-1000，us级1-1000）
eg： mode = 1,n= 1000,则1us计数一次,1000us进入一次中断，mode = 0,n=10,则0.1ms计数一次，10ms进入一次中断
******
*@desc: 
******************************************************/
void TIM5_CH2_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// 重载值
    u16 psc = 65535;		// 时钟的预分频数

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM5_ICInitStructure;				// 定时器5通道2输入捕获配置
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	// 使能TIM5时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	// 使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;  			// PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// 下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);						// 配置PA1 下拉


    if(mode == 0)
    {
        rstnum = 10 * n - 1;
        psc = 7199;			// 0.1ms 计数一次,精度为0.1ms
        // 实际调节区间0.2ms-6s
    }
    else if(mode == 1)
    {
        rstnum = n - 1;
        psc = 71; 			// 1us 计数一次,精度为1us
        // 实际调节区间1us-6ms
    }

    //初始化定时器5 TIM5
    TIM_TimeBaseStructure.TIM_Period = rstnum; 				// 设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; 				// 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		// TIM向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 					// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM5输入捕获参数
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; 					// 选择输入端 通道2
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 上升沿捕获
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			// 配置输入分频,不分频
    TIM5_ICInitStructure.TIM_ICFilter = 0x03;							// IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM5, &TIM5_ICInitStructure);

    TIM_Cmd(TIM5, ENABLE ); 											// 使能定时器5

}



/*****************************************************
*@Func:	TPAD复位一次(引脚输出低电平给触摸电容放电）
******
*@para:
******
*@desc:
******************************************************/
void TPAD_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 // 使能PA端口时钟
	
	//设置GPIOA.1为推挽使出
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 // PA1 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 // 推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_1);						 // PA.1输出0,放电

	delay_ms(5);

	TIM_SetCounter(TIM5,0);									 // 定时器计数归0
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC2|TIM_IT_Update); 	 // 清除中断标志
	//设置GPIOA.1为浮空输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 	 // 浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/*****************************************************
*@Func:	得到定时器捕获值
******
*@para:
******
*@desc:	如果超时,则直接返回定时器的计数值.
******************************************************/
u16 TPAD_Get_Val(void)
{				   
	TPAD_Reset();// 触摸电容放电
	while(TIM_GetFlagStatus(TIM5, TIM_IT_CC2) == RESET)// 检测标志位等待捕获上升沿
	{
		if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL-500)
			return TIM_GetCounter(TIM5);// 超时了,直接返回CNT的值
	};	
	return TIM_GetCapture2(TIM5);	  
} 	


/*****************************************************
*@Func:	读取n次,取最大值
******
*@para: n：连续获取的次数
******
*@desc:	返回值：n次读数里面读到的最大读数值
******************************************************/
u16 TPAD_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=TPAD_Get_Val();// 得到一次值
		if(temp>res)res=temp;
	};
	return res;
}


/*****************************************************
*@Func:	扫描触摸按键
******
*@para: mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按着)
******
*@desc:	返回值:0,没有按下;1,有按下;
******************************************************/								  
#define TPAD_GATE_VAL 	10	// 触摸的阈值，也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;		// 0,可以开始检测;>0,还不能开始检测	 
	u8 res=0;
	u8 sample=3;			// 默认采样次数为3次	 
	u16 maxval;
	if(mode)
	{
		sample=6;			// 支持连按的时候，设置采样次数为6次
		keyen=0;			// 支持连按	  
	}
	maxval=TPAD_Get_MaxVal(sample);			 		// 获取几次采样中的最大值 
	if(maxval>(tpad_default_val+TPAD_GATE_VAL))		// 大于tpad_default_val+TPAD_GATE_VAL,有效
	{	
		if(mode)delay_ms(100);				 		// 支持连续触发模式下也能检测到单次触发
		if(keyen==0)res=1;					 		// keyen==0,有效
//		LCD_ShowInt(30,240,200,16,16,maxval);		// 显示手触摸时电容的充电的时间 
//		printf("r:%d\r\n",maxval);		     	   	// 打印手触摸时电容的充电的时间 					   
		keyen=3;							 		// 至少要再过3次之后才能按键有效   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}

