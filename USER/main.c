#include "wy_headfile.h"

float tempF;				// ��ʱ�����ͱ���
u32 tempI;					// ��ʱ���ͱ���

int main(void)
{	
    delay_init();			// ��ʼ����ʱ����
	Gpio_Init();			// ��ʼ��ͨ�������
//  Key_Init();				// ��ʼ��������Ӳ���ӿ�
    Beep_Init();			// ��ʼ��Beep
	LED_Init();				// ��ʼ��LED
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(3,3,115200);	// USB���ڳ�ʼ�������ڲ�����Ϊ115200���ж����ȼ�3-3
	Blue_Init(3,1,115200);  // �������ڳ�ʼ�������ڲ�����Ϊ115200���ж����ȼ�3-1��PC10/PC11
//	EXTIX_Init();			// �ⲿ�жϳ�ʼ��
//	OLED_Init();			// ��ʼ��OLED       
//	OLED_ShowString(20,20,"WORKING",24);
//	OLED_Refresh_Gram();
//	delay_ms(1000);
//	OLED_Clear();
//	TIM3_PWM_Init(0,50,90);	  		// PWM�����ʼ����Ƶ��Ϊ50Hz, ռ�ձ�Ϊ90%����ʱ��3��ͨ��2��PB5�����
	General_Cap_Init(2,0,1,1000);	// ͨ�����벶���ʼ��������Ϊ1us, 1000us����һ���жϣ��ж����ȼ�2-0����ʱ��5ͨ��3��PA2)����
//	TPAD_Cap_Init();				// ���ݴ��������ʼ������ʱ��5ͨ��2��PA1)����   
	LCD_Init();						// TFTLCD��ʼ�� 
	LCD_SetWordColor(RED_, WHITE_); 	// ����TFTLCD��ʾ��������ɫ
//	Adc_Init();
//	MPU_Init(); 
	while(1)
    {		

		USART_SendToKeYou(UART4);					// �����������������ݵ���λ��
	}
	
}
	



