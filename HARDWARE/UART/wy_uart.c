#include "wy_headfile.h"

char USART_SEND_BUF[USART_SEND_LEN]; // ����������Ŵ����͵�����
char UsartRecData[USART_REC_LEN]; 	 // ���յ������ݣ���Ž��յ�����������

float Variable[NUM_MAX];			 // ���Ҫ���͵ı���


/*****************************************************
*@Func:	����Э�鷢������
******
*@para:
******
*@desc: Ĭ�ϵ�Э��Ϊ��@ + ���� + �س���
******************************************************/	
void USART_SendStr_AsDeal(USART_TypeDef* USARTx, char* Uart_Str)
{
    char* p = Uart_Str;
    USART_SendData(USARTx, DATAHEAD);               //�����׼����
    while(*p != '\0')
    {
        USART_SendData(USARTx, *p++); // ������һλ����
    }
    USART_SendData(USARTx, DATATAIL1);               //����β�����1
    USART_SendData(USARTx, DATATAIL2);               //����β�����2
}

void USART_SendStr(USART_TypeDef* USARTx, char* Uart_Str)
{
    char* p = Uart_Str;
    while(*p != '\0')
    {
        USART_SendData(USARTx, *p++); // ������һλ����
    }
}

void USART_SendInt(USART_TypeDef* USARTx, int Uart_Int)
{
    char Uart_str[10];
    sprintf((char*)Uart_str, "%5d", Uart_Int); // ��u16) ��ӡ��(char)��
    USART_SendStr(USARTx, Uart_str);
}


void USART_SendFloat(USART_TypeDef* USARTx, float Uart_Float)
{
    char Uart_str[10];
    sprintf((char*)Uart_str, "%7.2f", Uart_Float); // ��u16) ��ӡ��(char)��
    USART_SendStr(USARTx, Uart_str);
}

/*****************************************************
*@Func:	�������ڳ�ʼ��
******
*@para:
******
*@desc: 
******************************************************/	
void Blue_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound) 
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��UART4ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��GPIOCʱ��

    //USART4_TX   PC10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				// PC10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			// �����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);					// ��ʼ��Pc10

    //USART4_RX	  PC11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				// PC11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// ��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);					// ��ʼ��Pc11

//    //Usart3 NVIC ����
//    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc ; 	// ��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;			// �����ȼ�1
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);									// ����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;					// ���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		// һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;			// ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// �շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); 										// ��ʼ������4
//    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);								// �������ڽ����ж�
    USART_Cmd(UART4, ENABLE);                    									// ʹ�ܴ���4
}



/*****************************************************
*@Func:	�����������ͱ��������ſƴ���������
******
*@para:
******
*@desc: 
******************************************************/		
void USART_SendToKeYou(USART_TypeDef* USARTx)
{
    u8 i;
    float temp = 0.0;
    u8 ch;
	
	/*���������Ҫ���͵ı�����ע������ĸ������ܳ����趨������͸���NUM_MAX*/
    Variable[0] =  UlCurDistance;
//	Variable[1] =  gyrox;
//	Variable[2] =  gyroy;
//	Variable[3] =  gyroz;
//	Variable[4] =  aacx;
//	Variable[5] =  aacy;
//	Variable[6] =  aacz;
//	Variable[7] =  Temp;
//	Variable[8] =  pitch;
//	Variable[9] =  roll;
//	Variable[10] =  yaw;
//	Variable[11] =  tempI;
//	Variable[12] =  bias;
//	Variable[13] =  angle_accl;
//	Variable[14] = angle_cur;
//	Variable[15] = Balance_pid.errIntg;


	/*���������ſƴ���������֮���ͨ��Э��*/
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0x11);
	USART_SendData(USARTx, 0xaa);
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0xaa);
	USART_SendData(USARTx, 0xff);
	USART_SendData(USARTx, 0x01);
	USART_SendData(USARTx, NUM_MAX);

    for(i = 0; i < NUM_MAX; i++)// ��λ����(stm32���õ��Ǵ�˴洢���ݵķ�ʽ�������ֽڴ���ڸߵ�ַ��)
    {
        temp = Variable[i];

		ch = BYTE0(temp);
		USART_SendData(USARTx, ch);
		
        ch = BYTE1(temp);
        USART_SendData(USARTx, ch);

        ch = BYTE2(temp);
        USART_SendData(USARTx, ch);

        ch = BYTE3(temp);
        USART_SendData(USARTx, ch);
    }
	
    USART_SendData(USARTx, 0x01);// βУ���
}

