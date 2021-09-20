#include "wy_headfile.h"

float tempF;				// 临时浮点型变量
u32 tempI;					// 临时整型变量

int main(void)
{	
    delay_init();			// 初始化延时函数
	Gpio_Init();			// 初始化通用输出口
//  Key_Init();				// 初始化按键的硬件接口
    Beep_Init();			// 初始化Beep
	LED_Init();				// 初始化LED
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(3,3,115200);	// USB串口初始化，串口波特率为115200，中断优先级3-3
	Blue_Init(3,1,115200);  // 蓝牙串口初始化，串口波特率为115200，中断优先级3-1，PC10/PC11
//	EXTIX_Init();			// 外部中断初始化
//	OLED_Init();			// 初始化OLED       
//	OLED_ShowString(20,20,"WORKING",24);
//	OLED_Refresh_Gram();
//	delay_ms(1000);
//	OLED_Clear();
//	TIM3_PWM_Init(0,50,90);	  		// PWM输出初始化，频率为50Hz, 占空比为90%，定时器3的通道2（PB5）输出
	General_Cap_Init(2,0,1,1000);	// 通用输入捕获初始化，精度为1us, 1000us进入一次中断，中断优先级2-0，定时器5通道3（PA2)输入
//	TPAD_Cap_Init();				// 电容触摸捕获初始化，定时器5通道2（PA1)输入   
	LCD_Init();						// TFTLCD初始化 
	LCD_SetWordColor(RED_, WHITE_); 	// 设置TFTLCD显示的字体颜色
//	Adc_Init();
//	MPU_Init(); 
	while(1)
    {		

		USART_SendToKeYou(UART4);					// 蓝牙发送陀螺仪数据到上位机
	}
	
}
	



