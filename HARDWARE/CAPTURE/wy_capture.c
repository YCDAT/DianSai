#include "wy_headfile.h"


u32 TIM5_CAPTURE_STA = 0;	// 输入捕获状态，给32位是为了能够捕获持续时间长的信号
u16	TIM5_CAPTURE_VAL;		// 输入捕获值


/*****************************************************
*@Func:	输入捕获初始化，用于常规捕获
******
*@para:
ocp_nvc	抢占优先级
fow_nvc 次优先级
mode 捕获精度区间选择，0为ms级（1ms-1000ms), 1为us级（1us-1000us) ，mode=0，计数精度为0.1ms,mode=1,计数精度为1us
n 捕获精度（ms级1-1000，us级1-1000）
eg： mode = 1,n= 1000,则1us计数一次,1000us进入一次中断，mode = 0,n=10,则0.1ms计数一次，10ms进入一次中断
******
*@desc:		**通用捕获不要和电容触摸输入捕获一起使用，**
通用定时器为T2/T3/T4/T5
定时器溢出时间计算公式：Tout= ((rstnum+1)*(psc+1))/Tclk
系统频率1分频时（TIM_CKD_DIV1），Tclk = 72M
定时时间最高可以达到60s
eg: 定时0.5s : rstnum = 4999, psc = 7199 (一般在此函数内部选择TIM_CKD_DIV1即可)
TIM_CKD_DIV1是最高频率了，即延时最精确在us级，此频率下延时最高可以调节到60s,但如果需要更高就需要切换了
******************************************************/
void General_Cap_Init(u8 ocp_nvc, u8 fow_nvc, u8 mode, u16 n)
{
    u16 rstnum = 65535;  	// 重载值
    u16 psc = 65535;		// 时钟的预分频数

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM5_ICInitStructure;				// 定时器5通道3输入捕获配置

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	// 使能TIM5时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	// 使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  			// PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// PA2 输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);						// 配置PA2 下拉


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
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3; 					// 通道3
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 上升沿捕获
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	// 直接映射
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			// 配置输入分频,不分频
    TIM5_ICInitStructure.TIM_ICFilter = 0x03;							// IC1F=0011 配置输入滤波器（连续采样八次）
    TIM_ICInit(TIM5, &TIM5_ICInitStructure);

    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  					// TIM5中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc;  	// 先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;  			// 从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					// IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  									// 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC3, ENABLE);				// 允许更新中断 ,允许3号通道捕获中断

    TIM_Cmd(TIM5, ENABLE ); 											// 使能定时器5

}



/*****************************************************
*@Func:	得到捕获时间
******
*@para:
******
*@desc: *注意一定要先对用于捕获的端口进行初始化（放在Gpio_Init()里实现）使用PA4测试程序运行的时间
******************************************************/
u32 GetCaptureTime(void)
{
    static u32 temp = 0;
    if(TIM5_CAPTURE_STA & 0X80000000)			// 成功捕获到了一次上升沿
    {
		LED1 = !LED1;
        temp = TIM5_CAPTURE_STA & 0X3FFFFFFF;	// 获取捕获期间定时器溢出的次数
        temp *= 1000;							// 定时器溢出时间总和（1000us溢出一次）
        temp += TIM5_CAPTURE_VAL;				// 加上寄存器中的计数值，得到总的高电平时间
        TIM5_CAPTURE_STA = 0;					// 开启下一次捕获
    }
    return temp;
}




//定时器5中断服务程序
void TIM5_IRQHandler(void)
{
    // 对定时器更新次数进行计数
    if((TIM5_CAPTURE_STA & 0X80000000) == 0)						// 还未捕获结束
    {
        if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
        {
            if(TIM5_CAPTURE_STA & 0X40000000)					// 已经捕获到高电平了
            {
                if((TIM5_CAPTURE_STA & 0X3FFFFFFF) == 0X3FFFFFFF)	// 高电平太长了，重新捕获
                {
                    TIM5_CAPTURE_STA |= 0X80000000;				// 标记成功捕获了一次
                    TIM5_CAPTURE_VAL = 0XFFFF;
                }
                else
                {
                    TIM5_CAPTURE_STA++;
                }
            }
        }
		
        // 这是对捕获情况进行检测（一个捕获动作进入两次，上升沿进入一次，下降沿进入另一次）
        if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)				// 捕获3发生捕获事件
        {
            if(TIM5_CAPTURE_STA & 0X40000000)						// 这之前已经捕获到了下降沿
            {
                TIM5_CAPTURE_STA |= 0X80000000;						// 标记这次成功捕获到的上升沿
                TIM5_CAPTURE_VAL = TIM_GetCapture3(TIM5);  			// 将寄存器中存放的定时次数读出来
                TIM_OC3PolarityConfig(TIM5, TIM_ICPolarity_Rising); // 通道3 设置为上升沿捕获
            } else  												// 还未开始,第一次捕获上升沿
            {
                TIM5_CAPTURE_STA = 0;								// 清空
                TIM5_CAPTURE_VAL = 0;
                TIM_SetCounter(TIM5, 0);
                TIM5_CAPTURE_STA |= 0X40000000;						// 标记捕获到了上升沿
                TIM_OC3PolarityConfig(TIM5, TIM_ICPolarity_Falling);// 通道3设置为下降沿捕获
            }
        }
    }

    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3 | TIM_IT_Update); 		// 清除通道3中断标志位
}

