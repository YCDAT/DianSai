#include "wy_headfile.h"

/*****************************************************
@ Func:	按键初始化
******
@ para: 
******
@ desc:	KEY0: PE3   KEY1:PE4   KEY_UP:PA0
******************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef Key0_InitStructure;// 按键0的IO口初始化结构体
    GPIO_InitTypeDef Key1_InitStructure;// 按键1的IO口初始化结构体
    GPIO_InitTypeDef KeyUp_InitStructure;// 按键up的IO口初始化结构体
    // Key的硬件端口时钟初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA, ENABLE);
    // Key的IO口初始化
    Key0_InitStructure.GPIO_Pin = GPIO_Pin_4;
    Key0_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    Key0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    Key1_InitStructure.GPIO_Pin = GPIO_Pin_3;
    Key1_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    Key1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    KeyUp_InitStructure.GPIO_Pin = GPIO_Pin_0;
    KeyUp_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    KeyUp_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &Key0_InitStructure);
    GPIO_Init(GPIOE, &Key1_InitStructure);
    GPIO_Init(GPIOA, &KeyUp_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}


/*****************************************************
@ Func:	读取指定按键状态
******
@ para:
******
@ desc:
******************************************************/
uint8_t Key_Read(KEY Key_Num)
{
    if(Key_Num == KeyUp)
    {
        return KEYUP;
    }
    else if(Key_Num == Key0)
    {
        return KEY0;
    }
    else if(Key_Num == Key1)
    {
        return KEY1;
    }
    return 0;
}


/*****************************************************
@ Func:	扫描所有按键并返回检测并返回所摁下按键的按键号
******
@ para:
******
@ desc: 各按键检测顺序有优先级KEY0>KEY1>KEYUP*****长摁检测暂时不好使*****
******************************************************/
uint8_t Key_Scan(KEYMODE Key_Mode)
{
    static uint8_t KeyFlag = 0;// 声明为静态变量以保留按键状态
    static u16 KeyStayTimes = 0;
    if(KEY0 == 0 && KeyFlag == 0)// 检测是否有按键被摁下
    {
        printf("0");
        KeyStayTimes = 0;
        KeyFlag = 1;
        delay_ms(10);// 按键消抖
    }
    else if (KEY1 == 0 && KeyFlag == 0)
    {
        printf("0");
        KeyStayTimes = 0;
        KeyFlag = 2;
        delay_ms(10);// 按键消抖
    }
    else if (KEYUP == 1 && KeyFlag == 0)
    {
        printf("0");
        KeyStayTimes = 0;
        KeyFlag = 3;
        delay_ms(10);// 按键消抖
    }
    else if(KEY0 == 1 && KeyFlag == 1)// 当按键被弹起时返回相应按键号
    {
        printf("1\n");
        KeyFlag = 0;
        delay_ms(10);// 按键消抖
		if(Key_Mode == long_check && KeyStayTimes > 50000)
		{
			printf("11");
			KeyStayTimes =0;
			return Key0_L;
		}
		else
			return Key0;
    }
    else if(KEY1 == 1 && KeyFlag == 2)
    {
        printf("2\n");
        KeyFlag = 0;
        delay_ms(10);// 按键消抖
		if(Key_Mode == long_check && KeyStayTimes > 50000)
		{
			printf("12");
			KeyStayTimes =0;
			return Key1_L;
		}
		else
			return Key1;
    }
    else if(KEYUP == 0 && KeyFlag == 3)
    {
        printf("3\n");
        KeyFlag = 0;
        delay_ms(10);// 按键消抖			
		if(Key_Mode == long_check && KeyStayTimes > 50000)
		{
			printf("13");
			KeyStayTimes =0;
			return KeyUp_L;
		}
		else
			return KeyUp;
    }
    else if(KEY0 == 0 || KEY1 == 0 || KEYUP == 1)// 按键保持计时
    {
        KeyStayTimes++;
    }
    else
    {
        return 0;
    }
	return 0;
}




