#include "wy_headfile.h"

/*****************************************************
@ Func:	������ʼ��
******
@ para: 
******
@ desc:	KEY0: PE3   KEY1:PE4   KEY_UP:PA0
******************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef Key0_InitStructure;// ����0��IO�ڳ�ʼ���ṹ��
    GPIO_InitTypeDef Key1_InitStructure;// ����1��IO�ڳ�ʼ���ṹ��
    GPIO_InitTypeDef KeyUp_InitStructure;// ����up��IO�ڳ�ʼ���ṹ��
    // Key��Ӳ���˿�ʱ�ӳ�ʼ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA, ENABLE);
    // Key��IO�ڳ�ʼ��
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
@ Func:	��ȡָ������״̬
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
@ Func:	ɨ�����а��������ؼ�Ⲣ���������°����İ�����
******
@ para:
******
@ desc: ���������˳�������ȼ�KEY0>KEY1>KEYUP*****���������ʱ����ʹ*****
******************************************************/
uint8_t Key_Scan(KEYMODE Key_Mode)
{
    static uint8_t KeyFlag = 0;// ����Ϊ��̬�����Ա�������״̬
    static u16 KeyStayTimes = 0;
    if(KEY0 == 0 && KeyFlag == 0)// ����Ƿ��а���������
    {
        printf("0");
        KeyStayTimes = 0;
        KeyFlag = 1;
        delay_ms(10);// ��������
    }
    else if (KEY1 == 0 && KeyFlag == 0)
    {
        printf("0");
        KeyStayTimes = 0;
        KeyFlag = 2;
        delay_ms(10);// ��������
    }
    else if (KEYUP == 1 && KeyFlag == 0)
    {
        printf("0");
        KeyStayTimes = 0;
        KeyFlag = 3;
        delay_ms(10);// ��������
    }
    else if(KEY0 == 1 && KeyFlag == 1)// ������������ʱ������Ӧ������
    {
        printf("1\n");
        KeyFlag = 0;
        delay_ms(10);// ��������
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
        delay_ms(10);// ��������
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
        delay_ms(10);// ��������			
		if(Key_Mode == long_check && KeyStayTimes > 50000)
		{
			printf("13");
			KeyStayTimes =0;
			return KeyUp_L;
		}
		else
			return KeyUp;
    }
    else if(KEY0 == 0 || KEY1 == 0 || KEYUP == 1)// �������ּ�ʱ
    {
        KeyStayTimes++;
    }
    else
    {
        return 0;
    }
	return 0;
}




